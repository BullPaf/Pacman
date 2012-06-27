#include "editeur.h"
#include "jeu.h"
#include "menus.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	int continuer=1, level=0, selection=0, result=1;
	//SDL_Event event;
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	while (continuer)
	{
		selection=main_menu();
		switch(selection)
		{
			case 0:
				level=0;
				while(result && level<NB_LEVEL)
				{
					result=jouer(level);
					if(result==1) win_menu();
					else if(result==2) result=0;
					else if(!result) lost_menu();
					level++;
				}
				//if(level==NB_LEVEL)
				break;
			case 1: break;
			case 2: editer();
				break;
			case 3: continuer=0;
				break;
			default: break;
		}
		/*SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym==SDLK_ESCAPE) continuer=0;
				else if (event.key.keysym.sym==SDLK_RETURN)
				{
					if(selection==0)
					{
						while(level<NB_LEVEL && jouer(level))
						{
							win_menu();
							level++;
						}
						if(level<NB_LEVEL) lost_menu();
					}
					else if(selection==2) editer();
					else if(selection==3) continuer=0;
				}
				else if(event.key.keysym.sym==SDLK_DOWN)
				{
					if(selection==0) selection++;
					selection=(selection+1)%4;
				}
				else if(event.key.keysym.sym==SDLK_UP)
				{
					if(!selection) selection=3;
					else selection--;
				}
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		main_menu(selection);
		SDL_Flip(screen);*/
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
