require 'fileutils'
require 'date'

THIS_DIR = File.dirname(__FILE__)

SOURCE_ROOT = "#{THIS_DIR}/../"
EXE_NAME = 'Debugify'
BUILD_ROOT = "#{THIS_DIR}/../../"

class BundleBuilder
  ENTRY = '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>

'
  EXIT = '

</dict>
</plist>'

  CONTENTS = "Contents/"
  CONTENTS_MACOS = "#{CONTENTS}MacOS/"
  CONTENTS_RESOURCES = "#{CONTENTS}Resources/"

  def initialize(name, plistKeys)
    @root = name
    FileUtils.mkdir_p(name)
    FileUtils.mkdir_p("#{name}/#{CONTENTS_MACOS}")
    FileUtils.mkdir_p("#{name}/#{CONTENTS_RESOURCES}")

    File.open("#{name}/#{CONTENTS}Info.plist", 'w') do |f|
      f.write(makePlist(plistKeys))
    end
  end

  def self.cleanup(file)
    if (File.directory?(file))
      FileUtils.rm_rf(file)
    end
  end

  def mkdir(name)
    FileUtils.mkdir_p("#{@root}/#{CONTENTS_MACOS}#{name}")
  end

  def mv(src, dest)
    FileUtils.mv("#{@root}/#{CONTENTS_MACOS}/#{src}", "#{@root}/#{CONTENTS_MACOS}/#{dest}")
  end

  def path(file)
    return "#{@root}/#{CONTENTS_MACOS}#{file}"
  end

  def open(file)
    File.open(path(file), 'w') do |f|
      yield(f)
    end
  end

  def install(srcPath, dest, name = nil)
    if (name == nil)
      name = File.basename(srcPath)
    end
    destPath = "#{@root}/#{CONTENTS_MACOS}/#{dest}/#{name}"

    FileUtils.cp(srcPath, destPath)
  end

  def installFolder(src, dest)
    FileUtils.cp_r(src, "#{@root}/#{CONTENTS_MACOS}/#{dest}")
  end

  def installAll(glob, dest)
    Dir.glob(glob).each do |item|
      next if item == '.' or item == '..'
      # do work on real items

      install(item, dest)
    end
  end

  def installAllWithPrefix(src, dest, glob)
    srcPath = Pathname.new(src)
    Dir.glob(src + "/" + glob).each do |item|
      next if item == '.' or item == '..'
      # do work on real items

      path = Pathname.new(item)
      relativeDir = File.dirname(path.relative_path_from(srcPath))

      destDir = dest + relativeDir
      FileUtils.mkdir_p("#{@root}/#{CONTENTS_MACOS}/#{destDir}")
      install(item, destDir)
    end
  end

  def setIcon(file)
    name = ""
    if (name == nil)
      name = File.basename(srcPath)
    end
    
    destPath = "#{@root}/#{CONTENTS_RESOURCES}/#{name}"

    FileUtils.cp(file, destPath)
  end

private
  def makePlist(keys)
    tags = keys.map { |k, v| makePlistEntry(k, v) }.join("\n\n") 

    return ENTRY + tags + EXIT
  end

  def makePlistEntry(k, v)
    return "  <key>#{k}</key>\n  <string>#{v}</string>"
  end
end

class DmgBuilder
  def build(source, dest)
    puts "size is #{`File.size(source)`}"
  end
end

BUILD_ID = "Desktop_Qt_5_4_0_clang_64bit"
BUILD_VARIANT = "qtc_#{BUILD_ID}"
DEBUGGER_BUILD_ROOT = "#{BUILD_ROOT}/build-debugger-#{BUILD_ID}-Debug/#{BUILD_VARIANT}-debug/"

def makeDebugifyBundle(version, options = {})
  llvmVariant = options[:llvm_variant]
  llvmVariant ||= "Debug+Asserts"
  llvmBuildRoot = "#{BUILD_ROOT}/llvm-build/#{llvmVariant}/"

  versionStr = "#{version[:major]}.#{version[:minor]}.#{version[:revision]}"

  bundleName = options[:path]
  bundleName ||= "#{THIS_DIR}/#{EXE_NAME}_#{versionStr}.app"

  BundleBuilder.cleanup(bundleName)
  bundle = BundleBuilder.new(bundleName, {
      :CFBundleName => "Debugify",
      :CFBundleVersion => versionStr,
      :CFBundleIdentifier => "com.debugify.#{version[:major]}",
      :CFBundlePackageType => "APPL",
      :CFBundleIconFile => "Debugify.icns",
      :CFBundleSignature => "dbfy",
      :CFBundleExecutable => "#{EXE_NAME}",
      :LSMinimumSystemVersion => "10.10",
      :NSHumanReadableCopyright => "Copyright © 2014-#{Date.today.strftime("%Y")} Debugify",
      :CFBundleShortVersionString => "Build #{versionStr} (#{version[:version_control]})"
    })

  bundle.mkdir("App")

  bundle.open(EXE_NAME) do |f|
    f.write('#!/bin/bash
echo "test"
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
$DIR/App/Debugify'
)
  end
  FileUtils.chmod("+x", bundle.path(EXE_NAME))

  bundle.open("App/initEnvironment.sh") do |f|
    f.write('#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
export PATH=$DIR/../ruby/bin/:$PATH
export LLDB_DEBUGSERVER_PATH="/Applications/Xcode.app/Contents/SharedFrameworks/LLDB.framework/Versions/A/Resources/debugserver"
export DYLD_LIBRARY_PATH=$DIR/../ruby/lib:$DIR:$DYLD_LIBRARY_PATH')
  end

  bundle.setIcon("build/Resources/Debugify.icns")
  bundle.install("#{SOURCE_ROOT}/App/Debugify", "App")
  bundle.installAll("#{SOURCE_ROOT}/App/*.rb", "App")
  bundle.installAllWithPrefix(SOURCE_ROOT, "", "plugins/DebugifyBindings/ruby/**/*.rb")
  bundle.installAllWithPrefix(SOURCE_ROOT, "", "plugins/UIBindings/ruby/**/*.rb")
  bundle.installAll("#{DEBUGGER_BUILD_ROOT}/**/*.dylib", "App")
  bundle.install("#{llvmBuildRoot}/lib/liblldb.dylib", "App")
  bundle.installFolder(File.expand_path("~/.rvm/rubies/ruby-2.2.0"), "ruby")

  renamableScriptBundles = [
    "DebugifyBindings",
    "UIBindings"
  ]

  renamableScriptBundles.each do |lib|
    bundle.mv("App/lib#{lib}.dylib", "plugins/#{lib}/ruby/#{lib}.bundle")
  end
end

def makeDebugifyDmg(version, options = {})
  versionStr = "#{version[:major]}.#{version[:minor]}.#{version[:revision]}"

  output = "#{THIS_DIR}/#{EXE_NAME}_#{versionStr}.dmg"
  path = "#{THIS_DIR}/tmp"

  if (File.exist?(path))
    FileUtils.rm_r(path)
  end
  
  location = "#{path}/#{EXE_NAME}.app"
  FileUtils.mkdir_p(path)

  bundleOptions = options.clone
  bundleOptions[:path] = location
  makeDebugifyBundle(version, bundleOptions)

  dmgSize = `du -sm #{path}`.split[0].to_i + 10
  puts `hdiutil create -volname #{EXE_NAME} -srcfolder #{path} -size #{dmgSize}m #{output} -ov`

  if (options[:keep_bundle] != true)
    FileUtils.rm_r(path) 
  end
end
