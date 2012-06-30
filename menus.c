#include "menus.h"

/*FACTORISER TOUT CE CODE MERDIQUE*/

/*Affiche le menu
 * Remplacer par qqchose
 * de mieux qd meme */
int main_menu()
{
	POINT p1;
	SDL_Event event;
	SDL_Surface* pacman=NULL;
	SDL_Rect pos;
	int selection=0, continuer=1, couleur[]={blanc,gris,blanc, blanc, blanc};
	if( (pacman = IMG_Load("image/menu.png")) == NULL ) exit(EXIT_FAILURE);
	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_RETURN)
				{
					free(pacman);
					return selection;
				}
				else if(event.key.keysym.sym==SDLK_DOWN)
				{
					couleur[selection]=blanc;
					if(selection==0) selection++;
					selection=(selection+1)%5;
				}
				else if(event.key.keysym.sym==SDLK_UP)
				{
					couleur[selection]=blanc;
					if(!selection) selection=4;
					else if(selection==2) selection=0;
					else selection--;
				}
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		couleur[selection]=jaune;
		p1.x=100; p1.y=75;
		aff_pol("BIENVENUE DANS PACMAN!!", 60, p1, jaune);
		p1.x=300; p1.y=200;
		pos.x=p1.x-60;
		pos.y=p1.y+(selection*80);
		SDL_BlitSurface(pacman, NULL, screen, &pos);
		aff_pol("JOUER", 50, p1, couleur[0]);
		p1.y=p1.y+80;
		aff_pol("CHARGER", 50, p1, couleur[1]);
		p1.y=p1.y+80;
		aff_pol("EDITER", 50, p1, couleur[2]);
		p1.y=p1.y+80;
		aff_pol("OPTIONS", 50, p1, couleur[3]);
		p1.y=p1.y+80;
		aff_pol("QUITTER", 50, p1, couleur[4]);
		p1.x=EDIT_WIDTH-50; p1.y=EDIT_HEIGHT-30;
		aff_pol("v 0.1b", 20, p1, jaune);
		SDL_Flip(screen);
	}
	return 0;
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

int game_menu()
{
	POINT p1;
	SDL_Event event;
	SDL_Surface* pacman=NULL;
	SDL_Rect pos;
	int selection=0, continuer=1, couleur[]={blanc,gris,gris,blanc};
	if( (pacman = IMG_Load("image/menu.png")) == NULL ) exit(EXIT_FAILURE);
	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_RETURN)
				{
					free(pacman);
					return selection;
				}
				else if(event.key.keysym.sym==SDLK_DOWN)
				{
					couleur[selection]=blanc;
					if(selection==0) selection+=2;
					selection=(selection+1)%4;
				}
				else if(event.key.keysym.sym==SDLK_UP)
				{
					couleur[selection]=blanc;
					if(!selection) selection=3;
					else if(selection==3) selection=0;
					else selection--;
				}
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		couleur[selection]=jaune;
		p1.x=100; p1.y=75;
		aff_pol("PAUSE", 60, p1, jaune);
		p1.x=300; p1.y=250;
		pos.x=p1.x-60;
		pos.y=p1.y+(selection*80);
		SDL_BlitSurface(pacman, NULL, screen, &pos);
		aff_pol("CONTINUER", 50, p1, couleur[0]);
		p1.y=p1.y+80;
		aff_pol("SAUVER", 50, p1, couleur[1]);
		p1.y=p1.y+80;
		aff_pol("CHARGER", 50, p1, couleur[2]);
		p1.y=p1.y+80;
		aff_pol("MENU PRINCIPAL", 50, p1, couleur[3]);
		SDL_Flip(screen);
	}
	return 0;
}

int select_file_menu()
{
	POINT p1;
	SDL_Event event;
	SDL_Surface* pacman=NULL;
	SDL_Rect pos;
	char level[32];
	int selection=0, continuer=1, i, couleur[NB_LEVEL+1];
	for(i=0; i<NB_LEVEL+1; i++) couleur[i]=blanc;
	if( (pacman = IMG_Load("image/menu.png")) == NULL ) exit(EXIT_FAILURE);
	while(continuer)
	{
		couleur[selection]=blanc;
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_RETURN)
				{
					free(pacman);
					return selection;
				}
				else if(event.key.keysym.sym==SDLK_DOWN) selection=(selection+1)%(NB_LEVEL+1);
				else if(event.key.keysym.sym==SDLK_UP)
				{
					if(!selection) selection=NB_LEVEL;
					else selection--;
				}
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		couleur[selection]=jaune;
		p1.x=100; p1.y=75;
		aff_pol("Choississez un niveau", 50, p1, jaune);
		p1.x=200; p1.y=200;
		pos.x=p1.x-60;
		pos.y=p1.y+(selection*50);
		SDL_BlitSurface(pacman, NULL, screen, &pos);
		for(i=0; i<NB_LEVEL; i++)
		{
			sprintf(level, "Level %d", i+1);
			aff_pol(level, 40, p1, couleur[i]);
			p1.y+=50;
		}
		aff_pol("Annuler", 40, p1, couleur[i]);
		SDL_Flip(screen);
	}
	return 0;
}

int edit_menu()
{
	POINT p1;
	SDL_Event event;
	SDL_Surface* pacman=NULL;
	SDL_Rect pos;
	int selection=0, couleur[]={blanc,blanc,blanc,blanc,gris,blanc};
	if( (pacman = IMG_Load("image/menu.png")) == NULL ) exit(EXIT_FAILURE);
	while(1)
	{
		couleur[selection]=blanc;
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_RETURN)
				{
					free(pacman);
					return selection;
				}
				else if(event.key.keysym.sym==SDLK_DOWN)
				{
					if(selection==3) selection=5;
					else selection=(selection+1)%6;
				}
				else if(event.key.keysym.sym==SDLK_UP)
				{
					if(!selection) selection=5;
					else if(selection==5) selection=3;
					else selection--;
				}
				break;
		}
		// Effacement de l'écran
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		couleur[selection]=jaune;
		p1.x=200; p1.y=50;
		aff_pol("MENU EDITEUR", 60, p1, jaune);
		p1.x=275; p1.y=175;
		pos.x=p1.x-60;
		pos.y=p1.y+(selection*70);
		SDL_BlitSurface(pacman, NULL, screen, &pos);
		aff_pol("CONTINUER", 40, p1, couleur[0]);
		p1.y=p1.y+70;
		aff_pol("SAUVER", 40, p1, couleur[1]);
		p1.y=p1.y+70;
		aff_pol("CHARGER", 40, p1, couleur[2]);
		p1.y=p1.y+70;
		aff_pol("EFFACER", 40, p1, couleur[3]);
		p1.y=p1.y+70;
		aff_pol("TESTER", 40, p1, couleur[4]);
		p1.y=p1.y+70;
		aff_pol("QUITTER", 40, p1, couleur[5]);
		SDL_Flip(screen);
	}
	return 0;
}
