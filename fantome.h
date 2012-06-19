#ifndef H_FANTOME
#define H_FANTOME
#include "levelmanager.h"
#include "constantes.h"

typedef enum Deplacement {HAUT_1, HAUT_2, BAS_1, BAS_2, GAUCHE_1, GAUCHE_2, DROITE_1, DROITE_2}Deplacement;

typedef struct Fantome
{
	int cur_direction;
	SDL_Rect position;
	SDL_Surface* image[8];
}Fantome;

//Fantome ftm[NB_GHOST_BLOCKS];

void init_ghosts(Fantome*);
void affiche_fantome(Fantome*, int);

#endif
