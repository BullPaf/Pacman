#ifndef H_PACMAN
#define H_PACMAN
#include "graphics.h"
#include "constantes.h"
#include "levelmanager.h"

typedef struct Pacman
{
	int cur_direction; //Direction en cours
	int speed; //Deplacement en pixel à chaque coup
	int nb_lives; //Nombre de vie(s)
	int nb_keys; //Nombdre de clé
	int num_image; //L'image à afficher
	int counter; //Permet de mesurer le temps
	SDL_Rect position; //Position actuelle
	SDL_Surface* image[19]; //Les différentes images de pacman
}Pacman;

void init_pacman(Pacman*);
void affiche_pacman(Pacman*);
void deplace_pacman(Pacman*, int);
void pac_restart(Pacman*);
void pac_death(Pacman*);

#endif
