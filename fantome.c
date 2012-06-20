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
			ftm[i].image[j] = IMG_Load(img);
		}
		ftm[i].position.x = (GHOST_START_X[i])*BLOCK_SIZE;
		ftm[i].position.y = (GHOST_START_Y[i])*BLOCK_SIZE;
		ftm[i].cur_direction = 1;
		ftm[i].num_image=(ftm[i].cur_direction-1)*2;
		LEVEL[GHOST_START_Y[i]][GHOST_START_X[i]].type=RIEN;
	}
}

//Pour afficher Pacman
void affiche_fantomes(Fantome *ftm, int visible)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++)
	{
		if (!visible) SDL_FillRect(screen, &(ftm[i].position), noir);
		else SDL_BlitSurface(ftm[i].image[ftm[i].num_image], NULL, screen, &(ftm[i].position));
	}
}

int find_direction(Fantome f)
{
	//srand(time(NULL));
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
			ftm[i].num_image=(ftm[i].cur_direction-1)*2;
		}
		ftm[i].image[0]=ftm[i].image[ftm[i].num_image];
		if(ftm[i].num_image%2==0) ftm[i].num_image+=1;
		else ftm[i].num_image-=1;
	}
}
