#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.h"
#include "constantes.h"
#include <string.h>
#define LINE_SIZE 2048

//typedef enum Block_type;
typedef enum Block_type {RIEN, MUR, BONUS, PACMAN, GHOST}Block_type;
//typedef enum Position;
typedef enum Position {CENTRE, HAUT, DROITE, BAS, GAUCHE}Position;
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
	int nb_elt; //Combien d'element dans la case, si MUR nb_elt=1
	Position position[MAX_ELT_CASE]; //Pour chacun des éléments de la case défini sa position(HAUT ou DROITE ect...)
	int elt_type[MAX_ELT_CASE]; //Permet de savoir pour chaque élément quelle texture afficher
}CASE;

int PAC_START_X;
int PAC_START_Y;
int PAC_START_DIRECTION;
int GHOST_START_X[NB_GHOST_BLOCKS];
int GHOST_START_Y[NB_GHOST_BLOCKS];
int SCORE;
int POINTS;
Couleur GHOST_COULEUR[NB_GHOST_BLOCKS];

//Un niveau est un ensemble de CASE
CASE LEVEL[NB_BLOCKS_HAUTEUR][NB_BLOCKS_LARGEUR];
char LEVEL_FILE[NB_LEVEL][128];

int init_blocks(); //Ok
void init_level(); //Ok
void load_level(char*); //Ok
void save_level(char*); //ok
void extract_val(char *s, int line); //OK
void draw_level(); //ok
void affiche_une_case(CASE, SDL_Rect*, SDL_Surface*);
char* select_file();

int dans_case(SDL_Rect);
int can_move(SDL_Rect, int, int);
int in_intersection(SDL_Rect, int);
void move(SDL_Rect*, int, int);
SDL_Rect get_case(SDL_Rect, int);

#endif
