def os
  @os ||= (
    host_os = RbConfig::CONFIG['host_os']
    case host_os
    when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
      :windows
    when /darwin|mac os/
      :macosx
    when /linux/
      :linux
    when /solaris|bsd/
      :unix
    else
      raise Error::WebDriverError, "unknown os: #{host_os.inspect}"
    end
  )
end

# preamble helps us set up libclang, and ffi-clang. 
def prefix 
  case os
  when :macosx
    return 'lib'
  when :linux
    return 'lib'
  when :windows
    return ''
  end
end

def sourceExtension
  case os
  when :macosx
    return 'dylib'
  when :linux
    return 'so'
  when :windows
    return 'dll'
  end
end

def requiredExtension
  case os
  when :macosx
    return 'bundle'
  when :linux
    return 'so'
  when :windows
    return 'dll'
  end
end

buildDir = File.dirname(__FILE__) + '/../../../../../build-debugger-Desktop-Debug/qtc_Desktop-debug/DebugifyBindings.qtc_Desktop/'
libraryName = 'DebugifyBindings'
sourceName = "#{prefix}DebugifyBindings.#{sourceExtension}"
destinationName = "DebugifyBindings.#{requiredExtension}"

require 'fileutils'

FileUtils.ln_s(buildDir + sourceName, buildDir + destinationName, { :force => true })
require buildDir + libraryName
