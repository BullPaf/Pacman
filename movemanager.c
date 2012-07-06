#include "movemanager.h"
/*Le coeur du probleme!*/


/*Quand un humain control un personnage*/
void human_controller(Input in, config cfg, int controller, SDL_Rect *position, int *cur_direction, int *nb_keys, int *speed, int *num_image, SDL_Rect target)
{
	int new_direction;
	if(in.key[cfg.players[controller].down]) new_direction=BAS;
	else if(in.key[cfg.players[controller].up]) new_direction=HAUT;
	else if (in.key[cfg.players[controller].left]) new_direction=GAUCHE;
	else if (in.key[cfg.players[controller].right]) new_direction=DROITE;
	else new_direction=*cur_direction;
	//Si on peut se deplacer dans la nouvelle direction
	if(can_move(*position, new_direction, *cur_direction, nb_keys))
	{
		move(position, new_direction, *speed); //Eh ben on bouge
		*cur_direction = new_direction;
	}
	//Sinon si on peut continuer dans l'ancienne direction
	else if(can_move(*position, *cur_direction, *cur_direction, nb_keys)) move(position, *cur_direction, *speed);
	//Bah sinon on peut pas bouger
}

/*L'IA*/
void ia_controller(Input in, config cfg, int controller, SDL_Rect *position, int *cur_direction, int *nb_keys, int *speed, int *num_image, SDL_Rect target)
{
	int new_direction;
	/*Si c'est une intersection on a le droit de cherche à nouveau
	 * où aller*/
	if(in_intersection(*position, *cur_direction))
	{
		//Cherche la meilleur direction à prendre
		new_direction = find_direction(*position, *cur_direction, target, nb_keys);
		move(position, new_direction, *speed);
		*cur_direction = new_direction;
	}
	/*Si c'est pas une intersection on continuer dans la direction courante... enfin si on peut*/
	else if(can_move(*position, *cur_direction, *cur_direction, nb_keys))
		move(position, *cur_direction, *speed);
	//Si on peut pas on cherche une nouvelle direction
	else
	{
		new_direction = find_direction(*position, *cur_direction, target, nb_keys);
		move(position, new_direction, *speed);
		*cur_direction = new_direction;
	}
}

/*Si on peut bouger renvoie 1
 * Sinon renvoie 0
 * Test si new_direction est possible
 * De maniere générale si on est pas pile dans une case on ne peut qu'aller dans la direction opposée
 * Si on ne rencontre pas de mur dans la case suivante c'est bon
 * Si c'est un mur avec un cadenas et que l'on a une clé c'est bon aussi*/
int can_move(SDL_Rect pos, int new_direction, int cur_direction, int *key)
{
	int case_x = pos.x / BLOCK_SIZE, case_y = pos.y / BLOCK_SIZE;
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
	if(case_y >= NB_BLOCKS_HAUTEUR || case_y < 0 || case_x >= NB_BLOCKS_LARGEUR || case_x < 0) return 0;
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
			if(pos->x+BLOCK_SIZE > WIDTH) pos->x = -BLOCK_SIZE+speed;
			break;
		case GAUCHE:
			pos->x -= speed;
			if(pos->x+BLOCK_SIZE <= 0) pos->x = WIDTH-speed;
			break;
		case HAUT:
			pos->y -= speed;
			if(pos->y+BLOCK_SIZE <= 0) pos->y = HEIGHT-speed;
			break;
		case BAS:
			pos->y += speed;
			if(pos->y+BLOCK_SIZE > HEIGHT) pos->y = -BLOCK_SIZE+speed;
			break;
		default: break;
	}
}

/*Fonction assez naïve qui cherche la meilleur direction à prendre
 * en fonction de l'objectif
 * Amélioration: implémenter une recherche de plus cours chemin*/
int find_direction(SDL_Rect pos, int cur_direction, SDL_Rect target, int *nb_keys)
{
	/*Si on a un objectif*/
	if(target.x != -1 && target.y != -1)
	{
		SDL_Rect ftm_case, target_case;
		ftm_case = get_case(pos, cur_direction);
		target_case = get_case(target, HAUT);
		if(ftm_case.x > target_case.x)
		{
			if(can_move(pos, GAUCHE, cur_direction, nb_keys) && cur_direction != DROITE) return GAUCHE;
		}
		else if(ftm_case.x < target_case.x)
		{
			if(can_move(pos, DROITE, cur_direction, nb_keys) && cur_direction != GAUCHE) return DROITE;
		}
		if(ftm_case.y > target_case.y)
		{
			if(can_move(pos, HAUT, cur_direction, nb_keys) && cur_direction != BAS) return HAUT;
		}
		else if(ftm_case.y < target_case.y)
		{
			if(can_move(pos, BAS, cur_direction, nb_keys) && cur_direction != HAUT) return BAS;
		}
	}
	//Sinon on navigue au hasard
	int rand_dir = rand()%4;
	while( !can_move(pos, rand_dir, cur_direction, nb_keys) ) rand_dir = rand()%4;
	return rand_dir;
}
