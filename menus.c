#include "menus.h"
#include "input.h"

/*FACTORISER TOUT CE CODE MERDIQUE*/

void init_menu(Menu *menu, int nb)
{
	int i;
	menu->options   = (char**)malloc(nb*sizeof(char*));
	menu->couleur   = malloc(nb * sizeof(int));
	menu->available = malloc(nb * sizeof(int));
	for(i=0; i<nb; i++)
	{
		menu->couleur[i]   = blanc;
		menu->available[i] = 1;
		menu->options[i]   = malloc(64 * sizeof(char));
	}
	menu->img = IMG_Load("image/menu.png");
	if(menu->img == NULL )
	{
		fprintf(stderr, "Erreur loading menu.png in >>init_menu()<<\n");
		exit(EXIT_FAILURE);
	}
	menu->nb_options = nb;
	menu->selection=0;
}

int draw_menu(Menu menu)
{
	int i;
	Input in;
	memset(&in,0,sizeof(in));
	while(!in.quit)
	{
		menu.couleur[menu.selection]=blanc;
		UpdateEvents(&in);
		if(in.key[SDLK_RETURN])
		{
			in.key[SDLK_RETURN]=0;
			return menu.selection;
		}
		else if(in.key[SDLK_DOWN])
		{
			in.key[SDLK_DOWN]=0;
			menu.selection=(menu.selection+1)%(menu.nb_options);
			while(!menu.available[menu.selection]) menu.selection=(menu.selection+1)%(menu.nb_options);
		}
		else if(in.key[SDLK_UP])
		{
			in.key[SDLK_UP]=0;
			if(!(menu.selection)) menu.selection=menu.nb_options-1;
			else menu.selection=(menu.selection-1)%(menu.nb_options);
			while(!menu.available[menu.selection])
			{
				if(!(menu.selection)) menu.selection=menu.nb_options-1;
				else menu.selection=(menu.selection-1)%(menu.nb_options);
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		menu.couleur[menu.selection]=jaune;
		menu.p1.x=100; menu.p1.y=50;
		aff_pol(menu.title, 50, menu.p1, jaune);
		menu.p1.x=300; menu.p1.y=150;
		menu.pos.x=menu.p1.x-60;
		menu.pos.y=menu.p1.y+(menu.selection*70)-5;
		SDL_BlitSurface(menu.img, NULL, screen, &(menu.pos));
		for(i=0; i<menu.nb_options; i++)
		{
			aff_pol(menu.options[i], 40, menu.p1, menu.couleur[i]);
			menu.p1.y=menu.p1.y+70;
		}
		SDL_Flip(screen);
	}
	return menu.nb_options-1;
}

int main_menu()
{
	int nb=6;
	Menu menu;
	init_menu(&menu, nb);
	strcpy(menu.title, "BIENVENUE DANS PACMAN!!");
	strcpy(menu.options[0], "CAMPAGNE");
	strcpy(menu.options[1], "JOUER");
	strcpy(menu.options[2], "CHARGER");
	strcpy(menu.options[3], "EDITER");
	strcpy(menu.options[4], "OPTIONS");
	strcpy(menu.options[5], "QUITTER");
	menu.available[2]=0;
	int selection = draw_menu(menu);
	delete_menu(&menu);
	return selection;
}

int game_menu()
{
	int nb = 4;
	Menu menu;
	init_menu(&menu, nb);
	strcpy(menu.title, "PAUSE");
	strcpy(menu.options[0], "CONTINUER");
	strcpy(menu.options[1], "SAUVER");
	strcpy(menu.options[2], "CHARGER");
	strcpy(menu.options[3], "MENU PRINCIPAL");
	menu.available[1]=0;
	menu.available[2]=0;
	int selection = draw_menu(menu);
	delete_menu(&menu);
	return selection;
}

int select_file_menu()
{
	int nb = NB_LEVEL+1, i;
	Menu menu;
	init_menu(&menu, nb);
	strcpy(menu.title, "Choississez un niveau");
	for(i=0; i<nb-1; i++) sprintf(menu.options[i], "Level %d", i+1);
	strcpy(menu.options[i], "Annuler");
	int selection = draw_menu(menu);
	delete_menu(&menu);
	return selection;
}

int edit_menu()
{
	int nb=6;
	Menu menu;
	init_menu(&menu, nb);
	strcpy(menu.title, "MENU EDITEUR");
	strcpy(menu.options[0], "CONTINUER");
	strcpy(menu.options[1], "SAUVER");
	strcpy(menu.options[2], "CHARGER");
	strcpy(menu.options[3], "EFFACER");
	strcpy(menu.options[4], "TESTER");
	strcpy(menu.options[5], "QUITTER");
	menu.available[4]=0;
	int selection = draw_menu(menu);
	delete_menu(&menu);
	return selection;
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

void draw_version()
{
	POINT p1;
	p1.x=EDIT_WIDTH-50; p1.y=EDIT_HEIGHT-30;
	aff_pol(VERSION, 20, p1, jaune);
}

void delete_menu(Menu *menu)
{
	free(menu->options);
	free(menu->couleur);
	free(menu->img);
	free(menu->available);
}
