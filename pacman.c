#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"

void init_pacman(Pacman *pac)
{
	int i;
	pac->image[GAUCHE] = IMG_Load("./image/pacman/1.png");
	pac->image[HAUT] = IMG_Load("./image/pacman/2.png");
	pac->image[DROITE] = IMG_Load("./image/pacman/3.png");
	pac->image[BAS] = IMG_Load("image/pacman/4.png");
	//for(i=1; i<5; i++) SDL_SetColorKey(pac->image[i], SDL_SRCCOLORKEY, SDL_MapRGB(pac->image[i]->format, 0, 0, 0));
	pac->position.x = 1*BLOCK_SIZE;
	pac->position.y = 1*BLOCK_SIZE;
	pac->cur_direction = 3;
	pac->nb_lives = 2;
	pac->score=0;
}

//Pour afficher Pacman
void affiche_pacman(Pacman *pac, int visible)
{
	if (!visible) SDL_FillRect(screen, &pac->position, noir);
	else SDL_BlitSurface(pac->image[0], NULL, screen, &pac->position); //SDL_FillRect(screen, &pac->position, rouge);
}

int dans_case(Pacman *pac)
{
	if( (pac->position.x % BLOCK_SIZE==0) && (pac->position.y % BLOCK_SIZE==0) ) return 1;
	else return 0;
}

void deplace_pacman(Pacman *pac, int new_direction)
{
	//Si on a rien récupéré
	if (new_direction == 0) new_direction = pac->cur_direction;
	int case_x = pac->position.x / BLOCK_SIZE, case_y = pac->position.y / BLOCK_SIZE, can_move=0;
	switch (new_direction)
	{
		case DROITE: //Vers la droite
			if (!dans_case(pac)) //Si on est entre deux cases
			{
				if( (pac->cur_direction==GAUCHE) || (pac->cur_direction==DROITE) )
				{
					pac->position.x += STEP;
					can_move=1;
				}
			}
			else if(LEVEL[case_y][case_x+1].type != MUR)
			{
				pac->position.x += STEP;
				can_move=1;
			}
			break;
		case GAUCHE: //Vers la gauche
			if(!dans_case(pac))
			{
				if( (pac->cur_direction==GAUCHE) || (pac->cur_direction==DROITE) )
				{
					pac->position.x -= STEP;
					can_move=1;
				}
			}
			else if(LEVEL[case_y][case_x-1].type != MUR)
			{
				pac->position.x -= STEP;
				can_move=1;
			}
			break;
		case HAUT: //Vers le haut
			if(!dans_case(pac))
			{
				if( (pac->cur_direction==HAUT) || (pac->cur_direction==BAS) )
				{
					pac->position.y -= STEP;
					can_move=1;
				}
			}
			else if(LEVEL[case_y-1][case_x].type != MUR)
			{
				pac->position.y -= STEP;
				can_move=1;
			}
			break;
		case BAS: //Vers le bas
			if(!dans_case(pac))
			{
				if( (pac->cur_direction==HAUT) || (pac->cur_direction==BAS) )
				{
					pac->position.y += STEP;
					can_move=1;
				}
			}
			else if(LEVEL[case_y+1][case_x].type != MUR)
			{
				pac->position.y += STEP;
				can_move=1;
			}
			break;
		default: break;
	}
	if(!can_move) //Si on ne peut pas changer de direction
	{
		if(pac->cur_direction==GAUCHE)
		{
			if(LEVEL[case_y][case_x-1].type != MUR) pac->position.x -= STEP;
			else if(!dans_case(pac)) pac->position.x -= STEP;
		}
		else if(pac->cur_direction==DROITE)
		{
			if(LEVEL[case_y][case_x+1].type != MUR) pac->position.x += STEP;
			else if(!dans_case(pac)) pac->position.x += STEP;
		}
		else if(pac->cur_direction==HAUT)
		{
			if(LEVEL[case_y-1][case_x].type != MUR) pac->position.y -= STEP;
			else if(!dans_case(pac)) pac->position.y -= STEP;
		}
		else if(pac->cur_direction==BAS)
		{
			if(LEVEL[case_y+1][case_x].type != MUR) pac->position.y += STEP;
			else if(!dans_case(pac)) pac->position.y += STEP;
		}
	}
	else
	{
		pac->cur_direction = new_direction;
		pac->image[0]=pac->image[pac->cur_direction];
	}
}

