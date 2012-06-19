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
		ftm[i].cur_direction = 0;
	}
}

//Pour afficher Pacman
void affiche_fantome(Fantome *ftm, int visible)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++)
	{
		if (!visible) SDL_FillRect(screen, &(ftm[i].position), noir);
		else SDL_BlitSurface(ftm[i].image[ftm[i].cur_direction], NULL, screen, &(ftm[i].position));
	}
	
}
