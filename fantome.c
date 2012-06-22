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
		ftm[i].image[j] = IMG_Load("image/ghosts/32.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/33.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/34.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/35.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/36.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/37.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/38.png");
		j++;
		ftm[i].image[j] = IMG_Load("image/ghosts/39.png");
		ftm[i].position.x    = (GHOST_START_X[i])*BLOCK_SIZE;
		ftm[i].position.y    = (GHOST_START_Y[i])*BLOCK_SIZE;
		ftm[i].cur_direction = 1;
		ftm[i].num_image     = (ftm[i].cur_direction-1)*2;
		ftm[i].invinsible    = 1;
		ftm[i].dead          = 0;
		ftm[i].counter       = 0;
		LEVEL[GHOST_START_Y[i]][GHOST_START_X[i]].type=RIEN;
	}
}

void ghost_restart(Fantome *ftm, int i)
{
	ftm->position.x    = (GHOST_START_X[i])*BLOCK_SIZE;
	ftm->position.y    = (GHOST_START_Y[i])*BLOCK_SIZE;
	ftm->cur_direction = rand()%4+1;;
	ftm->num_image     = (ftm->cur_direction-1)*2;
	ftm->dead          = 0;
	ftm->counter       = 0;
}

//Pour afficher Pacman
void affiche_fantomes(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++) SDL_BlitSurface(ftm[i].image[ftm[i].num_image], NULL, screen, &(ftm[i].position));
}

int find_direction(Fantome f, SDL_Rect target_pos, int target_dir)
{
	SDL_Rect ftm_case, target_case;
	ftm_case = get_case(f.position, f.cur_direction);
	target_case = get_case(target_pos, target_dir);
	if(ftm_case.x > target_case.x)
	{
		if(can_move(f.position, GAUCHE, f.cur_direction)) return GAUCHE;
	}
	else if(ftm_case.x < target_case.x)
	{
		if(can_move(f.position, DROITE, f.cur_direction)) return DROITE;
	}
	if(ftm_case.y > target_case.y)
	{
		if(can_move(f.position, HAUT, f.cur_direction)) return HAUT;
	}
	else if(ftm_case.y < target_case.y)
	{
		if(can_move(f.position, BAS, f.cur_direction)) return BAS;
	}
	int rand_dir = rand()%4+1;
	while( !can_move(f.position, rand_dir, f.cur_direction) ) rand_dir = rand()%4+1;
	return rand_dir;
}

/*Deplacements aléatoires*/
void deplace_fantomes(Fantome *ftm, int *new_directions, SDL_Rect target, int target_dir)
{
	if (*new_directions != ftm->cur_direction && !ftm->dead) *new_directions = ftm->cur_direction;
	if(can_move(ftm->position, *new_directions, ftm->cur_direction)) move(&(ftm->position), *new_directions);
	else
	{
		*new_directions = find_direction(*ftm, target, target_dir);
		move(&(ftm->position), *new_directions);
		ftm->cur_direction = *new_directions;
		if(ftm->invinsible) ftm->num_image=(ftm->cur_direction-1)*2;
	}
	if(!(ftm->invinsible))
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
			ftm->invinsible=1;
			//On charge l'image correspondante à la direction en cours
			ftm->num_image=(ftm->cur_direction-1)*2;
		}
	}
	if(ftm->dead) ftm->num_image=11+ftm->cur_direction;
	//Permutation des images pour effets de mouvements
	else if(ftm->num_image%2==0) ftm->num_image+=1;
	else ftm->num_image -= 1;
}

void ghost_death(Fantome* ftm, int i, Pacman *pac)
{
	SDL_Rect start;
	start.x = (GHOST_START_X[i])*BLOCK_SIZE;
	start.y = (GHOST_START_Y[i])*BLOCK_SIZE;
	ftm[i].dead=1;
	ftm[i].invinsible=1;
	int dir=find_direction(ftm[i], start, 0);
	ftm[i].cur_direction = dir;
	int timer=SDL_GetTicks(), time_elapsed=0;
	while ( (ftm[i].position.x != start.x || ftm[i].position.y != start.y) && time_elapsed < 7500)
	{
		SDL_Delay(5);
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		draw_level();
		draw_lives(pac);
		draw_score();
		deplace_fantomes(ftm+i, &dir, start, 0);
		affiche_pacman(pac);
		affiche_fantomes(ftm);
		SDL_Flip(screen);
		time_elapsed = SDL_GetTicks()-timer;
	}
	ghost_restart(ftm+i, i);
}
