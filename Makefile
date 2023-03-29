# Makefile for for both MacOS and windows 
SRCDIR=./src 
CC=gcc 

ifeq ($(shell uname -s), Darwin)

    INCLUDE = /opt/homebrew/include 
	LIBS = /opt/homebrew/lib 
	LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image

else

    INCLUDE = C:\msys64\mingw64\include
    LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows -lm
 
endif

CFLAGS = -g -I$(INCLUDE) -c 



ifeq ($(shell uname -s), Darwin)

main: 
	@echo "Building Game" 
	$(CC) $(CFLAGS) $(SRCDIR)src/main.c -o $@.o
	$(CC) main.o -o main.out $(LDFLAGS) -L $(LIBS) 

clean: 
	rm -f main.o 
	rm -f main.out
else

main: 
	@echo "Building Game" 
	$(CC) $(CFLAGS) $(SRCDIR)src/main.c -o $@.o
	$(CC) main.o -o main $(LDFLAGS)

clean:
	rm main
	rm main.o

endif




