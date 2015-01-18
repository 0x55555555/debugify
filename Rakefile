require_relative 'build/Bundle.rb'

version = {
  :major => 0,
  :minor => 0,
  :revision => 1,
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

task :default => :dmg
