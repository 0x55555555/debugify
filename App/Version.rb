VERSION_GEN_PATH = File.dirname(__FILE__) + '/VersionGen.rb'

require VERSION_GEN_PATH

module App
  class VersionContainer
    def initialize(maj, min, rev, meta)
      @major = maj
      @minor = min
      @revision = rev
      @meta = meta
    end

    attr_accessor :major, :minor, :revision, :meta

    def to_s 
      return "#{major}.#{minor}.#{revision}-#{meta}"
    end

    def to_safe_s
      return to_s.gsub(/[-.]/, '_')
    end

    def increment
      return clone.increment!
    end

    def increment!
      @revision += 1

      return self
    end

    def tag
      return "release/#{to_s}"
    end
  end

  Version = VersionContainer.new(VersionData::Number[0], VersionData::Number[1], VersionData::Number[2], VersionData::Meta)
end