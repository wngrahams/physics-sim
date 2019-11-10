#!/bin/bash

g++ -std=c++11 hellotri.cpp -framework Cocoa -framework OpenGL -framework IOKit -o out -I/usr/local/include -I/sw/include -I/usr/local/include -L/usr/local/lib -lGLEW -lglfw

