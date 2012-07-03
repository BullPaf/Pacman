#include "fantome.h"

void init_ghosts(Fantome *ftm, config *cfg)
{
	int i,j;
	char img[32];
	for (i=0; i<NB_GHOST; i++)
	{
		ftm[i].couleur = GHOST_COULEUR[i];
		for(j=0; j<8; j++)
		{
			sprintf(img, "image/ghosts/%d.png", ftm[i].couleur*8+j);
			if( (ftm[i].image[j] = IMG_Load(img)) == NULL )
			{
				fprintf(stderr, "Erreur chargement texture du Fantome %d\n", i);
				exit(EXIT_FAILURE);
			}
		}
		for(j=8; j<16; j++)
		{
			sprintf(img, "image/ghosts/%d.png", 24+j);
			if( (ftm[i].image[j] = IMG_Load(img)) == NULL )
			{
				fprintf(stderr, "Erreur chargement texture du Fantome %d\n", i);
				exit(EXIT_FAILURE);
			}
		}
		ftm[i].start.x       = (GHOST_START_X[i])*BLOCK_SIZE;
		ftm[i].start.y       = (GHOST_START_Y[i])*BLOCK_SIZE;
		ftm[i].position.x    = ftm[i].start.x;
		ftm[i].position.y    = ftm[i].start.y;
		ftm[i].speed         = 4;
		ftm[i].cur_direction = 1;
		ftm[i].num_image     = (ftm[i].cur_direction)*2;
		ftm[i].invinsible    = 1;
		ftm[i].dead          = 0;
		ftm[i].nb_keys       = 0;
		ftm[i].counter       = 0;
		ftm[i].controllerFonction = ia_controller;
		ftm[i].target.x      = -1;
		ftm[i].target.y      = -1;
		ftm[i].controlled_by = -1;
		for(j=0; j<cfg->nb_players; j++)
		{
			if (cfg->players[j].character==GHOST && !i)
			{
				ftm[i].controlled_by = j;
				ftm[i].controllerFonction = human_controller;
			}
		}
		LEVEL[GHOST_START_Y[i]][GHOST_START_X[i]].type=RIEN;
	}
}

void ghost_restart(Fantome *ftm)
{
	ftm->position.x    = ftm->start.x;
	if(ftm->controlled_by != -1) ftm->controllerFonction = human_controller;
	ftm->position.y    = ftm->start.y;
	ftm->cur_direction = rand()%4;
	ftm->num_image     = (ftm->cur_direction)*2;
	ftm->speed         = 4;
	ftm->invinsible    = 1;
	ftm->dead          = 0;
	ftm->counter       = 0;
}

//Pour afficher Pacman
void affiche_fantomes(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		if(ftm[i].position.x < 0 || ftm[i].position.y < 0)
		{
			SDL_Rect copie_pos;
			copie_pos.x = ftm[i].position.x;
			copie_pos.y = ftm[i].position.y;
			SDL_BlitSurface(ftm[i].image[ftm[i].num_image], NULL, screen, &copie_pos);
		}
		else SDL_BlitSurface(ftm[i].image[ftm[i].num_image], NULL, screen, &(ftm[i].position));
	}
}

void set_ftm_target(Fantome *f, SDL_Rect pac)
{
	if(f->dead)
	{
		f->target.x=f->start.x;
		f->target.y=f->start.y;
	}
	else 
	{
		f->target.x=pac.x;
		f->target.y=pac.y;
	}
}

void updateGhosts(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		if( ftm[i].invinsible )
		{
			if( (ftm[i].num_image)%2==0 ) ftm[i].num_image = (ftm[i].cur_direction*2)+1;
			else ftm[i].num_image = (ftm[i].cur_direction*2);
		}
		else if( !(ftm[i].dead)) //Fantome peut etre mangé
		{
			int tempsEcoule = SDL_GetTicks()-ftm[i].counter;
			if(tempsEcoule < 7000 && tempsEcoule > 5000) //Fantome bientot invulnerable
			{
				if(ftm[i].num_image==10) ftm[i].num_image=9;
				else ftm[i].num_image=10;
			}
			else if (tempsEcoule >= 7000) //Fantome redevient invulnérable
			{
				ftm[i].invinsible = 1;
				ftm[i].speed      = 4;
				if(ftm[i].position.x % 4 != 0) ftm[i].position.x+=2;
				if(ftm[i].position.y % 4 != 0) ftm[i].position.y+=2;
				//On charge l'image correspondante à la direction en cours
				ftm[i].num_image=(ftm[i].cur_direction)*2;
			}
			else
			{
				if(ftm[i].num_image%2==0) ftm[i].num_image+=1;
				else ftm[i].num_image -= 1;
			}
		}
		else //Fantome mort
		{
			int tempsEcoule = SDL_GetTicks()-ftm[i].counter;
			if (tempsEcoule >= 7000 || (ftm[i].position.x == ftm[i].start.x && ftm[i].position.y == ftm[i].start.y) )
				ghost_restart(ftm+i);
			ftm[i].num_image=12+ftm[i].cur_direction;
		}
	}
}

void ghost_death(Fantome* ftm)
{
	//POINT p1;
	SDL_Rect ftm_case = get_case(ftm->position, ftm->cur_direction);
	//p1.x=(ftm_case.x+1)*BLOCK_SIZE; p1.y=(ftm_case.y+1)*BLOCK_SIZE;
	//aff_pol("BRAVO BB!", FONT_SIZE, p1, blanc);
	//SDL_Flip(screen);
	SDL_Delay(500);
	ftm->dead=1;
	ftm->controllerFonction = ia_controller;
	ftm->speed = 10;
	ftm->position.x=ftm_case.x*BLOCK_SIZE;
	ftm->position.y=ftm_case.y*BLOCK_SIZE;
	ftm->counter=SDL_GetTicks();
}
