
class Project
  class DefaultHandler
    def initialize()
      @locations = { }
      @location_stack = []
    end

    def install_location(id, path)
      loc = FileLocation.new(path)
      @location_stack << loc
      @locations[id] = @location_stack.length - 1
      return loc
    end

    def has_location(id)
      return @locations.include?(id)
    end

    def location(id)
      idx = @locations[id]
      raise "Location #{id} not installed" unless idx
      return @location_stack[idx]
    end

    def write()
      @location_stack.each do |l|
        l.write()
      end
    end

    def value(key, start_location = nil)
      loc = first_location_with(key, start_location)
      if (loc == nil)
        return nil
      end

      loc.value(key)
    end

    def owns_value(key)
      return true
    end

    def has_value(key, start_location = nil)
      return first_location_with(key, start_location) != nil
    end

    def set_value(key, val, location = nil)
      if (location != nil)
        loc = location(location)
        raise "Invalid location #{location}" unless loc
        return loc.set_value(key, val)
      end

      loc = first_location_with(key, location)
      if (loc == nil)
        loc = default_location
      end
      return loc.set_value(key, val)
    end

    def first_location_with(key, start_location = nil)
      start = @locations[start_location]
      if (start == nil)
        start = @location_stack.length - 1
      end

      raise "No locations installed" if (@location_stack.length == 0)

      start.downto(0) do |i|
        loc = @location_stack[i]
        if (loc.has_value(key))
          return loc
        end
      end

      return nil
    end

    def default_location()
      raise "No locations installed" unless @location_stack.length
      return @location_stack[-1]
    end
  end
end