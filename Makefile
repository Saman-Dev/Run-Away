# Makefile for for both MacOS and windows 
SRCDIR=./src 
CC=gcc 
INCLUDE = /opt/homebrew/include 
LIBS = /opt/homebrew/lib 
CFLAGS = -g -I$(INCLUDE) -c 
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image 
 

ifeq ($(shell uname -s), Darwin)
    INCLUDE = /opt/homebrew/include 
else
    INCLUDE = C:\msys64\mingw64\include\SDL2
endif


main: 
	@echo "Building Game" 
	$(CC) $(CFLAGS) $(SRCDIR)src/main.c -o $@.o  
	$(CC) main.o -o main.out $(LDFLAGS) -L $(LIBS) 
 
clean: 
	rm -f main.o 
	rm -f main.out



