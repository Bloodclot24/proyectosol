# Makefile de ejemplo para C++
#
# Creado: jue abr 15 15:34:19 ART 2004
#
# Ultima modificacion: mar jun 30 09:59:42 ART 2009
#
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
# 
# Modificado para el trabajo practico Sun Torrent

# CONFIGURACION
################

# Nombre del ejecutable.
target_ppal = Sun

ifndef PREFIX
PREFIX=~/Sun/
endif

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
pthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
gtk = si

# Si es un programa gtkmm, descomentÃ¡ (quitale el '#' a) la siguiente lÃ­nea.
gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = si

# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
#CFLAGS = -Wall -pedantic -pedantic-errors

CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
#CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-2.4)
LDFLAGS += $(shell pkg-config --libs gtkmm-2.4)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

extension=cpp

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
fuentes = main.cpp Vista/GUI/*.cpp Vista/Shell/shell.cpp Controlador/controlador.cpp Controlador/ControladorShell/controladorShell.cpp Controlador/ControladorGUI/controladorGUI.cpp Modelo/Client/client.cpp Modelo/Client/torrent.cpp Modelo/Client/mensaje.cpp Modelo/Client/thread*.cpp Modelo/Client/socket.cpp Modelo/ParserBencode/parserBencode.cpp Modelo/HTTP/HttpRe*.cpp Modelo/Client/torrentFile.cpp Modelo/SHA1/sha1.cpp Modelo/Client/peer.cpp Modelo/ProtocoloBitTorrent/protocoloBitTorrent.cpp Modelo/Client/bitField.cpp  Modelo/SHA1/bitStream.cpp Modelo/FileManager/fileManager.cpp Vista/ventanaCargando.cpp

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: principal

principal: $(fuentes)
	$(LD) $(LDFLAGS) $(CFLAGS) $(fuentes) $(LOADLIBS) $(LDLIBS) -o $(target_ppal)

install: $(all)
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/Vista/GUI/
	cp -r Vista/GUI/images $(PREFIX)/Vista/GUI/
	cp Vista/GUI/*.glade $(PREFIX)/Vista/GUI/
	cp $(target_ppal) $(PREFIX)/

uninstall:
	@if [ -r $(PREFIX)/$(target_ppal) ]; \
	then \
		rm $(PREFIX)/$(target_ppal) \
		rm -rf $(PREFIX)/; \
	fi

clean:
	$(RM) $(target_ppal)

