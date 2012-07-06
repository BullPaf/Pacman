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
	pac->score         = 0;
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
}

void pac_restart(Pacman *pac)
{
	pac->position.x = PAC_START_X*BLOCK_SIZE;
	pac->position.y = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->num_image = (pac->cur_direction*2);
	LEVEL[PAC_START_Y][PAC_START_X].type=RIEN;
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

/*Pour l'instant si Pacman est controllé par IA
 * il ira au hasard
 * Voir Find_direction() dans movemanager.c*/
void set_pac_target(Pacman *pac)
{
	pac->target.x = -1;
	pac->target.y = -1;
}

/*Affiche les vies et les clés du pacman*/
void draw_pac_infos(Pacman *pac)
{
	POINT p1;
	SDL_Rect position;
	int i;
	p1.x=WIDTH+10; p1.y=10;
	aff_pol("Lives :", FONT_SIZE, p1, blanc);
	position.y=p1.y+30;
	for(i=0; i<pac->nb_lives; i++)
	{
		position.x=WIDTH+5+i*BLOCK_SIZE;
		SDL_BlitSurface(pac->image[DROITE*2], NULL, screen, &position);
	}
	p1.y+=75;
	aff_pol("Keys :", FONT_SIZE, p1, blanc);
	position.y=p1.y+30;
	for(i=0; i<pac->nb_keys; i++)
	{
		position.x=WIDTH+5+i*BLOCK_SIZE;
		SDL_BlitSurface(BLOCK_BONUS[8], NULL, screen, &position);
	}
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
	pac->nb_lives--; //On perd une vie :'(
	pac_restart(pac);
}

void delete_pacman(Pacman *pac)
{
	int i;
	for(i=0; i<19; i++) free(pac->image[i]);
}
