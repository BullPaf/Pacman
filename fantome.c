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
		for(j=8; j<16; j++)
		{
			sprintf(img, "image/ghosts/%d.png", 24+j);
			if( (ftm[i].image[j] = IMG_Load(img)) == NULL )
			{
				fprintf(stderr, "Erreur chargement texture n°%d de Pacman\n", i);
				exit(EXIT_FAILURE);
			}
		}
		ftm[i].start.x       = (GHOST_START_X[i])*BLOCK_SIZE;
		ftm[i].start.y       = (GHOST_START_Y[i])*BLOCK_SIZE;
		ftm[i].position.x    = ftm[i].start.x;
		ftm[i].position.y    = ftm[i].start.y;
		ftm[i].speed         = 4;
		ftm[i].cur_direction = 1;
		ftm[i].num_image     = (ftm[i].cur_direction-1)*2;
		ftm[i].invinsible    = 1;
		ftm[i].dead          = 0;
		ftm[i].counter       = 0;
		LEVEL[GHOST_START_Y[i]][GHOST_START_X[i]].type=RIEN;
	}
}

void ghost_restart(Fantome *ftm)
{
	ftm->position.x    = ftm->start.x;
	ftm->position.y    = ftm->start.y;
	ftm->cur_direction = rand()%4+1;;
	ftm->num_image     = (ftm->cur_direction-1)*2;
	ftm->speed         = 4;
	ftm->invinsible    = 1;
	ftm->dead          = 0;
	ftm->counter       = 0;
}

//Pour afficher Pacman
void affiche_fantomes(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++) SDL_BlitSurface(ftm[i].image[ftm[i].num_image], NULL, screen, &(ftm[i].position));
}

/*Fonction assez naïve qui cherche la meilleur direction à prendre
 * en fonction de l'objectif*/
int find_direction(Fantome f, SDL_Rect target_pos, int target_dir)
{
	if(f.dead)
	{
		SDL_Rect ftm_case, target_case;
		ftm_case = get_case(f.position, f.cur_direction);
		target_case = get_case(target_pos, target_dir);
		if(ftm_case.x > target_case.x)
		{
			if(can_move(f.position, GAUCHE, f.cur_direction) && f.cur_direction != DROITE) return GAUCHE;
		}
		else if(ftm_case.x < target_case.x)
		{
			if(can_move(f.position, DROITE, f.cur_direction) && f.cur_direction != GAUCHE) return DROITE;
		}
		if(ftm_case.y > target_case.y)
		{
			if(can_move(f.position, HAUT, f.cur_direction) && f.cur_direction != BAS) return HAUT;
		}
		else if(ftm_case.y < target_case.y)
		{
			if(can_move(f.position, BAS, f.cur_direction) && f.cur_direction != HAUT) return BAS;
		}
	}
	int rand_dir = rand()%4+1;
	while( !can_move(f.position, rand_dir, f.cur_direction) ) rand_dir = rand()%4+1;
	return rand_dir;
}

/*Deplacements aléatoires*/
void deplace_fantomes(Fantome *ftm, int *new_directions, SDL_Rect target, int target_dir)
{
	if(ftm->dead)
	{
		target.x=ftm->start.x;
		target.y=ftm->start.y;
	}
	if (*new_directions != ftm->cur_direction && !ftm->dead) *new_directions = ftm->cur_direction;
	if(in_intersection(ftm->position, ftm->cur_direction))
	{
		*new_directions = find_direction(*ftm, target, target_dir);
		move(&(ftm->position), *new_directions, ftm->speed);
		ftm->cur_direction = *new_directions;
		if(ftm->invinsible) ftm->num_image=(ftm->cur_direction-1)*2;
	}
	else if(can_move(ftm->position, *new_directions, ftm->cur_direction)) move(&(ftm->position), *new_directions, ftm->speed);
	else
	{
		*new_directions = find_direction(*ftm, target, target_dir);
		move(&(ftm->position), *new_directions, ftm->speed);
		ftm->cur_direction = *new_directions;
		if(ftm->invinsible) ftm->num_image=(ftm->cur_direction-1)*2;
	}
	if( !(ftm->invinsible) && !(ftm->dead) )
	{
		int tempsEcoule = SDL_GetTicks()-ftm->counter;
		if(tempsEcoule < 7000 && tempsEcoule > 5000) //Fantome bientot invulnerable
		{
			if(ftm->num_image==10) ftm->num_image=9;
			else ftm->num_image=10;
			return;
		}
		else if (tempsEcoule >= 7000) //Fantome redevient invulnérable
		{
			ftm->invinsible = 1;
			ftm->speed      = 4;
			if(ftm->position.x % 4 != 0) ftm->position.x+=2;
			if(ftm->position.y % 4 != 0) ftm->position.y+=2;
			//On charge l'image correspondante à la direction en cours
			ftm->num_image=(ftm->cur_direction-1)*2;
		}
	}
	if(ftm->dead)
	{
		int tempsEcoule = SDL_GetTicks()-ftm->counter;
		if (tempsEcoule >= 7000 || (ftm->position.x == ftm->start.x && ftm->position.y == ftm->start.y) ) ghost_restart(ftm);
		ftm->num_image=11+ftm->cur_direction;
	}
	//Permutation des images pour effets de mouvements
	else if(ftm->num_image%2==0) ftm->num_image+=1;
	else ftm->num_image -= 1;
}

void ghost_death(Fantome* ftm)
{
	POINT p1;
	SDL_Rect ftm_case = get_case(ftm->position, ftm->cur_direction);
	p1.x=(ftm_case.x+1)*BLOCK_SIZE; p1.y=(ftm_case.y+1)*BLOCK_SIZE;
	aff_pol("BRAVO BB!", FONT_SIZE, p1, blanc);
	SDL_Flip(screen);
	SDL_Delay(500);
	ftm->dead=1;
	ftm->speed = 10;
	ftm->position.x=ftm_case.x*BLOCK_SIZE;
	ftm->position.y=ftm_case.y*BLOCK_SIZE;
	ftm->counter=SDL_GetTicks();
}
