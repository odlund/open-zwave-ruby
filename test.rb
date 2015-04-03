require './OpenZWave'

opts = OpenZWave::Options.new("../open-zwave/config", "", "");

puts opts.are_locked
puts opts.add("TestInt", 42)
puts opts.add("TestBool", false)
puts opts.add("TestString", "Foo")
puts opts.lock
puts opts.add("TestString2", "Bar")
puts "TestInt: " + opts.get("TestInt").to_s
puts "TestBool: " + opts.get("TestBool").to_s
puts "TestString: " + opts.get("TestString").to_s
puts "TestString: " + opts.get("TestString2").to_s
puts opts.are_locked