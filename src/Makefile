CC  = g++
CXX = g++ -O0 

INCLUDES =

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall -std=c++11 $(INCLUDES)

LDFLAGS = -g
LDLIBS  =

executables = sim test-mass
objects = mass.o sim.o test-mass.o

.PHONY: default
default: $(executables)

$(executables): mass.o

$(objects): mass.hpp sim.hpp

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default

