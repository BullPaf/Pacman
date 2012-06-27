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
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
