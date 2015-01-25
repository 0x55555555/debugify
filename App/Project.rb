require_relative 'Project/Project'

module App

class DebuggerProject
  ConfigName = "debugify"

  def initialize(appPath)
    @handlers = { }
    @project = Project.new()

    reset(appPath)
  end

  def install_handler(id, cls)
    raise "Handler already registered" if @handlers.include?(id)
    @handlers[id] = cls
    @project.install_handler(id, cls)
  end

  def close()
    @project.write()
    @project = nil
  end

  def reset(appPath)
    close()
    @project = Project.new()

    @project.install_location(:user, "#{ENV['HOME']}/.#{ConfigName}/settings.json")

    if (appPath != nil)
      ext = File.extname(appPath)
      appName = File.basename(appPath, ext)
      appDir = File.dirname(appPath)

      @project.install_location(:exe, "#{appDir}/#{appName}_#{ConfigName}.json")
    end

    @handlers.each do |id, cls|
      @project.install_handler(id, cls)
    end
  end

  def value(key, default = nil)
    if (@project.has_value(key))
      return @project.value(key)
    end

    return default
  end

  def set_value(key, val, location)
    @project.set_value(key, val, location)
  end
end

end 