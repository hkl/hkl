desc "Build Env"
task :build_env do
  # Pull down all submodules and then build the autotools environment
  system('git submodule update --init --recursive')
  system('build/auto-build-env')
end

task :default => :build_env
