#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"

void init_pacman(Pacman *pac)
{
	pac->image[GAUCHE] = IMG_Load("./image/pacman/4.png");
	pac->image[HAUT] = IMG_Load("./image/pacman/1.png");
	pac->image[DROITE] = IMG_Load("./image/pacman/2.png");
	pac->image[BAS] = IMG_Load("image/pacman/3.png");
	pac->position.x = PAC_START_X*BLOCK_SIZE;
	pac->position.y = PAC_START_Y*BLOCK_SIZE;
	pac->cur_direction = PAC_START_DIRECTION;
	pac->image[0]=pac->image[pac->cur_direction];
	LEVEL[PAC_START_Y][PAC_START_X].type=RIEN;
	pac->nb_lives = 2;
}

//Pour afficher Pacman
void affiche_pacman(Pacman *pac, int visible)
{
	if (!visible) SDL_FillRect(screen, &pac->position, noir);
	else SDL_BlitSurface(pac->image[0], NULL, screen, &pac->position); //SDL_FillRect(screen, &pac->position, rouge);
}

void deplace_pacman(Pacman *pac, int new_direction)
{
	if (new_direction == 0) new_direction = pac->cur_direction;
	//Si on peut se deplacer dans la nouvelle direction
	if(can_move(pac->position, new_direction, pac->cur_direction))
	{
		move(&(pac->position), new_direction);
		pac->cur_direction = new_direction;
		pac->image[0]=pac->image[pac->cur_direction];
	}
	//Sinon si on peut continuer dans l'ancienne direction
	else if(can_move(pac->position, pac->cur_direction, pac->cur_direction))
		move(&(pac->position), pac->cur_direction);
}

