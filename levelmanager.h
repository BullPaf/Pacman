#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.h"
#include <string.h>
#define BLOCK_SIZE 40
#define OFFSET 4
#define NB_BLOCKS 11
#define LINE_SIZE 128

/*Les surface qui contiennent toutes les textures du jeu*/
SDL_Surface *block[NB_BLOCKS]; //Les différents type de blocs

//Une CASE contient le type correspondant au tableau des textures
typedef struct CASE
{
	int block_type;
}CASE;

//Un niveau est un ensemble de CASE
CASE LEVEL[HEIGHT/BLOCK_SIZE][WIDTH/BLOCK_SIZE];
int init_blocks(); 

void init_level();
void load_level();
void save_level();
void extract_val(char *s, int line);
void draw_level();

int enter_edit_mode();
void load_gui();
void highlight_block(int);
int get_block_type(POINT, int);

#endif
