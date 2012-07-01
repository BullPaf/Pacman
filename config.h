#ifndef H_CONFIG
#define H_CONFIG
#include <stdlib.h>

enum {PACMAN, FANTOME};

typedef struct config
{
	int nb_players;
	int *players;
}config;

void load_default_config(config*);

#endif
