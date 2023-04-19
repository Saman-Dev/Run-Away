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


LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_mixer
LDFLAGS_WINDOWS := -lmingw32 -mwindows
LDFLAGS_MAC := -L /opt/homebrew/lib/
CFLAGS_WINDOWS := C:\msys64\mingw64\include
CFLAGS_MAC := /opt/homebrew/include/


ifeq ($(CURRENT_OS),windows)
    LDFLAGS := $(LDFLAGS)
	CFLAGS := -I$(CFLAGS_WINDOWS) -c 

endif

ifeq ($(CURRENT_OS),mac)
    LDFLAGS := $(LDFLAGS_MAC) $(LDFLAGS)
	CFLAGS := -I$(CFLAGS_MAC) -c 
endif


theGame: main.o player.o map.o
	$(CC) -o game main.o player.o map.o $(LDFLAGS)
	./game.exe
	./game.out
main.o: src/main.c
	@echo "Building Game" 
	$(CC) src/main.c $(CFLAGS)
player.o: src/player/player.c
	$(CC) -c src/player/player.c
map.o: src/map/map.c
	$(CC) -c src/map/map.c

clean:
	rm -f game
	rm *.o
	rm *.out