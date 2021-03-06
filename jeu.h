#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "menus.h"
#include "levelmanager.h"
#include "config.h"
#include "input.h"
#include "scoremanager.h"
#include <dirent.h>
#include <string.h>

typedef struct score_message score_message;
struct score_message
{
	SDL_Surface *img;
	SDL_Rect pos;
	int elapsed;
	struct score_message *next;
};

void one_level(int, config*);
void jouer(Pacman*, Fantome*, Input, config*, int, score_message**);
void survivor(int, config*);
int pacmanIsHuman(config*);
void delete(Pacman *, Fantome*);
void campagne(config*, int);
void action(Pacman*, Fantome*, score_message**);
void add_new_message(score_message**, int, SDL_Rect);
void draw_score(int, int);
int check_colision(Pacman*, Fantome);
void display_messages(score_message**);
void init_game();

#endif
