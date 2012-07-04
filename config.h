#ifndef H_CONFIG
#define H_CONFIG
#include <stdlib.h>
#include "levelmanager.h"
#include "menus.h"
#include "graphics.h"

typedef struct
{
	int up, down, left, right; //Key for directions
	int character;
}player;

typedef struct config
{
	int nb_players;
	player *players;
}config;

void load_default_config(config*);
void configure(config*);
void delete_config(config *cfg);

#endif
