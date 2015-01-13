require 'fileutils'
require 'erb'
require 'Pathname'

SOURCE_ROOT = "../"
BUNDLE = 'Debugify.app'
CONTENTS = "#{BUNDLE}/Contents/"
CONTENTS_MACOS = "#{CONTENTS}MacOS/"
CONTENTS_MACOS_APP = "#{CONTENTS_MACOS}App/"
INFO_PLIST = "#{CONTENTS}Info.plist"
EXE_NAME = 'Debugify'
ROOT_EXE = "#{CONTENTS_MACOS}/#{EXE_NAME}" 
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
    :CFBundleExecutable => EXE_NAME,
    :LSMinimumSystemVersion => "10.10",
    :NSHumanReadableCopyright => "Copyright © 20014-2015 Debugify",
    :CFBundleShortVersionString => "Build 1"
  }))
end

File.open(ROOT_EXE, 'w') do |f|
  f.write("#!/bin/bash\n$( cd \"$( dirname \"${BASH_SOURCE[0]}\" )\" && pwd )/App/Debugify")
end
FileUtils.chmod("+x", ROOT_EXE)

install("#{SOURCE_ROOT}/App/initEnvironment.sh", CONTENTS_MACOS_APP)
install("#{SOURCE_ROOT}/App/Debugify", CONTENTS_MACOS_APP)
installAll("#{SOURCE_ROOT}/App/*.rb", CONTENTS_MACOS_APP)