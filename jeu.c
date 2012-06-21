#include "jeu.h"

int jouer()
{
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	Pacman pac;
	Fantome ftm[NB_GHOST_BLOCKS];
	SDL_Event event;
	int i, pac_new_direction=0, ghosts_new_directions[NB_GHOST_BLOCKS];
	init_level();
	init_blocks();
	load_level();
	init_pacman(&pac);
	init_ghosts(ftm);
	for(i=0; i<NB_GHOST_BLOCKS; i++) ghosts_new_directions[i]=0;
	while(POINTS)
	{
		SDL_Delay(40);
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT : exit(EXIT_SUCCESS);
				case SDL_KEYDOWN :
					if (event.key.keysym.sym == SDLK_ESCAPE) return 0;
					else if (event.key.keysym.sym == SDLK_UP) pac_new_direction=HAUT;
					else if (event.key.keysym.sym == SDLK_DOWN) pac_new_direction=BAS;
					else if (event.key.keysym.sym == SDLK_LEFT) pac_new_direction=GAUCHE;
					else if (event.key.keysym.sym == SDLK_RIGHT) pac_new_direction=DROITE;
					break;
				default:
					pac_new_direction=0;
					break;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		draw_level();
		draw_lives(&pac);
		draw_score();
		deplace_pacman(&pac, pac_new_direction);
		deplace_fantomes(ftm, ghosts_new_directions);
		affiche_pacman(&pac, 1);
		affiche_fantomes(ftm, 1);
		action(&pac, ftm);
		SDL_Flip(screen);
	}
	return 1;
}

void action(Pacman *pac, Fantome *ftm)
{
	int case_x,case_y, i;
	switch(pac->cur_direction)
	{
		case DROITE: //Vers la droite
			case_x = (pac->position.x+BLOCK_SIZE-1)/BLOCK_SIZE;
			case_y = pac->position.y/BLOCK_SIZE;
			break;
		case BAS:
			case_x = pac->position.x/BLOCK_SIZE;
			case_y = (pac->position.y+BLOCK_SIZE-1)/BLOCK_SIZE;
			break;
		default:
			case_x = pac->position.x/BLOCK_SIZE;
			case_y = pac->position.y/BLOCK_SIZE;
			break;
	}
	for(i=0; i<NB_GHOST_BLOCKS; i++) check_colision(pac, ftm[i]);
	if(LEVEL[case_y][case_x].type == BONUS)
	{
		if(LEVEL[case_y][case_x].elt_type[0]==0) {
			SCORE+=100;
			POINTS--;
		}
		else if(LEVEL[case_y][case_x].elt_type[0]==1) {
			if(pac->nb_lives<5) pac->nb_lives++;
			else SCORE+=1000;
		}
		LEVEL[case_y][case_x].type=RIEN;
	}
}

int check_colision(Pacman *pac, Fantome f)
{
	if(pac->cur_direction != f.cur_direction)
	{
		return 1;
	}
	else return 0;
}

void set_ghosts_eatable(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST_BLOCKS; i++)
	{
		ftm[i].invinsible = 0;
		//Initialisation du compteur pour compter 5 sec
		ftm[i].counter = SDL_GetTicks();
		//On charge l'image du fantome vulnérable
		ftm[i].num_image = 8;
	}
}

void draw_lives(Pacman *pac)
{
	POINT p1;
	SDL_Rect position;
	int i;
	p1.x=WIDTH+10; p1.y=10;
	aff_pol("Lives :", 22, p1, blanc);
	position.y=p1.y+30;
	for(i=0; i<pac->nb_lives; i++)
	{
		position.x=WIDTH+5+i*BLOCK_SIZE;
		SDL_BlitSurface(pac->image[DROITE], NULL, screen, &position);
	}
}

void draw_score()
{
	POINT p1;
	char score[50];
	sprintf(score, "Score : %d", SCORE);
	p1.x=WIDTH+10; p1.y=100;
	aff_pol(score, 22, p1, blanc);
	sprintf(score, "Restant : %d", POINTS);
	p1.x=WIDTH+10; p1.y=150;
	aff_pol(score, 22, p1, blanc);
}
