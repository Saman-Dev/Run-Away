
INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -lm

Client: main.o
	gcc -o Client main.o $(LDFLAGS)

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o Client