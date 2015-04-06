require 'mkmf'

# Workaround to get the -I directive for OpenZWave into CXXFLAGS

pkg_config("libopenzwave") 

$INCFLAGS = "#{$INCFLAGS} " + `pkg-config --cflags-only-I libopenzwave`

create_makefile("OpenZWave")
