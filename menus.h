#ifndef H_MENUS
#define H_MENUS
#include <stdlib.h>
#include "input.h"
#include "constantes.h"
#include "levelmanager.h"
#include "scoremanager.h"
#include "config.h"

typedef struct
{
	char title[64];
	char **options;
	SDL_Surface *img;
	SDL_Rect pos;
	int selection, nb_options, *couleur, *available, font_size, space;
	POINT p1;
}Menu;


void init_menu(Menu*, int);
void delete_menu(Menu*);
void edit_campagne();
void draw_version();
int main_menu();
int save_menu(int);
void play_menu(int);
void new_file_menu(char*);
void options_menu(config*);
void win_menu();
void lost_menu();
int game_menu();
int edit_menu();
int select_file_menu();
int draw_menu(Menu);

#endif
