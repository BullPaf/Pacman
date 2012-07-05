#include "config.h"

void load_default_config(config *cfg)
{
	cfg->nb_players = 1;
	cfg->players[0].character = PACMAN;
	cfg->players[0].up = SDLK_UP;
	cfg->players[0].down = SDLK_DOWN;
	cfg->players[0].left = SDLK_LEFT;
	cfg->players[0].right = SDLK_RIGHT;
	
	cfg->players[1].character = GHOST;
	cfg->players[1].up = SDLK_z;
	cfg->players[1].down = SDLK_s;
	cfg->players[1].left = SDLK_q;
	cfg->players[1].right = SDLK_d;
}

void configure(config *cfg)
{
	//options_menu(*cfg);
	fprintf(stderr, "Default configuration loaded\n");
	load_default_config(cfg);
}
