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
CFLAGS_MAC := /opt/homebrew/include


ifeq ($(CURRENT_OS),windows)
    LDFLAGS := $(LDFLAGS_WINDOWS) $(LDFLAGS)
	CFLAGS := -g -I$(CFLAGS_WINDOWS) -c 

endif

ifeq ($(CURRENT_OS),mac)
    LDFLAGS := $(LDFLAGS_MAC) $(LDFLAGS)
	CFLAGS := -g -I$(CFLAGS_MAC) -c 
	CFLAGS  := $(CFLAGS_MAC) $(CFLAGS)
endif


main: 
	@echo "Building Game" 
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $@.o
	$(CC) main.o -o main $(LDFLAGS)

clean:
	rm main
	rm main.o
	rm main.out








