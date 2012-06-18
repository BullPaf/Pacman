#ifndef H_PACMAN
#define H_PACMAN
#include "graphics.h"
#include "constantes.h"
#include "levelmanager.h"

typedef struct Pacman
{
	int cur_direction;
	int nb_lives;
	int score;
	SDL_Rect position;
	SDL_Surface* image[5];
}Pacman;

void init_pacman(Pacman*);
void affiche_pacman(Pacman*, int);
void deplace_pacman(Pacman*, int);
int dans_case(Pacman*);

#endif
