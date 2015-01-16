require 'fileutils'
require 'Pathname'

THIS_DIR = File.dirname(__FILE__)

SOURCE_ROOT = "#{THIS_DIR}/../"
BUNDLE_NAME = 'Debugify.app'
BUNDLE = "#{THIS_DIR}/#{BUNDLE_NAME}"
CONTENTS = "#{BUNDLE}/Contents/"
CONTENTS_MACOS = "#{CONTENTS}MacOS/"
CONTENTS_MACOS_APP = "#{CONTENTS_MACOS}App/"
INFO_PLIST = "#{CONTENTS}Info.plist"
EXE_NAME = 'Debugify'
ROOT_EXE = "#{CONTENTS_MACOS}/#{EXE_NAME}" 

BUILD_ROOT = "#{THIS_DIR}/../../"
LLVM_VARIANT = "Debug+Asserts"
LLVM_BUILD_ROOT= "#{BUILD_ROOT}/llvm-build/#{LLVM_VARIANT}/"
BUILD_ID = "Desktop_Qt_5_4_0_clang_64bit"
BUILD_VARIANT = "qtc_#{BUILD_ID}"
DEBUGGER_BUILD_ROOT = "#{BUILD_ROOT}/build-debugger-#{BUILD_ID}-Debug/#{BUILD_VARIANT}-debug/"

ENTRY = '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>

'
EXIT = '

</dict>
</plist>'

def cleanup(file)
  if (File.directory?(file))
    FileUtils.rm_rf(file)
  end
end

def makePlistEntry(k, v)
  return "  <key>#{k}</key>\n  <string>#{v}</string>"
end

def makePlist(keys)
  tags = keys.map { |k, v| makePlistEntry(k, v) }.join("\n\n") 

  return ENTRY + tags + EXIT
end

def install(srcPath, dest, name = nil)
  if (name == nil)
    name = File.basename(srcPath)
  end
  destPath = "#{dest}/#{name}"

  FileUtils.cp(srcPath, destPath)
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
    FileUtils.mkdir_p(destDir)
    install(item, destDir)
  end
end

cleanup(BUNDLE)
FileUtils.mkdir_p(BUNDLE)
FileUtils.mkdir_p(CONTENTS_MACOS)
FileUtils.mkdir_p(CONTENTS_MACOS_APP)

File.open(INFO_PLIST, 'w') do |f|
  f.write(makePlist({
    :CFBundleName => "Debugify",
    :CFBundleVersion => 1,
    :CFBundleIdentifier => "com.debugify.1",
    :CFBundlePackageType => "APPL",
    :CFBundleIconFile => "Debugify",
    :CFBundleSignature => "dbfy",
    :CFBundleExecutable => "App/#{EXE_NAME}",
    :LSMinimumSystemVersion => "10.10",
    :NSHumanReadableCopyright => "Copyright © 20014-2015 Debugify",
    :CFBundleShortVersionString => "Build 1"
  }))
end

File.open("#{CONTENTS_MACOS_APP}/initEnvironment.sh", 'w') do |f|
  f.write('DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
export LLDB_DEBUGSERVER_PATH=$DIR/debugserver
export DYLD_LIBRARY_PATH=$DIR:$DYLD_LIBRARY_PATH')
end

install("#{SOURCE_ROOT}/App/Debugify", CONTENTS_MACOS_APP)
installAll("#{SOURCE_ROOT}/App/*.rb", CONTENTS_MACOS_APP)
installAllWithPrefix(SOURCE_ROOT, CONTENTS_MACOS, "plugins/DebugifyBindings/ruby/**/*.rb")
installAllWithPrefix(SOURCE_ROOT, CONTENTS_MACOS, "plugins/UIBindings/ruby/**/*.rb")
installAll("#{DEBUGGER_BUILD_ROOT}/**/*.dylib", CONTENTS_MACOS_APP)
install("#{LLVM_BUILD_ROOT}/lib/liblldb.dylib", CONTENTS_MACOS_APP)
install("#{LLVM_BUILD_ROOT}/bin/debugserver", CONTENTS_MACOS_APP)

RenamableScriptBundles = [
  "DebugifyBindings",
  "UIBindings"
]

RenamableScriptBundles.each do |lib|
  FileUtils.mv("#{CONTENTS_MACOS_APP}/lib#{lib}.dylib", "#{CONTENTS_MACOS}/plugins/#{lib}/ruby/#{lib}.bundle")
end
