#ifndef H_FANTOME
#define H_FANTOME
#include <time.h>
#include "levelmanager.h"
#include "constantes.h"

typedef enum Deplacement {HAUT_1, HAUT_2, DROITE_1, DROITE_2, BAS_1, BAS_2, GAUCHE_1, GAUCHE_2}Deplacement;

typedef struct Fantome
{
	int cur_direction;
	int num_image;
	int invinsible;
	int counter;
	SDL_Rect position;
	SDL_Surface* image[16];
}Fantome;

void init_ghosts(Fantome*);
void affiche_fantomes(Fantome*);
int find_direction(Fantome, SDL_Rect);
void deplace_fantomes(Fantome*, int*);
void ghost_death(Fantome*, int);
void ghost_restart(Fantome*, int);

#endif
