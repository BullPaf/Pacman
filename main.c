#include <stdio.h>
#include "editeur.h"
#include "jeu.h"
#include "menus.h"
#include "config.h"

int main(int argc, char** argv)
{
	int continuer=1, level, selection=0, result;
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	config cfg;
	load_default_config(&cfg);
	while (continuer)
	{
		selection=main_menu();
		switch(selection)
		{
			case 0: //Campagne
				campagne(&cfg);
				break;
			case 1: //Jouer un niveau
				level=select_file_menu();
				result=one_level(level, &cfg);
				if(result==1) win_menu();
				else if(!result) lost_menu();
				break;
			case 2: // Charger
				break;
			case 3: //Edition
				editer();
				break;
			case 4: //Option
				configure(&cfg);
				break;
			case 5: //Quitter
				continuer=0;
				break;
			default: break;
		}
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
