#include <stdio.h>
#include "editeur.h"
#include "jeu.h"
#include "menus.h"
#include "config.h"

int main(int argc, char** argv)
{
	int continuer=1, selection=0;
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	init_game();
	config cfg;
	load_default_config(&cfg);
	while (continuer)
	{
		selection=main_menu();
		switch(selection)
		{
			case 0: //Campagne
				load_default_config(&cfg); //Pour eviter de faire des campagnes avec des configs folkloriques
				campagne(&cfg, 0);
				break;
			case 1: //Jouer un niveau
				one_level(select_file_menu(), &cfg);
				break;
			case 2: // Charger une partie
				load_default_config(&cfg); //Pour eviter de faire des campagnes avec des configs folkloriques
				campagne(&cfg, load_game());
				break;
			case 3: //Edition
				editer();
				break;
			case 4: //Option
				options_menu(&cfg);
				break;
			case 5: //Voir les meilleurs score
				draw_result(0);
				break;
			case 6: //Quitter
				continuer=0;
				break;
			default: break;
		}
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
