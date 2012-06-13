CC=gcc
CFLAGS=-O3 -Wall `sdl-config --cflags`
LIBS=`sdl-config --libs` -lm -lSDL_ttf -lSDL_image

all: pacman

pacman: main.c pacman.o graphics.o levelmanager.o
	$(CC) $(CFLAGS) main.c graphics.o pacman.o levelmanager.o -o pacman $(LIBS)

pacman.o: pacman.c pacman.h
	$(CC) $(CFLAGS) -c pacman.c

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) -c graphics.c

levelmanager.o: levelmanager.c levelmanager.h
	$(CC) $(CFLAGS) -c levelmanager.c

clean:
	rm -f *core *.o pacman
