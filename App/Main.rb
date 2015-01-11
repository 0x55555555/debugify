require 'pry-rescue'
require 'pry'
require_relative 'NotifierExtensions'
require_relative 'Enum'

application = nil

Pry.rescue do

  require_relative '../plugins/DebugifyBindings/ruby/Debugify'
  require_relative '../plugins/UIBindings/ruby/UI'
  require_relative 'Application'
  
  application = App::Application.new()

  application.execute()
end