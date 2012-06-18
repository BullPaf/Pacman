#include "jeu.h"

void jouer()
{
	Pacman pac;
	Pacman *ptr=&pac;
	SDL_Event event;
	int ok=1;
	init_level();
	init_blocks();
	load_level();
	init_pacman(ptr);
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	//draw_level();
	//SDL_Flip(screen);
	while(ok)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT : exit(EXIT_SUCCESS);
				case SDL_KEYDOWN :
					if (event.key.keysym.sym == SDLK_ESCAPE) ok=0;
					break;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		draw_level();
		affiche_pacman(ptr, 0);
		affiche_pacman(ptr, 1);
		SDL_Flip(screen);
	}
}
