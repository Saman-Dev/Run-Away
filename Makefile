# Makefile for for MacOS, windows and  
SRCDIR=./src
CC=gcc 


CURRENT_OS := windows
ifneq ($(OS),Windows_NT)
    UNAME := $(shell uname)
    ifeq ($(UNAME),Linux)
        CURRENT_OS := linux
    endif
    ifeq ($(UNAME),Darwin)
        CURRENT_OS := mac
    endif
endif


LDFLAGS := -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
LDFLAGS_WINDOWS := -lmingw32 -mwindows
LDFLAGS_MAC := -L /opt/homebrew/lib
CFLAGS_WINDOWS := C:\msys64\mingw64\include
CFLAGS_MAC := /opt/homebrew/include/SDL2


ifeq ($(CURRENT_OS),windows)
    LDFLAGS := $(LDFLAGS_WINDOWS) $(LDFLAGS)
	CFLAGS := -g -I$(CFLAGS_WINDOWS) -c 

endif

ifeq ($(CURRENT_OS),mac)
    LDFLAGS := $(LDFLAGS_MAC) $(LDFLAGS)
	CFLAGS := -g -I$(CFLAGS_MAC) -c 
	CFLAGS  := $(CFLAGS_MAC) $(CFLAGS)
endif


theGame: main.o player.o map.o
	gcc -o game main.o player.o map.o -L/opt/homebrew/lib/ -lSDL2 -lSDL2_image
main.o: src/main.c
	gcc -c src/main.c -I/opt/homebrew/include/SDL2
player.o: src/player/player.c
	gcc -c src/player/player.c
map.o: src/map/map.c
	gcc -c src/map/map.c

clean:
	rm -f game
	rm *.o
	rm *.out














