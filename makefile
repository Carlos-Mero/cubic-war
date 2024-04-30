CC=clang
include_flags=-I./include -I./deps/Chipmunk2D/include/chipmunk -I./deps/uthash/include
cflags=-std=c23 -O3 $(include_flags) -fopenmp $(shell sdl2-config --cflags)
ldflags=-L./deps/Chipmunk2D/src -L./deps/SDL2/src $(shell sdl2-config --libs)
libs=./deps/Chipmunk2D/src/libchipmunk.a
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
target=cubicwar

all : $(target)

$(target): $(OBJECTS)
	$(CC) $(cflags) $(ldflags) -o $@ $(OBJECTS) $(libs)

%.o: %.c
	$(CC) $(cflags) -c $< -o $@

clean: 
	rm -f $(OBJECTS) $(target)

deps:
	cd ./deps/Chipmunk2D && cmake . && make
	cd ./deps/SDL2 && mkdir -p build && cd build && ../configure && make && sudo make install

.PHONY: all deps clean
