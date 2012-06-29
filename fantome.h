#ifndef H_FANTOME
#define H_FANTOME
#include <time.h>
#include "levelmanager.h"
#include "constantes.h"
#include "pacman.h"

typedef struct Fantome
{
	int cur_direction; //Direction en cours
	int speed; //Deplacement en pixel à chaque coup
	int num_image; //L'image à afficher
	int invinsible; //S'il est invinsible
	int dead; //S'il est mort
	int counter; //Permet de mesurer le temps
	int couleur; //La couleur du fantome ca peut servir
	SDL_Rect position, start; //Position actuelle et position de départ
	SDL_Surface* image[16]; //Les différentes images des fantomes
}Fantome;

void init_ghosts(Fantome*);
void affiche_fantomes(Fantome*);
int find_direction(Fantome, SDL_Rect, int);
void deplace_fantomes(Fantome*, int*, SDL_Rect, int);
void ghost_restart(Fantome*);
void ghost_death(Fantome*);

#endif
