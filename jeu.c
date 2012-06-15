#include "jeu.h"

void jouer()
{
	SDL_Event event;
	int ok=1;
	init_level();
	fprintf(stderr, "Level initialized successfully!\n");
	init_blocks();
	fprintf(stderr, "Blocks loaded successfully!\n");
	load_level();
	fprintf(stderr, "Level loaded successfully!\n");
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	fprintf(stderr, "Dessin start!\n");
	draw_level();
	fprintf(stderr, "Level draw successfully!\n");
	SDL_Flip(screen);
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
	}
}
