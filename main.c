#include "editeur.h"
#include "jeu.h"
#include "menus.h"
#include <stdio.h>

<<<<<<< HEAD
/*Affiche le menu
 * Remplacer par qqchose
 * de mieux qd meme */
void affiche_menu()
{
	POINT p1;
	p1.x=200; p1.y=300;
	aff_pol("JOUER", 25, p1, rouge);
	p1.y=p1.y+50;
	aff_pol("EDITER", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("QUITTER", 25, p1, blanc);
	SDL_Flip(screen);
}

=======
>>>>>>> a49e7cbafbc6e2ecc28fc1972e55e0516e23d459
int main(int argc, char** argv)
{
	int continuer=1, level=0, selection=0;
	SDL_Event event;
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	while (continuer)
	{
		SDL_WaitEvent(&event);
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
					else if(selection==1) editer();
					else if(selection==2) continuer=0;
				}
				else if(event.key.keysym.sym==SDLK_DOWN) selection=(selection+1)%3;
				else if(event.key.keysym.sym==SDLK_UP)
				{
					if(!selection) selection=2;
					else selection--;
				}
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		main_menu(selection);
		SDL_Flip(screen);
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
