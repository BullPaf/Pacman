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
	SDL_Surface* image[12];
}Fantome;

void init_ghosts(Fantome*);
void affiche_fantomes(Fantome*, int);
int find_direction(Fantome);
void deplace_fantomes(Fantome*, int*);

#endif
