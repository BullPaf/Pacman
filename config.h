#ifndef H_CONFIG
#define H_CONFIG
#include <stdlib.h>
//#include "menus.h"
#include "levelmanager.h"
#include "graphics.h"
//#include "menus.h"

typedef struct
{
	int up, down, left, right; //Key for directions
	int character;
}player;

typedef struct config
{
	int nb_players;
	player players[2];
}config;

void load_default_config(config*);
void configure(config*);

#endif
