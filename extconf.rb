require 'mkmf'

# Workaround to get the -I directive for OpenZWave into CXXFLAGS
$INCFLAGS = "#{$INCFLAGS} " + `pkg-config --cflags-only-I libopenzwave`

pkg_config("libopenzwave") 
create_makefile("OpenZWave")
