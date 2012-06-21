#include "fantome.h"

void init_ghosts(Fantome *ftm)
{
	int i,j;
	char img[32];
	for (i=0; i<NB_GHOST_BLOCKS; i++)
	{
		for(j=0; j<8; j++)
		{
			sprintf(img, "image/ghosts/%d.png", i*8+j);
			if( (ftm[i].image[j] = IMG_Load(img)) == NULL )
			{
				fprintf(stderr, "Erreur chargement texture n°%d de Pacman\n", i);
				exit(EXIT_FAILURE);
			}
		}
		ftm[i].image[j] = IMG_Load("image/ghosts/32.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/33.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/34.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/35.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/36.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/37.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/38.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/39.png");
		ftm[i].position.x    = (GHOST_START_X[i])*BLOCK_SIZE;
		ftm[i].position.y    = (GHOST_START_Y[i])*BLOCK_SIZE;
		ftm[i].cur_direction = 1;
		ftm[i].num_image     = (ftm[i].cur_direction-1)*2;
		ftm[i].invinsible    = 1;
		ftm[i].counter       = 0;
		LEVEL[GHOST_START_Y[i]][GHOST_START_X[i]].type=RIEN;
	}
}

void ghost_restart(Fantome *ftm, int i)
{
	ftm->position.x    = (GHOST_START_X[i])*BLOCK_SIZE;
	ftm->position.y    = (GHOST_START_Y[i])*BLOCK_SIZE;
	ftm->cur_direction = 1;
	ftm->num_image     = (ftm->cur_direction-1)*2;
	ftm->invinsible    = 1;
	ftm->counter       = 0;
}

//Pour afficher Pacman
void affiche_fantomes(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++) SDL_BlitSurface(ftm[i].image[ftm[i].num_image], NULL, screen, &(ftm[i].position));
}

int find_direction(Fantome f)
{
	return rand()%4+1;
}

/*Deplacements aléatoires*/
void deplace_fantomes(Fantome *ftm, int *new_directions)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++)
	{
		if (new_directions[i] == 0) new_directions[i] = ftm[i].cur_direction;
		if(can_move(ftm[i].position, new_directions[i], ftm[i].cur_direction)) move(&(ftm[i].position), new_directions[i]);
		else
		{
			while( !can_move(ftm[i].position, new_directions[i], ftm[i].cur_direction) ) new_directions[i] = find_direction(ftm[i]);
			move(&(ftm[i].position), new_directions[i]);
			ftm[i].cur_direction = new_directions[i];
			//Si le fantome est invincible, on charge l'image correspondante à la nouvelle direction
			if(ftm[i].invinsible) ftm[i].num_image=(ftm[i].cur_direction-1)*2;
		}
		if(!(ftm[i].invinsible))
		{
			int tempsEcoule = SDL_GetTicks()-ftm[i].counter;
			if(tempsEcoule < 5000 && tempsEcoule > 3000) //Fantome bientot invulnerable
			{
				if(ftm[i].num_image==10) ftm[i].num_image=9;
				else ftm[i].num_image=10;
				continue;
			}
			else if (tempsEcoule >= 5000) //Fantome redevient invulnérable
			{
				ftm[i].invinsible=1;
				//On charge l'image correspondante à la direction en cours
				ftm[i].num_image=(ftm[i].cur_direction-1)*2;
			}
		}
		//Permutation des images pour effets de mouvements
		if(ftm[i].num_image%2==0) ftm[i].num_image+=1;
		else ftm[i].num_image-=1;
	}
}

void ghost_death(Fantome* ftm, int i)
{
	ghost_restart(ftm, i);
}
