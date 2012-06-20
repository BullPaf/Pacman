#include "jeu.h"

void jouer()
{
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	Pacman pac;
	Fantome ftm[NB_GHOST_BLOCKS];
	SDL_Event event;
	int ok=1, i, pac_new_direction=0, ghosts_new_directions[NB_GHOST_BLOCKS];
	init_level();
	init_blocks();
	load_level();
	init_pacman(&pac);
	init_ghosts(ftm);
	for(i=0; i<NB_GHOST_BLOCKS; i++) ghosts_new_directions[i]=0;
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
		affiche_pacman(&pac, 0);
		affiche_fantomes(ftm, 0);
		deplace_pacman(&pac, pac_new_direction);
		deplace_fantomes(ftm, ghosts_new_directions);
		mange(&pac);
		affiche_pacman(&pac, 1);
		affiche_fantomes(ftm, 1);
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
