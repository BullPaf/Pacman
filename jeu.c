#include "jeu.h"

void jouer()
{
	Pacman pac;
	Pacman *ptr=&pac;
	SDL_Event event;
	int ok=1, direction=0;
	init_level();
	init_blocks();
	load_level();
	init_pacman(ptr);
	while(ok)
	{
		SDL_Delay(50);
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT : exit(EXIT_SUCCESS);
				case SDL_KEYDOWN :
					if (event.key.keysym.sym == SDLK_ESCAPE) ok=0;
					else if (event.key.keysym.sym == SDLK_UP) direction=HAUT;
					else if (event.key.keysym.sym == SDLK_DOWN) direction=BAS;
					else if (event.key.keysym.sym == SDLK_LEFT) direction=GAUCHE;
					else if (event.key.keysym.sym == SDLK_RIGHT) direction=DROITE;
					break;
				default:
					direction=0;
					break;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		draw_level();
		affiche_pacman(ptr, 0);
		deplace_pacman(ptr, direction);
		mange(ptr);
		affiche_pacman(ptr, 1);
		SDL_Flip(screen);
	}
}

void mange(Pacman *pac)
{
	int case_x,case_y;
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
	if(LEVEL[case_y][case_x].type == BONUS)
	{
		if(LEVEL[case_y][case_x].elt_type[0]==0) pac->score+=100;
		else if(LEVEL[case_y][case_x].elt_type[0]==1)pac->nb_lives++;
		LEVEL[case_y][case_x].type=RIEN;
	}
}
