#ifndef H_PACMAN
#define H_PACMAN
#include "graphics.h"
#include "constantes.h"
#include "levelmanager.h"
#include "movemanager.h"

typedef struct Pacman
{
	int cur_direction; //Direction en cours
	int score;
	int speed; //Deplacement en pixel à chaque coup
	int nb_lives; //Nombre de vie(s)
	int nb_keys; //Nombdre de clé
	int num_image; //L'image à afficher
	int counter; //Permet de mesurer le temps
	int controlled_by;
	int ghost_eaten; //Le nombre de fantomes mangés en un seul coup
	SDL_Rect position, target; //Position actuelle
	SDL_Surface* image[19]; //Les différentes images de pacman
	void (*controllerFonction)(Input, config, int, SDL_Rect*, int*, int*, int*, int*, SDL_Rect);
}Pacman;

void init_pacman(Pacman*, config*);
void affiche_pacman(Pacman*);
void updatePacman(Pacman*);
void set_pac_target(Pacman*);
void pac_restart(Pacman*);
void pac_death(Pacman*);
void draw_pac_infos(Pacman*);
void delete_pacman(Pacman*);

#endif
