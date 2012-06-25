#ifndef H_PACMAN
#define H_PACMAN
#include "graphics.h"
#include "constantes.h"
#include "levelmanager.h"

typedef struct Pacman
{
	int cur_direction;
	int speed;
	int nb_lives;
	int num_image;
	SDL_Rect position;
	SDL_Surface* image[20];
}Pacman;

void init_pacman(Pacman*);
void affiche_pacman(Pacman*);
void deplace_pacman(Pacman*, int);
void pac_restart(Pacman*);

#endif
