#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.h"
#include "constantes.h"
#include <string.h>
#define LINE_SIZE 128

/*Les surface qui contiennent toutes les textures du jeu*/
SDL_Surface *block[NB_BLOCKS]; //Les différents type de blocs

/*Une CASE contient le type correspondant au tableau des textures*/
typedef struct CASE
{
	int block_type;
}CASE;

//Un niveau est un ensemble de CASE
CASE LEVEL[NB_BLOCKS_HAUTEUR][NB_BLOCKS_LARGEUR];

int init_blocks(); 
void init_level();
void load_level();
void save_level();
void extract_val(char *s, int line);
void draw_level();

#endif
