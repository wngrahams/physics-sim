CC  = clang -O0
CXX = clang++ -O0 

INCLUDES = -I/usr/local/include

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall -std=c++11 $(INCLUDES)

LDFLAGS = -g -lstdc++ -framework Cocoa -framework OpenGL -framework IOKit -L/usr/local/lib 
LDLIBS  = -lGLEW -lglfw

executables = graphics
objects = gl-utils.o graphics.o maths_funcs.o obj_parser.o

.PHONY: default
default: $(executables)

$(executables): gl-utils.o maths_funcs.o obj_parser.o

$(objects): gl-utils.h maths_funcs.hpp obj_parser.h

.PHONY: clean
clean:
	rm -f *~ a.out gl.log core $(objects) $(executables)

.PHONY: all
all: clean default

