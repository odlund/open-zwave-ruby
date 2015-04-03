require 'mkmf'

$INCFLAGS = "#{$INCFLAGS} " + `pkg-config --cflags-only-I libopenzwave`

puts $INCFLAGS

pkg_config("libopenzwave") 
create_makefile("OpenZWave")
