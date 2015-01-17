debug = ENV['DEBUGIFY_DEBUG'] == '1'

def rescueApp(debug)
  if (debug)
    Pry.rescue do 
      yield()
    end
  else
    yield()
  end
end

require 'pry-rescue' unless !debug
require 'pry' unless !debug
require_relative 'NotifierExtensions'
require_relative 'Enum'

application = nil

rescueApp(debug) do

  require_relative '../plugins/DebugifyBindings/ruby/Debugify'
  require_relative '../plugins/UIBindings/ruby/UI'
  require_relative 'Application'
  
  application = App::Application.new(debug)

  application.execute()
end