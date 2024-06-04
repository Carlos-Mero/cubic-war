UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S), Windows_NT)
	CC=cl.exe
else ifeq ($(UNAME_S), Darwin)
	CC=clang
else ifeq ($(UNAME_S), Linux)
	CC=gcc
endif

include_flags=-I./include -I./deps/Chipmunk2D/include/chipmunk -I./deps/uthash/include
cflags=-std=gnu2x -O3 $(include_flags) -fopenmp $(shell sdl2-config --cflags)
ldflags=-L./deps/Chipmunk2D/src -L./deps/SDL2/src $(shell sdl2-config --libs)
libs=./deps/Chipmunk2D/src/libchipmunk.a
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
target=cubicwar

all : $(target)
native: $(target)

$(target): $(OBJECTS)
	$(CC) $(cflags) $(ldflags) -o $@ $(OBJECTS) $(libs)

%.o: %.c
	$(CC) $(cflags) -c $< -o $@

clean: 
	rm -f $(OBJECTS) $(target)

deps:
	cd ./deps/Chipmunk2D && cmake . && make
	cd ./deps/SDL2 && mkdir -p build && cd build && ../configure && make && sudo make install

.PHONY: all deps clean native
