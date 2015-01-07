
module App

  class RubyNotifier
    def initialize()
      @calls = []
    end

    def call(*vals)
      @calls.each { |c| c.call(*vals) }
    end

    def listen(&blk)
      @calls << blk
    end
  end

end