#include <stdio.h>
#include "editeur.h"
#include "jeu.h"
#include "menus.h"

int main(int argc, char** argv)
{
	int continuer=1, level=0, selection=0, result=1;
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Pacman");
	while (continuer)
	{
		selection=main_menu();
		switch(selection)
		{
			case 0: //Campagne
				campagne_pacman(0);
				break;
			case 1:
				if(argc==2) level=atoi(argv[1]);
				else level=0;
				while(result && level<NB_LEVEL)
				{
					result=jouer(level);
					if(result==1) win_menu();
					else if(result==2) result=0;
					else if(!result) lost_menu();
					level++;
				}
				//if(level==NB_LEVEL)
				break; // Charger
			case 2: break;
			case 3: editer(); //Edition
				break;
			case 4: //options(); //Option
				break;
			case 5: continuer=0; //Quitter
				break;
			default: break;
		}
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
