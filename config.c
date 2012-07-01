#include "config.h"

void load_default_config(config *cfg)
{
	cfg->nb_players = 1;
	cfg->players = (int*)malloc(cfg->nb_players*sizeof(int));
	cfg->players[0] = PACMAN;
}
