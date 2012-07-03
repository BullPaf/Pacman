#include "pacman.h"

void init_pacman(Pacman *pac, config *cfg)
{
	char img[32];
	int i;
	for(i=0; i<19; i++)
	{
		sprintf(img, "image/pacman/%d.png", i);
		if( (pac->image[i] = IMG_Load(img)) == NULL )
		{
			fprintf(stderr, "Erreur chargement texture n°%d de Pacman\n", i);
			exit(EXIT_FAILURE);
		}
	}
	pac->position.x    = PAC_START_X*BLOCK_SIZE;
	pac->position.y    = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->num_image     = (pac->cur_direction*2);
	pac->counter       = 0;
	pac->nb_lives      = 2;
	pac->nb_keys       = 0;
	pac->speed         = 4;
	pac->target.x      = -1;
	pac->target.y      = -1;
	pac->controllerFonction = ia_controller;
	for(i=0; i<cfg->nb_players; i++)
	{
		if (cfg->players[i].character==PACMAN)
		{
			pac->controlled_by = i;
			pac->controllerFonction = human_controller;
		}
	}
	LEVEL[PAC_START_Y][PAC_START_X].type=RIEN;
}

void pac_restart(Pacman *pac)
{
	pac->position.x = PAC_START_X*BLOCK_SIZE;
	pac->position.y = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->num_image = (pac->cur_direction*2);
}

//Pour afficher Pacman
void affiche_pacman(Pacman *pac)
{
	if(pac->position.x < 0 || pac->position.y < 0)
	{
		SDL_Rect copie_pos;
		copie_pos.x = pac->position.x;
		copie_pos.y = pac->position.y;
		SDL_BlitSurface(pac->image[pac->num_image], NULL, screen, &copie_pos);
	}
	else SDL_BlitSurface(pac->image[pac->num_image], NULL, screen, &pac->position);
}

void updatePacman(Pacman *pac)
{
	if( (pac->num_image)%2==0 ) pac->num_image = (pac->cur_direction*2)+1;
	else pac->num_image = (pac->cur_direction*2);
}

void set_pac_target(Pacman *pac)
{
	pac->target.x = -1;
	pac->target.y = -1;
}

//Animation de mort
void pac_death(Pacman *pac)
{
	int i;
	for(i=8; i<19; i++)
	{
		SDL_Delay(40);
		SDL_FillRect(screen, &pac->position, noir);
		SDL_BlitSurface(pac->image[i], NULL, screen, &pac->position);
		SDL_Flip(screen);
	}
	pac->nb_lives--;
	pac_restart(pac);
}
