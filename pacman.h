#ifndef H_PACMAN
#define H_PACMAN
#include "graphics.h"

#define PAC_SIZE 30

typedef struct Pacman
{
	int cur_direction;
	int direction_locked;
	SDL_Rect position;
	SDL_Surface* image[5];
}Pacman;

void init_pacman(Pacman*);
void affiche_pacman(Pacman*, int);
void deplace_pacman(Pacman*, int);
int can_go(Pacman*, int);
int isAlive(Pacman*);
void getPixelColor(int, int, SDL_Surface*, Uint8*, Uint8*, Uint8*);
Uint32 getpixel(SDL_Surface*, int, int);
void test_color();


#endif
