require_relative 'FileLocation'
require_relative 'DefaultHandler'

class Project
  def initialize()
    @handler_stack = [ ]
    @handlers = { }

    @default_handler = DefaultHandler.new
    install_handler(:default, @default_handler)
  end

  attr_reader :default_handler

  def install_handler(id, handler)
    raise "Handler #{id} already installed" if @handlers.include?(id)

    @handler_stack << handler
    @handlers[id] = @handler_stack.length - 1

    if (handler != @default_handler)
      handler.deserialise(@default_handler)
    end
  end
    
  def install_location(id, path)
    return @default_handler.install_location(id, path)
  end

  def location(id)
    return @default_handler.location(id)
  end

  def write()
    @handler_stack.each do |h|
      if (h != @default_handler)
        h.serialise(@default_handler)
      end
    end

    return @default_handler.write()
  end

  def value(key, start_location = nil)
    @handler_stack.reverse_each do |h|
      if (h.owns_value(key))
        return h.value(key, start_location)
      end
    end
  end

  def has_value(key, start_location = nil)
    @handler_stack.reverse_each do |h|
      if (h.owns_value(key))
        return h.has_value(key, start_location)
      end
    end
  end

  def set_value(key, val, location = nil)
    @handler_stack.reverse_each do |h|
      if (h.owns_value(key))
        return h.set_value(key, val, location)
      end
    end
  end

  def method_missing(sym, *args, &block)
    val = sym.to_s
    sym = sym
    if (val.end_with? '=')
      sym = val[0..-2].to_sym
      return set_value(sym, *args)
    end
    
    return value(sym)
  end

  def respond_to?(sym, include_private = false)
    val = sym.to_s
    sym = sym
    if (val.end_with? '=')
      sym = val[0..-2].to_sym
    end

    return has_value(sym)
  end
end