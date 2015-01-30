require_relative 'build/Bundle.rb'
require_relative 'App/Version.rb'
require 'rspec/core/rake_task'

# Build bundle
task :bundle do |task|
  puts "Building a bundle for '#{App::Version}'"

  makeDebugifyBundle(App::Version)
end

# Build dmg
task :dmg do |task|
  puts "Building a dmg for '#{App::Version}'"

  makeDebugifyDmg(App::Version)
end

task :release, [:version_number] do |task, args|
  ver = args[:version_number]

  if (ver == nil)
    newVer = App::Version.increment
    newVer.meta = Time.now().strftime("%Y%m%d")
    ver = newVer.to_s
  end

  ver, meta = ver.split('-')
  ver = ver.split('.').map{ |n| n.to_i }[0..2]

  File.open(VERSION_GEN_PATH, 'w') do |w|
    w.write("module App\n")
    w.write("module VersionData\n")
    w.write("Number = #{ver}\n")
    w.write("Meta = \"#{meta}\"\n")
    w.write("end\n")
    w.write("end\n")
  end

  original_verbosity = $VERBOSE
  $VERBOSE = nil
  load(VERSION_GEN_PATH)
  load(File.dirname(__FILE__) + '/App/Version.rb')
  $VERBOSE = original_verbosity

  `git tag #{App::Version.tag}`

  Rake::Task[:dmg].invoke
end

RSpec::Core::RakeTask.new(:spec) do |task|
  task.pattern = 'spec/**{,/*/**}/*.spec.rb'
  task.rspec_opts = ['--color', '--format', 'progress']
end

task :version do |task|
  puts App::Version.to_safe_s
  puts App::Version
end


task :default => :dmg
