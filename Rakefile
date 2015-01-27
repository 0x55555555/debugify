require_relative 'build/Bundle.rb'
require_relative 'App/Version.rb'
require 'rspec/core/rake_task'

version = {
  :major => App::Version::Major,
  :minor => App::Version::Minor,
  :revision => App::Version::Revision,
  :version_control => `git describe --tags`.chomp,
}

# Build bundle
task :bundle do |task|
  puts "Building a bundle for '#{version[:version_control]}'"

  makeDebugifyBundle(version)
end

# Build dmg
task :dmg do |task|
  puts "Building a dmg for '#{version[:version_control]}'"

  makeDebugifyDmg(version)
end

RSpec::Core::RakeTask.new(:spec) do |task|
  task.pattern = 'spec/**{,/*/**}/*.spec.rb'
  task.rspec_opts = ['--color', '--format', 'progress']
end


task :default => :dmg
