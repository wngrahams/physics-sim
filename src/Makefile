CC  = g++
CXX = g++ -O0 

INCLUDES =

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall -std=c++11 $(INCLUDES)

LDFLAGS = -g
LDLIBS  =

executables = sim test-mass test-spring
objects = mass.o sim.o test-mass.o test-spring.o

.PHONY: default
default: $(executables)

$(executables): mass.o spring.o

$(objects): mass.hpp spring.hpp sim.hpp

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default

