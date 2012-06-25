#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"

void init_pacman(Pacman *pac)
{
	char img[32];
	int i;
	for(i=1; i<20; i++)
	{
		sprintf(img, "image/pacman/%d.png", i);
		if( (pac->image[i] = IMG_Load(img)) == NULL )
		{
			fprintf(stderr, "Erreur chargement texture n°%d de Pacman\n", i);
			exit(EXIT_FAILURE);
		}
	}
	pac->position.x = PAC_START_X*BLOCK_SIZE;
	pac->position.y = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->num_image = pac->cur_direction;
	//pac->image[0]=pac->image[pac->cur_direction];
	LEVEL[PAC_START_Y][PAC_START_X].type=RIEN;
	pac->nb_lives = 2;
	pac->speed = 4;
}

void pac_restart(Pacman *pac)
{
	pac->nb_lives--;
	pac->position.x = PAC_START_X*BLOCK_SIZE;
	pac->position.y = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->num_image = pac->cur_direction;
	//pac->image[0]=pac->image[pac->cur_direction];
}

//Pour afficher Pacman
void affiche_pacman(Pacman *pac)
{
	SDL_BlitSurface(pac->image[pac->num_image], NULL, screen, &pac->position);
}

void deplace_pacman(Pacman *pac, int new_direction)
{
	if (new_direction == 0) new_direction = pac->cur_direction;
	//Si on peut se deplacer dans la nouvelle direction
	if(can_move(pac->position, new_direction, pac->cur_direction))
	{
		move(&(pac->position), new_direction, pac->speed);
		pac->cur_direction = new_direction;
		pac->num_image = pac->cur_direction;
		//pac->image[0]=pac->image[pac->cur_direction];
	}
	//Sinon si on peut continuer dans l'ancienne direction
	else if(can_move(pac->position, pac->cur_direction, pac->cur_direction))
		move(&(pac->position), pac->cur_direction, pac->speed);
	if( (pac->num_image)%2==0 ) pac->num_image-=15;
	else pac->num_image += 15;
}
