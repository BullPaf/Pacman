//#include "graphics.h"
#include "pacman.h"
#include "levelmanager.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	if(argc == 2)
	{
		if(!strcmp(argv[1], "edit"))
			fprintf(stdout, "Entering Edit Mode!\n");
			init_graphics(EDIT_WIDTH, EDIT_HEIGHT);
			if(enter_edit_mode());
			else return 0;
	}
	Pacman pac;
	Pacman *ptr=&pac;
	int direction;
	
	init_pacman(ptr);
	init_graphics(WIDTH, HEIGHT);
	init_blocks();
	load_level();
	draw_level();
	while (isAlive(ptr))
	{
		attendre(20);
		direction = wait_fleche();
		//test_color();
		affiche_pacman(ptr, 0);
		deplace_pacman(ptr, direction);
		affiche_pacman(ptr, 1);
		SDL_Flip(screen);
	}
	fill_screen(noir);
	SDL_Flip(screen);
	wait_escape();

	return 0;
}
