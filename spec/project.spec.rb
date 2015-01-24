require_relative 'helper'
require 'fileutils'

TMP = "./tmp/"
TMP_LOCATION1 = "#{TMP}location1.json"
TMP_LOCATION2 = "#{TMP}location2.json"

describe Project do
  #it_behaves_like "Identifiable"

  it 'can store settings' do
    pro = described_class.new()

    FileUtils.rm_r(TMP) if Dir.exists?(TMP)

    loc = pro.install_location(:location, TMP_LOCATION1)
    expect(pro.default_handler.default_location()).to eq(loc)
    expect(pro.location(:location)).to eq(loc)

    expect(pro.has_value(:pork)).to eq(false)
    expect(pro.pork).to eq(nil)
    expect(pro.value(:pork)).to eq(nil)
    pro.set_value(:pork, 5)
    expect(pro.default_handler.first_location_with(:pork)).to eq(loc)
    expect(pro.value(:pork)).to eq(5)
    pro.pork = "test!"
    expect(pro.pork).to eq("test!")

    pro.write()

    expect(File.read(TMP_LOCATION1)).to eq('{"pork":"test!"}')
  end

  it 'can store settings in multiple files' do
    pro = described_class.new()

    FileUtils.rm_r(TMP) if Dir.exists?(TMP)

    loc1 = pro.install_location(:locationA, TMP_LOCATION1)
    loc2 = pro.install_location(:locationB, TMP_LOCATION2)

    expect(pro.default_handler.default_location()).to eq(loc2)
    expect(pro.location(:locationA)).to eq(loc1)
    expect(pro.location(:locationB)).to eq(loc2)

    pro.test = 5
    pro.test2 = "thing"

    pro.set_value(:test_locB, "abc", :locationB)
    pro.set_value(:test_locA, "abcd", :locationA)

    pro.write()

    expect(File.read(TMP_LOCATION1)).to eq('{"test_locA":"abcd"}')
    expect(File.read(TMP_LOCATION2)).to eq('{"test":5,"test2":"thing","test_locB":"abc"}')
  end

  it 'can restore settings from files' do

    class ProjectSetup
      def initialize()
        @project = Project.new()

        loc1 = @project.install_location(:locationA, TMP_LOCATION1)
      end

      attr_reader :project
    end
    
    FileUtils.rm_r(TMP) if Dir.exists?(TMP)

    # First save some project settings
    pro = ProjectSetup.new

    pro.project.abc = "TEST TEST"
    pro.project.write

    # Now try to load them
    pro = ProjectSetup.new
    expect(pro.project.abc).to eq("TEST TEST")

  end

  it 'can hold custom descriptors' do

    class SampleHandler
      def initialize()
        @val = "pork pork"
      end

      def owns_value(key)
        return key == :abc
      end

      def value(key, location)
        return @val
      end

      def set_value(key, val, location)
        @val = val
      end

      def serialise(handler)
        handler.set_value(:abc, @val)
      end

      def deserialise(handler)
        if (handler.has_value(:abc))
          @val = handler.value(:abc)
        end
      end
    end

    class ProjectSetup
      def initialize()
        @project = Project.new()
        @handler = SampleHandler.new()

        loc1 = @project.install_location(:locationA, TMP_LOCATION1)
        @project.install_handler(:abc, @handler)
      end

      attr_reader :project, :handler
    end
    
    FileUtils.rm_r(TMP) if Dir.exists?(TMP)
    pro = ProjectSetup.new

    expect(pro.project.value(:abc)).to eq("pork pork")
    pro.project.set_value(:abc, "test")
    expect(pro.project.value(:abc)).to eq("test")


    pro.project.write()

    pro = ProjectSetup.new
    expect(pro.project.value(:abc)).to eq("test")
  end
end
