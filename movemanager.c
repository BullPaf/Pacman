#include "movemanager.h"

//Si on peut bouger
int can_move(SDL_Rect pos, int new_direction, int cur_direction, int *key)
{
	int case_x = pos.x / BLOCK_SIZE, case_y = pos.y / BLOCK_SIZE;
	//fprintf(stderr, "Case x = %d && Case y = %d\n", case_x, case_y);
	switch (new_direction)
	{
		case DROITE: //Vers la droite
			if (!dans_case(pos)) {//Si on est entre deux cases
				if( (cur_direction==GAUCHE) || (cur_direction==DROITE) ) return 1;
			}
			else if(case_x == NB_BLOCKS_LARGEUR-1) return 1; //Si on on est contre le bord droit
			else if(LEVEL[case_y][case_x+1].type != MUR) return 1;
			else if(LEVEL[case_y][case_x+1].elt_type >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x+1, case_y);
				remove_bloc(case_y, case_x+1);
				(*key)--;
				return 1;
			}
			break;
		case GAUCHE: //Vers la gauche
			if(!dans_case(pos)) {
				if( (cur_direction==GAUCHE) || (cur_direction==DROITE) ) return 1;
			}
			else if(case_x == 0) return 1; //Si on on est contre le bord gauche
			else if(LEVEL[case_y][case_x-1].type != MUR) return 1;
			else if(LEVEL[case_y][case_x-1].elt_type >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x-1, case_y);
				remove_bloc(case_y, case_x-1);
				(*key)--;
				return 1;
			}
			break;
		case HAUT: //Vers le haut
			if(!dans_case(pos)) {
				if( (cur_direction==HAUT) || (cur_direction==BAS) ) return 1;
			}
			else if(case_y == 0) return 1; //Si on on est contre le bord haut
			else if(LEVEL[case_y-1][case_x].type != MUR) return 1;
			else if(LEVEL[case_y-1][case_x].elt_type >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x, case_y-1);
				remove_bloc(case_y-1, case_x);
				(*key)--;
				return 1;
			}
			break;
		case BAS: //Vers le bas
			if(!dans_case(pos)) {
				if( (cur_direction==HAUT) || (cur_direction==BAS) ) return 1;
			}
			else if(case_y == NB_BLOCKS_HAUTEUR-1) return 1; //Si on on est contre le bord bas
			else if(LEVEL[case_y+1][case_x].type != MUR) return 1;
			else if(LEVEL[case_y+1][case_x].elt_type >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x, case_y+1);
				remove_bloc(case_y+1, case_x);
				(*key)--;
				return 1;
			}
			break;
		default: break;
	}
	return 0;
}

//Si on est sur une intersection de chemins
int in_intersection(SDL_Rect pos, int direction)
{
	int case_x = pos.x / BLOCK_SIZE, case_y = pos.y / BLOCK_SIZE;
	//if(case_x==0 || case_x == NB_BLOCKS_LARGEUR-1 || case_y == 0 || case_y == NB_BLOCKS_HAUTEUR-1) return 1;
	if(direction == DROITE || direction == GAUCHE)
	{
		if(dans_case(pos) && (LEVEL[case_y-1][case_x].type != MUR || LEVEL[case_y+1][case_x].type != MUR)) return 1;
	}
	else
	{
		if(dans_case(pos) && (LEVEL[case_y][case_x+1].type != MUR || LEVEL[case_y][case_x-1].type != MUR)) return 1;
	}
	return 0;
}

//Deplace
void move(SDL_Rect *pos, int direction, int speed)
{
	switch (direction)
	{
		case DROITE:
			pos->x += speed;
			if(pos->x+BLOCK_SIZE > WIDTH) pos->x = -BLOCK_SIZE;
			break;
		case GAUCHE:
			pos->x -= speed;
			if(pos->x+BLOCK_SIZE <= 0) pos->x = WIDTH;
			break;
		case HAUT:
			pos->y -= speed;
			if(pos->y+BLOCK_SIZE <= 0) pos->y = HEIGHT;
			break;
		case BAS:
			pos->y += speed;
			if(pos->y+BLOCK_SIZE > HEIGHT) pos->y = -BLOCK_SIZE;
			break;
		default: break;
	}
}
