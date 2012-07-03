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
			case 1:
				level=select_file_menu();
				result=one_level(level, &cfg);
				if(result==1) win_menu();
				else if(!result) lost_menu();
				break; // Charger
			case 2:
				break;
			case 3:
				editer(); //Edition
				break;
			case 4:
				configure(&cfg); //Option
				break;
			case 5:
				continuer=0; //Quitter
				break;
			default: break;
		}
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
