#ifndef H_FANTOME
#define H_FANTOME
#include <time.h>
#include "levelmanager.h"
#include "constantes.h"
#include "movemanager.h"

typedef struct Fantome
{
	int cur_direction; //Direction en cours
	int speed; //Deplacement en pixel à chaque coup
	int num_image; //L'image à afficher
	int invinsible; //S'il est invinsible
	int dead; //S'il est mort
	int counter; //Permet de mesurer le temps
	int controlled_by;
	int nb_keys;
	int couleur; //La couleur du fantome ca peut servir
	SDL_Rect position, start, target; //Position actuelle et position de départ
	SDL_Surface* image[16]; //Les différentes images des fantomes
	void (*controllerFonction)(Input, config, int, SDL_Rect*, int*, int*, int*, int*, SDL_Rect);
}Fantome;

void init_ghosts(Fantome*, config*);
void affiche_fantomes(Fantome*);
void updateGhosts(Fantome *);
void set_ftm_target(Fantome*, SDL_Rect);
void ghost_restart(Fantome*);
void ghost_death(Fantome*);
void delete_ghosts(Fantome*);

#endif
