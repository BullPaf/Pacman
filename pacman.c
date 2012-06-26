#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"

void init_pacman(Pacman *pac)
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
	pac->num_image     = (pac->cur_direction*2)-2;
	pac->counter       = 0;
	pac->nb_lives      = 2;
	pac->speed         = 4;
	LEVEL[PAC_START_Y][PAC_START_X].type=RIEN;
}

void pac_restart(Pacman *pac)
{
	pac->nb_lives--;
	pac->position.x = PAC_START_X*BLOCK_SIZE;
	pac->position.y = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->num_image = (pac->cur_direction*2)-2;
}

//Pour afficher Pacman
void affiche_pacman(Pacman *pac)
{
	SDL_BlitSurface(pac->image[pac->num_image], NULL, screen, &pac->position);
}

void deplace_pacman(Pacman *pac, int new_direction)
{
	//Si on peut se deplacer dans la nouvelle direction
	if(new_direction && can_move(pac->position, new_direction, pac->cur_direction))
	{
		move(&(pac->position), new_direction, pac->speed);
		if(new_direction != pac->cur_direction)
		{
			pac->cur_direction = new_direction;
			pac->num_image = (pac->cur_direction*2)-2;
		}
		if( (pac->num_image)%2==0 ) pac->num_image+=1;
		else pac->num_image -= 1;
	}
	//Sinon si on peut continuer dans l'ancienne direction
	else if(can_move(pac->position, pac->cur_direction, pac->cur_direction))
	{
		move(&(pac->position), pac->cur_direction, pac->speed);
		if( (pac->num_image)%2==0 ) pac->num_image+=1;
		else pac->num_image -= 1;
	}
	/*if(pac->speed != 4)
	{
		int tempsEcoule = SDL_GetTicks()-pac->counter;
		if (tempsEcoule >= 5000) //Fantome redevient invulnérable
		{
			pac->speed      = 4;
			if(pac->position.x % 4 != 0) pac->position.x+=2;
			if(pac->position.y % 4 != 0) pac->position.y+=2;
		}
	}*/
}

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
	pac_restart(pac);
}
