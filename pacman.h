#ifndef H_PACMAN
#define H_PACMAN
#include "graphics.h"
#include "constantes.h"
#include "levelmanager.h"

typedef struct Pacman
{
	int cur_direction;
	int nb_lives;
	SDL_Rect position;
	SDL_Surface* image[16];
}Pacman;

void init_pacman(Pacman*);
void affiche_pacman(Pacman*, int);
void deplace_pacman(Pacman*, int);
void pac_death(Pacman*);
void pac_restart(Pacman*);

#endif
