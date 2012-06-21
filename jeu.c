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
	srand(time(NULL));
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
		affiche_pacman(&pac);
		affiche_fantomes(ftm);
		action(&pac, ftm);
		if(!pac.nb_lives) return 0;
		SDL_Flip(screen);
	}
	return 1;
}

void action(Pacman *pac, Fantome *ftm)
{
	int i, col;
	SDL_Rect pos;
	for(i=0; i<NB_GHOST_BLOCKS; i++) {
		col=check_colision(pac, ftm[i]);
		if(col==1) {
			pac_death(pac);
			return;
		}
		if(col==2) {
			ghost_death(&ftm[i], i);
			return;
		}
	}
	pos=get_case(pac->position, pac->cur_direction);
	if(LEVEL[pos.y][pos.x].type == BONUS)
	{
		if(LEVEL[pos.y][pos.x].elt_type[0]==0) {
			SCORE+=100;
			POINTS--;
		}
		else if(LEVEL[pos.y][pos.x].elt_type[0]==1) {
			//if(pac->nb_lives<5) pac->nb_lives++;
			//else SCORE+=1000;
			set_ghosts_eatable(ftm);
		}
		LEVEL[pos.y][pos.x].type=RIEN;
	}
}

SDL_Rect get_case(SDL_Rect position, int direction)
{
	SDL_Rect pos;
	switch(direction)
	{
		case DROITE: //Vers la droite
			pos.x = (position.x+BLOCK_SIZE-1)/BLOCK_SIZE;
			pos.y = position.y/BLOCK_SIZE;
			break;
		case BAS:
			pos.x = position.x/BLOCK_SIZE;
			pos.y = (position.y+BLOCK_SIZE-1)/BLOCK_SIZE;
			break;
		default:
			pos.x = position.x/BLOCK_SIZE;
			pos.y = position.y/BLOCK_SIZE;
			break;
	}
	return pos;
}

/*Detecteur de colision a améliorer!!*/
int check_colision(Pacman *pac, Fantome f)
{
	SDL_Rect pac_case = get_case(pac->position, pac->cur_direction);
	SDL_Rect ftm_case = get_case(f.position, f.cur_direction);
	//S'ils sont sur la meme case alors colision /!\ En cas de changement de case entre 2 deplacements
	//pas de colision détécté.
	if(pac_case.x == ftm_case.x && pac_case.y == ftm_case.y)
	{
		if(f.invinsible) return 1; //Pacman se fait manger
		else return 2; //Fantome se fait manger
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
	aff_pol("Lives :", FONT_SIZE, p1, blanc);
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
	aff_pol(score, FONT_SIZE, p1, blanc);
	sprintf(score, "Restant : %d", POINTS);
	p1.x=WIDTH+10; p1.y=150;
	aff_pol(score, FONT_SIZE, p1, blanc);
}
