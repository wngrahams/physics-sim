CC  = clang -O0
CXX = clang++ -O0 

INCLUDES = -I/usr/local/include

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall -std=c++11 $(INCLUDES)

LDFLAGS = -g -lstdc++ -framework Cocoa -framework OpenGL -framework IOKit -L/usr/local/lib 
LDLIBS  = -lGLEW -lglfw

executables = graphics
objects = gl-utils.o graphics.o maths_funcs.o obj_parser.o camera.o vboindexer.o objloader.p

.PHONY: default
default: $(executables)

$(executables): gl-utils.o maths_funcs.o obj_parser.o camera.o vboindexer.o objloader.o

$(objects): gl-utils.h maths_funcs.hpp obj_parser.hpp camera.hpp vboindexer.hpp objloader.hpp

.PHONY: clean
clean:
	rm -f *~ a.out gl.log core $(objects) $(executables)

.PHONY: all
all: clean default

