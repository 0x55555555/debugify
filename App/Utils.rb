
module App

  def self.shortPath(path)
    if (path.length > 50)
      path = "...#{path.chars.last(50).join}"
    end
    return path
  end

  class Logger

    attr_writer :console

    def log(s)
      @console.append(s)
    end
  end

end