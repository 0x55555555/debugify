$:.unshift File.dirname(__FILE__) + "/bondage/parser/ffi-clang/lib"
ENV['LLVM_CONFIG'] = "../../../../llvm-build/Release+Asserts/bin/llvm-config"
PLATFORM_INCLUDES = [ "/usr/include/c++/4.2.1/" ]

require_relative 'bondage/parser/Library.rb'
require_relative 'bondage/exposer/ParsedLibrary.rb'
require_relative 'bondage/exposer/ClassExposer.rb'
require_relative 'bondage/generators/CPP/LibraryGenerator.rb'
require_relative 'bondage/generators/Ruby/LibraryGenerator.rb'
require 'fileutils'

library = Parser::Library.new("Debugify", "../sample/", "EXPORT_DEBUGIFY")
library.addFiles('.', '*.h', true)

FileUtils::mkdir_p(library.autogenPath(:cpp))

debugging = false

parsedLibrary = ParsedLibrary.parse(library, PLATFORM_INCLUDES, [], debugging)
exposer = ClassExposer.new(parsedLibrary, debugging)

class HeaderHelper
  def filePrefix(lang)
    cmt = "//"
    if (lang == :ruby)
      cmt = "#"
    end
    return "#{cmt} Autogenerated - do not change."
  end

  def requiredIncludes(lib)
    return [  ]
  end

  def fileSuffix(lang)
    return ""
  end
end

class PathResolver
  def pathFor(cls)
    return "#{cls.name}"
  end

  def coreClassPath()
    return "Object"
  end

  def coreRequires()
    return [ "require_relative '/Users/jorj/Shared/llvm/ruby-debugger/plugins/DebugifyBindings/sample/DebugifyBindings'" ]
  end
end

helper = HeaderHelper.new

cpp = CPP::LibraryGenerator.new(helper)

cpp.generate(parsedLibrary, exposer)

expectedHeader = cpp.headerPath(library)
expectedSource = cpp.sourcePath(library)

FileUtils.mkdir_p(library.autogenPath(:cpp))
FileUtils.mkdir_p(library.autogenPath(:ruby))
File.open(expectedHeader, 'w') do |file|
  file.write(cpp.header)
end
File.open(expectedSource, 'w') do |file|
  file.write(cpp.source)
end

ruby = Ruby::LibraryGenerator.new({ }, { }, PathResolver.new, helper)
ruby.generate(parsedLibrary, exposer)
ruby.write(library.autogenPath(:ruby))