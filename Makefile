# Makefile for for MacOS, windows and  
SRCDIR=./src
SRC=src/Entities
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


LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf  `pkg-config --cflags --libs sdl2` -lSDL2main -mconsole
LDFLAGS_WINDOWS := -lmingw32 -mwindows
LDFLAGS_MAC := -L /opt/homebrew/lib/
CFLAGS_WINDOWS := C:\msys64\mingw64\include
CFLAGS_MAC := /opt/homebrew/include/


ifeq ($(CURRENT_OS),windows)
    LDFLAGS := $(LDFLAGS)
	CFLAGS := -I$(CFLAGS_WINDOWS) -c 
	RunGame := ./game.exe
endif

ifeq ($(CURRENT_OS),mac)
    LDFLAGS := $(LDFLAGS_MAC) $(LDFLAGS)
	CFLAGS := -I$(CFLAGS_MAC) -c 
	RunGame := ./game
endif


theGame: main.o player.o map.o audio.o network.o menu.o perks.o foundation.o
	$(CC) -o game main.o player.o map.o audio.o network.o menu.o perks.o foundation.o $(LDFLAGS)
	$(RunGame)
main.o: src/main.c
	@echo "Building Game" 
	$(CC) src/main.c $(CFLAGS)
player.o: $(SRC)/player/player.c
	$(CC) -c $(SRC)/player/player.c $(CFLAGS)
map.o: $(SRC)/map/map.c
	$(CC) -c $(SRC)/map/map.c $(CFLAGS)
audio.o: $(SRC)/audio/audio.c
	$(CC) -c $(SRC)/audio/audio.c $(CFLAGS)
network.o: $(SRC)/network/network.c
	$(CC) -c $(SRC)/network/network.c $(CFLAGS)
menu.o: $(SRC)/menu/menu.c
	$(CC) -c $(SRC)/menu/menu.c $(CFLAGS)
perks.o: $(SRC)/perks/perks.c
	$(CC) -c $(SRC)/perks/perks.c $(CFLAGS)
foundation.o: $(SRC)/foundation/foundation.c
	$(CC) -c $(SRC)/foundation/foundation.c $(CFLAGS)

clean:
	rm -f game
	rm *.o
	rm *.out