//#include "graphics.h"
#include "pacman.h"
#include "levelmanager.h"
#include "editeur.h"
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
				else if (event.key.keysym.sym==SDLK_KP1) fprintf(stdout, "On Joue!\n");//jouer();
				else if (event.key.keysym.sym==SDLK_KP2) editer();
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		affiche_menu();
		SDL_Flip(screen);
	}
	SDL_Quit();
	/*Pacman pac;
	Pacman *ptr=&pac;
	int direction;

	if(argc == 2)
	{
		if(!strcmp(argv[1], "edit"))
			fprintf(stdout, "Entering Edit Mode!\n");
			init_graphics(EDIT_WIDTH, EDIT_HEIGHT);
			if(enter_edit_mode());
			else return 0;
	}

	init_pacman(ptr);
	init_graphics(WIDTH, HEIGHT);
	init_blocks();
	load_level();
	draw_level();
	while (isAlive(ptr))
	{
		SDL_Delay(20);
		direction = wait_fleche();
		affiche_pacman(ptr, 0);
		deplace_pacman(ptr, direction);
		affiche_pacman(ptr, 1);
		SDL_Flip(screen);
	}
	fill_screen(noir);
	SDL_Flip(screen);
	wait_escape();*/

	return EXIT_SUCCESS;
}
