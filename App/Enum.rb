
class Enum
  def initialize(values)
    @values = values
    @inverseValues = @values.invert
  end

  def [](y)
    if (y.class == Symbol)
      @values[y]
    else
      @inverseValues[y]
    end
  end
end