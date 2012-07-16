#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.h"
#include "constantes.h"
#include <string.h>

#define LINE_SIZE 2048

//typedef enum Block_type;
typedef enum Block_type {RIEN, MUR, BONUS, PACMAN, GHOST}Block_type;
typedef enum Couleur {ROUGE, VIOLET, BLEU, JAUNE}Couleur;

/*Les surface qui contiennent toutes les textures du jeu*/
SDL_Surface *BLOCK_MUR[NB_WALL_BLOCKS]; //Les différents type de murs
SDL_Surface *BLOCK_BONUS[NB_BONUS_BLOCKS]; //Les différents type de bonus
SDL_Surface *BLOCK_PACMAN[NB_PACMAN_BLOCK];
SDL_Surface *BLOCK_GHOST[NB_GHOST_BLOCKS];

/*Une CASE contient le type correspondant au tableau des textures*/
typedef struct CASE
{
	Block_type type; //Si c'est un MUR ou un BONUS
	int elt_type;
}CASE;

int PAC_START_X;
int PAC_START_Y;
int PAC_START_DIRECTION;
int NB_GHOST;
int GHOST_START_X[NB_MAX_GHOSTS];
int GHOST_START_Y[NB_MAX_GHOSTS];
Couleur GHOST_COULEUR[NB_MAX_GHOSTS];
int POINTS; //Le nombre de petit point à manger dans le niveau courant
int DELAY; //Temps de rafraichissement
int NB_LEVEL; //Le nombre de niveaux total
int CAMPAGNE_LEVEL; //Le nombre de niveaux de la campagne
int SAVE_ENABLE;
//Un niveau est un ensemble de CASE
CASE LEVEL[NB_BLOCKS_HAUTEUR][NB_BLOCKS_LARGEUR];
char LEVEL_FILE[256][128]; //Stocke tout les niveaux
char CAMPAGNE[256][128]; //Stocke uniquement les niveaux pour la campagne

int init_blocks();
void init_level();
void load_level(int);
void save_level(int);
void save_game(int);
int load_game();
int has_saved_game();
void extract_val(char *s, int line);
void draw_level();
void affiche_une_case(CASE, SDL_Rect*, SDL_Surface*);
void delete_blocks();

int dans_case(SDL_Rect);
void remove_bloc(int, int);
int remove_right_bloc(int);
int remove_left_bloc(int);
int remove_down_bloc(int);
int remove_up_bloc(int);
SDL_Rect get_case(SDL_Rect, int);

#endif
