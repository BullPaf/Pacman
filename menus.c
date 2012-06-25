#include "menus.h"

/*Affiche le menu
 * Remplacer par qqchose
 * de mieux qd meme */
void main_menu(int selection)
{
	POINT p1;
	SDL_Surface* pacman=NULL;
	SDL_Rect pos;
	int couleur[]={blanc,blanc,blanc};
	couleur[selection]=jaune;
	if( (pacman = IMG_Load("image/menu.png")) == NULL ) exit(EXIT_FAILURE);
	p1.x=100; p1.y=75;
	aff_pol("BIENVENUE DANS PACMAN!!", 60, p1, jaune);
	p1.x=300; p1.y=250;
	pos.x=p1.x-60;
	pos.y=p1.y+(selection*80);
	SDL_BlitSurface(pacman, NULL, screen, &pos);
	aff_pol("JOUER", 50, p1, couleur[0]);
	p1.y=p1.y+80;
	aff_pol("EDITER", 50, p1, couleur[1]);
	p1.y=p1.y+80;
	aff_pol("QUITTER", 50, p1, couleur[2]);
	p1.x=EDIT_WIDTH-50; p1.y=EDIT_HEIGHT-30;
	aff_pol("v 0.1b", 20, p1, jaune);
	free(pacman);
}

void play_menu(int level)
{
	char tmp[16], time[4];
	POINT p1,p2;
	int counter=3;
	sprintf(tmp, "LEVEL  %d", level);
	p1.x=320; p1.y=200;
	p2.x=380; p2.y=260;
	while(counter)
	{
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		sprintf(time, "%d", counter);
		aff_pol(tmp, 50, p1, jaune);
		aff_pol(time, 50, p2, jaune);
		SDL_Flip(screen);
		SDL_Delay(1000);
		counter--;
	}
}

void win_menu()
{
	POINT p1;
	p1.x=200; p1.y=250;
	aff_pol("YOU WON !", 75, p1, jaune);
	SDL_Flip(screen);
	SDL_Delay(3000);
}

void lost_menu()
{
	POINT p1;
	p1.x=200; p1.y=250;
	aff_pol("YOU LOST !", 75, p1, jaune);
	SDL_Flip(screen);
	SDL_Delay(3000);
}
