require 'json'

class Project

  class FileLocation
    def initialize(path)
      @path = path
      @contents = { }

      if (File.exist?(path))
        data = File.read(path)
        @contents = parse_contents(JSON.parse(data))
      end
    end

    def write()
      data = JSON.pretty_generate(@contents)

      FileUtils.mkdir_p(File.dirname(@path))
      File.open(@path, 'w') do |f|
        f.write(data)
      end
    end

    def value(key)
      return @contents[key]
    end

    def set_value(key, val)
      @contents[key] = val
    end

    def has_value(key)
      return @contents.include?(key)
    end

    def parse_contents(cnt)
      out = { }

      cnt.each do |k, v|
        out[k.to_sym] = v
      end

      return out
    end
  end

end