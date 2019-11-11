CC  = clang -O0
CXX = g++ -O0 

INCLUDES = -I/usr/local/include

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall -std=c++11 $(INCLUDES)

LDFLAGS = -g -framework Cocoa -framework OpenGL -framework IOKit -L/usr/local/lib 
LDLIBS  = -lGLEW -lglfw

executables = test3
objects = gl_utils.o test3.o

.PHONY: default
default: $(executables)

$(executables): gl_utils.o

$(objects): gl_utils.h

.PHONY: clean
clean:
	rm -f *~ a.out gl.log core $(objects) $(executables)

.PHONY: all
all: clean default

