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

int f_dans_case(Fantome f)
{
	if( (f.position.x % BLOCK_SIZE==0) && (f.position.y % BLOCK_SIZE==0) ) return 1;
	else return 0;
}

int can_move(Fantome ftm, int new_direction)
{
	int case_x = ftm.position.x / BLOCK_SIZE, case_y = ftm.position.y / BLOCK_SIZE;
	switch (new_direction)
	{
		case DROITE: //Vers la droite
			if (!f_dans_case(ftm)) {//Si on est entre deux cases
				if( (ftm.cur_direction==GAUCHE) || (ftm.cur_direction==DROITE) ) return 1;
			}
			else if(LEVEL[case_y][case_x+1].type != MUR) return 1;
			break;
		case GAUCHE: //Vers la gauche
			if(!f_dans_case(ftm)) {
				if( (ftm.cur_direction==GAUCHE) || (ftm.cur_direction==DROITE) ) return 1;
			}
			else if(LEVEL[case_y][case_x-1].type != MUR) return 1;
			break;
		case HAUT: //Vers le haut
			if(!f_dans_case(ftm)) {
				if( (ftm.cur_direction==HAUT) || (ftm.cur_direction==BAS) ) return 1;
			}
			else if(LEVEL[case_y-1][case_x].type != MUR) return 1;
			break;
		case BAS: //Vers le bas
			if(!f_dans_case(ftm)) {
				if( (ftm.cur_direction==HAUT) || (ftm.cur_direction==BAS) ) return 1;
			}
			else if(LEVEL[case_y+1][case_x].type != MUR) return 1;
			break;
		default: break;
	}
	return 0;
}

void move(Fantome *ftm, int direction)
{
	switch (direction)
	{
		case DROITE:
			ftm->position.x += STEP;
			break;
		case GAUCHE:
			ftm->position.x -= STEP;
			break;
		case HAUT:
			ftm->position.y -= STEP;
			break;
		case BAS:
			ftm->position.y += STEP;
			break;
		default: break;
	}
}

void deplace_fantomes(Fantome *ftm, int *new_directions)
{
	int i; //case_x, case_y, move;
	for(i=0; i<NB_GHOST_BLOCKS; i++)
	{
		if (new_directions[i] == 0) new_directions[i] = ftm[i].cur_direction;
		if(can_move(ftm[i], new_directions[i]))
		{
			move(&ftm[i], new_directions[i]);
		}
		else
		{
			while( !can_move(ftm[i], new_directions[i]) ) new_directions[i] = find_direction(ftm[i]);
			move(&ftm[i], new_directions[i]);
			ftm[i].cur_direction = new_directions[i];
			ftm[i].num_image=(ftm[i].cur_direction-1)*2;
		}
		ftm[i].image[0]=ftm[i].image[ftm[i].num_image];
		if(ftm[i].num_image%2==0) ftm[i].num_image+=1;
		else ftm[i].num_image-=1;
	}
}
