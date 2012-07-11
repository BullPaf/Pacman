#include "fantome.h"

void init_ghosts(Fantome *ftm, config *cfg)
{
	int i,j;
	char img[32];
	for (i=0; i<NB_GHOST; i++)
	{
		ftm[i].couleur = GHOST_COULEUR[i]; //A chaque fantome une couleur
		//Chargement des textures propres à chaque fantome
		for(j=0; j<8; j++)
		{
			sprintf(img, "image/ghosts/%d.png", ftm[i].couleur*8+j);
			if( (ftm[i].image[j] = IMG_Load(img)) == NULL )
			{
				fprintf(stderr, "Erreur chargement texture du Fantome %d\n", i);
				exit(EXIT_FAILURE);
			}
		}
		//Chargement des texture commune à touts les fantomes
		for(j=8; j<16; j++)
		{
			sprintf(img, "image/ghosts/%d.png", 24+j);
			if( (ftm[i].image[j] = IMG_Load(img)) == NULL )
			{
				fprintf(stderr, "Erreur chargement texture du Fantome %d\n", i);
				exit(EXIT_FAILURE);
			}
		}
		ftm[i].start.x       = GHOST_START_X[i];
		ftm[i].start.y       = GHOST_START_Y[i];
		ftm[i].position.x    = ftm[i].start.x*BLOCK_SIZE;
		ftm[i].position.y    = ftm[i].start.y*BLOCK_SIZE;
		ftm[i].default_speed = 4;
		ftm[i].speed         = 4;
		ftm[i].cur_direction = HAUT;
		ftm[i].num_image     = (ftm[i].cur_direction)*2;
		ftm[i].invinsible    = 1;
		ftm[i].dead          = 0;
		ftm[i].nb_keys       = 0;
		ftm[i].counter       = 0;
		ftm[i].controllerFonction = ia_controller; //Par defaut controllé par IA
		ftm[i].target.x      = -1;
		ftm[i].target.y      = -1;
		ftm[i].controlled_by = -1;
		for(j=0; j<cfg->nb_players; j++)
		{
			if (cfg->players[j].character==GHOST && !i) //Si un joueur veut controller un fantome
			{
				ftm[i].controlled_by = j;
				ftm[i].controllerFonction = human_controller; //Il controllera toujours le fantome 0
			}
		}
		LEVEL[GHOST_START_Y[i]][GHOST_START_X[i]].type=RIEN;
	}
}

/*No comment*/
void ghost_restart(Fantome *ftm)
{
	ftm->position.x    = ftm->start.x*BLOCK_SIZE;
	if(ftm->controlled_by != -1) ftm->controllerFonction = human_controller; //Si un joueur controllé le fantome
	ftm->position.y    = ftm->start.y*BLOCK_SIZE;
	ftm->cur_direction = rand()%4;
	ftm->num_image     = (ftm->cur_direction)*2;
	ftm->speed         = ftm->default_speed;
	ftm->invinsible    = 1;
	ftm->dead          = 0;
	ftm->counter       = 0;
}

//Pour afficher le fantome
void affiche_fantomes(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		/*Si les coordonnées sont négatives on passe une copie à SDL_BlitSurface
		 * Pour ne pas remettre les coordonnées à 0*/
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

/*Permet d'affecter la cible*/
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

/*Met à jour graphiquement le fantome*/
void updateGhosts(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		if( ftm[i].invinsible ) //Cas normal
		{
			//Permet la permutation des images pour effet de mouvement
			if( (ftm[i].num_image)%2==0 ) ftm[i].num_image = (ftm[i].cur_direction*2)+1;
			else ftm[i].num_image = (ftm[i].cur_direction*2);
		}
		else if( !(ftm[i].dead)) //Fantome peut etre mangé
		{
			int tempsEcoule = SDL_GetTicks()-ftm[i].counter;
			if(tempsEcoule < 7000 && tempsEcoule > 5000) //Fantome bientot invulnerable
			{
				//Permutation images bleue et blanche
				if(ftm[i].num_image==10) ftm[i].num_image=9;
				else ftm[i].num_image=10;
			}
			else if (tempsEcoule >= 7000) //Fantome redevient invulnérable
			{
				ftm[i].invinsible = 1;
				ftm[i].speed      = ftm[i].default_speed;
				//On charge l'image correspondante à la direction en cours
				ftm[i].num_image=(ftm[i].cur_direction)*2;
			}
			else //Le fantome peut etre mangé
			{
				if(ftm[i].num_image%2==0) ftm[i].num_image+=1;
				else ftm[i].num_image -= 1;
			}
		}
		else //Fantome mort
		{
			int tempsEcoule = SDL_GetTicks()-ftm[i].counter;
			//Un timer pour remettre le fantome à se position initiale s'il n'a pas réussi à y aller tout seul
			if (tempsEcoule >= 7000 || (ftm[i].position.x == ftm[i].start.x*BLOCK_SIZE && ftm[i].position.y == ftm[i].start.y*BLOCK_SIZE) )
				ghost_restart(ftm+i);
			ftm[i].num_image=12+ftm[i].cur_direction; //Les yeux!
		}
	}
}

void speed_up(Fantome *ftm, int acc)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		ftm[i].default_speed += acc;
		if( !(ftm[i].dead) && ftm[i].invinsible) ftm[i].speed = ftm[i].default_speed;
	}
}

/*rend les fantomes mangeables*/
void set_ghosts_eatable(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		if( !(ftm[i].dead) ) //S'il n'est pas déjà mort
		{
			ftm[i].invinsible = 0;
			ftm[i].speed      = ftm[i].default_speed/2;
			//Initialisation du compteur pour compter 5 sec
			ftm[i].counter = SDL_GetTicks();
			//On charge l'image du fantome vulnérable
			ftm[i].num_image  = 8;
		}
	}
}

/*Tue le fantome*/
void ghost_death(Fantome* ftm)
{
	SDL_Delay(500);
	ftm->dead=1;
	/*Fantome controllé par IA pour revenir à la position initiale
	n'est util que si le fantome était controllé par un humain*/
	ftm->controllerFonction = ia_controller;
	ftm->speed = 10;
	ftm->counter=SDL_GetTicks();
}

void delete_ghosts(Fantome *ftm)
{
	int i,j;
	for (i=0; i<NB_GHOST; i++)
	{
		for(j=0; j<16; j++) free(ftm[i].image[j]);
	}
}
