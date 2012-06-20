//#include "graphics.h"
#include "pacman.h"
#include "levelmanager.h"
#include "editeur.h"
#include "jeu.h"
#include <stdio.h>

/*Affiche le menu
 * Remplacer par qqchose
 * de mieux qd meme */
void affiche_menu()
{
	POINT p1;
	p1.x=200; p1.y=300;
	aff_pol("1 ======> JOUER", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("2 ======> EDITER", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("ECHAP ====> QUITTER", 25, p1, blanc);
	SDL_Flip(screen);
}

int main(int argc, char** argv)
{
	int continuer=1;
	SDL_Event event;
	init_graphics(WIDTH, HEIGHT, "Pacman");
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
				else if (event.key.keysym.sym==SDLK_KP1) jouer(); //fprintf(stdout, "On Joue!\n");
				else if (event.key.keysym.sym==SDLK_KP2) editer(); //fprintf(stdout, "On Edite!\n");
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		affiche_menu();
		SDL_Flip(screen);
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
