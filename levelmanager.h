#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.h"
#include "constantes.h"
#include <string.h>
#define LINE_SIZE 2048

//typedef enum Block_type;
typedef enum Block_type {RIEN, MUR, BONUS}Block_type;
//typedef enum Position;
typedef enum Position {CENTRE, HAUT, DROITE, BAS, GAUCHE}Position;

/*Les surface qui contiennent toutes les textures du jeu*/
SDL_Surface *BLOCK_MUR[NB_WALL_BLOCKS]; //Les différents type de murs
SDL_Surface *BLOCK_BONUS[NB_BONUS_BLOCKS]; //Les différents type de bonus

/*Une CASE contient le type correspondant au tableau des textures*/
typedef struct CASE
{
	Block_type type; //Si c'est un MUR ou un BONUS
	int nb_elt; //Combien d'element dans la case, si MUR nb_elt=1
	Position position[MAX_ELT_CASE]; //Pour chacun des éléments de la case défini sa position(HAUT ou DROITE ect...)
	int elt_type[MAX_ELT_CASE]; //Permet de savoir pour chaque élément quelle texture afficher
}CASE;

//Un niveau est un ensemble de CASE
CASE LEVEL[NB_BLOCKS_HAUTEUR][NB_BLOCKS_LARGEUR];

int init_blocks(); //Ok
void init_level(); //Ok
void load_level(); //Ok
//void save_level(); //NO
void extract_val(char *s, int line); //OK
void draw_level(); //NO

#endif
