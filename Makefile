CC=gcc
CFLAGS=-O3 -Wall `sdl-config --cflags`
LIBS=`sdl-config --libs` -lm -lSDL_ttf -lSDL_image

all: pacman

pacman: main.c pacman.o graphics.o levelmanager.o editeur.o jeu.o fantome.o menus.o input.o
	$(CC) $(CFLAGS) main.c graphics.o pacman.o levelmanager.o editeur.o jeu.o fantome.o menus.o input.o -o pacman $(LIBS)

pacman.o: pacman.c pacman.h
	$(CC) $(CFLAGS) -c pacman.c

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) -c graphics.c

levelmanager.o: levelmanager.c levelmanager.h
	$(CC) $(CFLAGS) -c levelmanager.c

editeur.o: editeur.c editeur.h
	$(CC) $(CFLAGS) -c editeur.c

jeu.o: jeu.c jeu.h
	$(CC) $(CFLAGS) -c jeu.c

fantome.o: fantome.c fantome.h
	$(CC) $(CFLAGS) -c fantome.c

menus.o: menus.c menus.h
	$(CC) $(CFLAGS) -c menus.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

clean:
	rm -f *core *.o pacman
