#include <stdio.h>
#include "editeur.h"
#include "jeu.h"
#include "menus.h"
#include "config.h"

int main()
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
				if(CAMPAGNE_LEVEL)
				{
					load_default_config(&cfg); //Pour eviter de faire des campagnes avec des configs folkloriques
					campagne(&cfg, 0);
				}
				break;
			case 1: //Campagne
				edit_campagne();
				break;
			case 2: //Jouer un niveau
				one_level(select_file_menu(), &cfg);
				break;
			case 3: //Mode survivor
				survivor(select_file_menu(), &cfg);
				break;
			case 4: // Charger une partie
				load_default_config(&cfg); //Pour eviter de faire des campagnes avec des configs folkloriques
				campagne(&cfg, load_game());
				break;
			case 5: //Edition
				editer();
				break;
			case 6: //Option
				options_menu(&cfg);
				break;
			case 7: //Voir les meilleurs score
				draw_result("data/results.txt", 0);
				break;
			case 8: //Quitter
				continuer=0;
				break;
			default: break;
		}
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
