require 'fileutils'
require 'date'

DEBUG = false
THIS_DIR = File.dirname(__FILE__)

SOURCE_ROOT = "#{THIS_DIR}/../"
EXE_NAME = 'Debugify'
BUILD_ROOT = "#{THIS_DIR}/../../"
QT_FRAMEWORK_LOCATION = "/Applications/Qt/5.4_deployment/"
RUBY_URL = "https://github.com/jorj1988/traveling-ruby/releases/download/2.2.0_dylib_osx/traveling-ruby-20141224-j-2.2.0-osx.zip"

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
    @sources = { }
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

  def sourcePath(dst)
    return @sources[dst]
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
    destDir = "#{@root}/#{CONTENTS_MACOS}#{dest}"
    destPath = "#{destDir}/#{name}"

    FileUtils.mkdir_p(destDir)
    FileUtils.cp(srcPath, destPath)
    puts destPath if @debugging
    @sources[destPath] = srcPath
  end

  def debug(on = true)
    @debugging = on
  end

  def installFolder(src, dest)
    destFolder = "#{@root}/#{CONTENTS_MACOS}#{dest}"
    FileUtils.cp_r(src, destFolder)

    root = Pathname.new(destFolder)
    Dir["#{destFolder}/**/*"].each do |dest|
      relative = Pathname.new(dest).relative_path_from(root)

      puts dest if @debugging
      @sources[dest] = "#{src}/#{relative}"
    end
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
      FileUtils.mkdir_p("#{@root}/#{CONTENTS_MACOS}#{destDir}")
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
DEBUGGER_BUILD_ROOT = DEBUG ? 
  "#{BUILD_ROOT}/build-debugger-#{BUILD_ID}-Debug/#{BUILD_VARIANT}-debug/" : 
  "#{BUILD_ROOT}/build-debugger-#{BUILD_ID}-Release/#{BUILD_VARIANT}-release/"

def ensureRubyExists()
  rubyDir = "#{SOURCE_ROOT}/build/Resources/"
  destname = "ruby"
  destfolder = "#{rubyDir}#{destname}"
  if (!Dir.exists?(destfolder))
    outName = File.basename(RUBY_URL)

    FileUtils.mkdir_p(destfolder)
    download = "#{rubyDir}#{destname}.tar.gz"

    `curl --fail -L -o #{download} '#{RUBY_URL}'`
    `unzip #{download} -d #{destfolder}`
    FileUtils.rm(download)
  end
end

def makeDebugifyBundle(version, options = {})

  ensureRubyExists()

  llvmVariant = options[:llvm_variant]
  llvmVariant ||= DEBUG ? "Debug+Asserts" : "Release+Asserts"
  llvmBuildRoot = "#{BUILD_ROOT}/llvm-build/#{llvmVariant}/"

  versionStr = version.to_safe_s

  bundleName = options[:path]
  bundleName ||= "#{THIS_DIR}/#{EXE_NAME}_#{versionStr}.app"

  BundleBuilder.cleanup(bundleName)
  bundle = BundleBuilder.new(bundleName, {
      :CFBundleName => "Debugify",
      :CFBundleVersion => versionStr,
      :CFBundleIdentifier => "com.debugify.#{version.major}",
      :CFBundlePackageType => "APPL",
      :CFBundleIconFile => "Debugify.icns",
      :CFBundleSignature => "dbfy",
      :CFBundleExecutable => "#{EXE_NAME}",
      :LSMinimumSystemVersion => "10.10",
      :NSHumanReadableCopyright => "Copyright © 2014-#{Date.today.strftime("%Y")} Debugify",
      :CFBundleShortVersionString => "Build #{version}"
    })

  bundle.mkdir("App")
  bundle.mkdir("Frameworks")

  bundle.open(EXE_NAME) do |f|
    f.write('#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
[[ -s "$HOME/.rvm/scripts/rvm" ]] && source "$HOME/.rvm/scripts/rvm"
$DIR/App/Debugify'
)
  end
  FileUtils.chmod("+x", bundle.path(EXE_NAME))

  bundle.open("App/initEnvironment.sh") do |f|
    f.write('#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
export LLDB_DEBUGSERVER_PATH="/Applications/Xcode.app/Contents/SharedFrameworks/LLDB.framework/Versions/A/Resources/debugserver"
export DYLD_LIBRARY_PATH=$DIR/../ruby/lib:$DIR:$DYLD_LIBRARY_PATH
export PATH=$DIR/../ruby/bin:$PATH
export DYLD_FRAMEWORK_PATH=$DIR/../Frameworks:$DYLD_FRAMEWORK_PATH
')
  end

  rubyLocation = "#{SOURCE_ROOT}/build/Resources/ruby"
  rubyTmp = "/tmp/ruby"
  FileUtils.rm_r(rubyTmp) if Dir.exist?(rubyTmp)
  FileUtils.cp_r(rubyLocation, rubyTmp)

  bundle.setIcon("#{SOURCE_ROOT}/build/Resources/Debugify.icns")
  bundle.install("#{SOURCE_ROOT}/App/Debugify", "App")
  bundle.installAll("#{SOURCE_ROOT}/App/**.rb", "App")
  bundle.installAll("#{SOURCE_ROOT}/App/Project/**.rb", "App/Project")
  bundle.installAllWithPrefix(SOURCE_ROOT, "", "plugins/DebugifyBindings/ruby/**/*.rb")
  bundle.installAllWithPrefix(SOURCE_ROOT, "", "plugins/UIBindings/ruby/**/*.rb")
  bundle.installAll("#{DEBUGGER_BUILD_ROOT}/**/*.dylib", "App")
  bundle.install("#{llvmBuildRoot}/lib/liblldb.dylib", "App")
  bundle.installFolder(rubyTmp, "ruby")
  bundle.install("#{QT_FRAMEWORK_LOCATION}/plugins/platforms/libqcocoa.dylib", "App/platforms")

  exeToRoot = "@executable_path/../../"

  qtLibs = [ "QtCore", "QtGui", "QtWidgets", "QtPrintSupport" ]
  qtLibs.each do |l|
    dest = "Frameworks/#{l}.framework"
    fwLocation = "#{QT_FRAMEWORK_LOCATION}lib/#{l}.framework"
    bundle.installFolder(fwLocation, dest)
  end

  exeFiles = [ 
    [ bundle.path("ruby/bin.real/ruby") ],
    Dir[bundle.path("**/*.dylib")],
    Dir[bundle.path("**/*.bundle")]
  ].flatten

  sources = { }
  exeFiles.each do |e|
    path = bundle.sourcePath(e)
    raise "Unknown source for #{e}" unless path != nil
    sources[e] = path
  end

  qtLibs.each do |l|
    path = bundle.path("Frameworks/#{l}.framework/Versions/5/#{l}")
    exeFiles.push(path)
    sources[path] = "#{QT_FRAMEWORK_LOCATION}lib/#{l}.framework/Versions/5/#{l}"
  end

  changes = Hash.new { |h, k| h[k] = [] }
  root = Pathname.new(bundle.path(""))
  exeFiles.each do |f|
    relPath = Pathname.new(f).relative_path_from(root)
      
    newPath = "#{exeToRoot}/#{relPath}"
    source = Pathname.new(sources[f]).cleanpath.to_s
    changes[relPath] << "install_name_tool -id #{newPath} #{f}"

    exeFiles.each do |file|
      if (file != f)
        relFilePath = Pathname.new(file).relative_path_from(root)
        changes[relFilePath] << "install_name_tool -change #{source} #{newPath} #{file}"
      end
    end

    changes[relPath] << "strip #{f} &> /dev/null"
  end

  forks = []
  changes.each do |relPath, procs|
    forks << fork do 
      puts "Patching #{relPath}"
      procs.each do |cmd|
        `#{cmd}`
      end
    end
  end
  puts "Waiting for patch to complete..."
  forks.each{ |p| Process.waitpid(p) }

  extraChanges = [
    {
      :executable => bundle.path("App/libLldbDriver.dylib"),
      :old_library => "@executable_path/../lib/liblldb.dylib",
      :new_library => "#{exeToRoot}App/liblldb.dylib",
    }
  ]

  extraChanges.each do |l|
    `install_name_tool -change #{l[:old_library]} #{l[:new_library]} #{l[:executable]}`
  end

  FileUtils.rm_r(rubyTmp)

  renamableScriptBundles = [
    "DebugifyBindings",
    "UIBindings"
  ]

  renamableScriptBundles.each do |lib|
    relativeBundle = "plugins/#{lib}/ruby/#{lib}.bundle"
    relativeLib = "App/lib#{lib}.dylib"
    oldLoc = bundle.path(relativeLib)
    newLoc = bundle.path(relativeBundle)

    FileUtils.ln_s("../../../" + relativeLib, newLoc)
  end
end

def makeDebugifyDmg(version, options = {})
  output = "#{THIS_DIR}/#{EXE_NAME}_#{version.to_safe_s}.dmg"
  path = "#{THIS_DIR}/tmp"

  if (File.exist?(path))
    FileUtils.rm_r(path)
  end
  
  location = "#{path}/#{EXE_NAME}.app"
  FileUtils.mkdir_p(path)

  bundleOptions = options.clone
  bundleOptions[:path] = location
  makeDebugifyBundle(version, bundleOptions)

  puts "Creating app dmg #{output}"
  dmgSize = `du -sm #{path}`.split[0].to_i + 10
  puts `hdiutil create -volname #{EXE_NAME} -srcfolder #{path} -size #{dmgSize}m #{output} -ov`

  if (options[:keep_bundle] != true)
    FileUtils.rm_r(path) 
  end
end
