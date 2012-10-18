#!/usr/bin/env ruby

dependencies = 
  "aclocal",
  "autoconf",
  "automake",
  "autoreconf"

commands =
  "aclocal",
  "autoconf",
  "automake --add-missing",
  "autoreconf",
  "automake",
  "./configure"

missing_deps = Array.new

dependencies.each do |dependency|
  puts "build-env: checking #{dependency}"
  if `which #{dependency}`.empty?
    missing_deps.push dependency
  end
end

if not missing_deps.empty?
  puts "build-env failed..."

  missing_deps.each do |missing|
    puts "Install #{missing}"
  end
  exit 1
end

commands.each do |command|
  puts "build-env: #{command}"
  `#{command}`
end

puts "Your environment setup is complete. Run `make` to build hkl project."

