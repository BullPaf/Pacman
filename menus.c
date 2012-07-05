#include "menus.h"

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
	menu->img = IMG_Load("image/menu/menu.png");
	if(menu->img == NULL )
	{
		fprintf(stderr, "Erreur loading menu.png in >>init_menu()<<\n");
		exit(EXIT_FAILURE);
	}
	menu->nb_options = nb;
	menu->selection=0;
}

/*Rajouter le centrage des éléments
 * Vérifier que tout entre dans l'ecran
 * sinon réduire la police ou faire
 * des colonnes*/
int draw_menu(Menu menu)
{
	int i;
	Input in;
	memset(&in,0,sizeof(in));
	for(i=0; i<menu.nb_options; i++) if(!menu.available[i]) menu.couleur[i]=gris;
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
		menu.pos.y=menu.p1.y+(menu.selection*60)-5;
		SDL_BlitSurface(menu.img, NULL, screen, &(menu.pos));
		for(i=0; i<menu.nb_options; i++)
		{
			aff_pol(menu.options[i], 30, menu.p1, menu.couleur[i]);
			menu.p1.y=menu.p1.y+60;
		}
		SDL_Flip(screen);
	}
	return menu.nb_options-1;
}

int main_menu()
{
	int nb=7;
	Menu menu;
	init_menu(&menu, nb);
	strcpy(menu.title, "BIENVENUE DANS PACMAN!!");
	strcpy(menu.options[0], "CAMPAGNE");
	strcpy(menu.options[1], "JOUER");
	strcpy(menu.options[2], "CHARGER");
	strcpy(menu.options[3], "EDITER");
	strcpy(menu.options[4], "OPTIONS");
	strcpy(menu.options[5], "HIGH SCORE");
	strcpy(menu.options[6], "QUITTER");
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

/*Ceci est un menu special
 * Il ne suit pas les memes
 * regles que les autres menus
 * Accessoirement c'est tres moche*/
void options_menu(config *cfg)
{
	int i, nb=8, couleur[nb], available[nb], selection=0;
	SDL_Surface *pac, *ghost;
	SDL_Rect pos;
	POINT p1;
	Input in;
	char title[64];
	char options[8][64];

	for(i=0; i<nb; i++)
	{
		available[i]=1;
		couleur[i]=blanc;
	}
	pac   = IMG_Load("image/pacman/2.png");
	ghost = IMG_Load("image/ghosts/2.png");
	if(pac == NULL || ghost == NULL)
	{
		fprintf(stderr, "Erreur loading pngs in >>options_menu()<<\n");
		exit(EXIT_FAILURE);
	}
	memset(&in,0,sizeof(in));
	strcpy(title, "OPTIONS");
	strcpy(options[0], "Nb players");
	sprintf(options[1], "%d", cfg->nb_players);
	strcpy(options[2], "Player 1");
	strcpy(options[3], "Player 2");
	strcpy(options[4], "Sound");
	strcpy(options[5], "Reset");
	strcpy(options[6], "Cancel");
	strcpy(options[7], "Save");
	available[1]=0;
	available[4]=0;
	if(cfg->nb_players==0)
	{
		available[2]=0;
		available[3]=0;
	}
	else if(cfg->nb_players==1) available[3]=0;
	while(!in.quit)
	{
		for(i=0; i<nb; i++)
		{
			if(!available[i]) couleur[i]=gris;
			else couleur[i]=blanc;
		}
		couleur[selection]=blanc;
		UpdateEvents(&in);
		if(in.key[SDLK_ESCAPE]) exit(EXIT_SUCCESS);
		else if(in.key[SDLK_RETURN])
		{
			in.key[SDLK_RETURN]=0;
			if(selection==5) load_default_config(cfg); //reset
			else if(selection==6)
			{
				load_default_config(cfg);
				return;
			}
			else if(selection==7) return;
		}
		else if(in.key[SDLK_DOWN])
		{
			in.key[SDLK_DOWN]=0;
			selection=(selection+1)%nb;
			while(!available[selection]) selection=(selection+1)%nb;
		}
		else if(in.key[SDLK_UP])
		{
			in.key[SDLK_UP]=0;
			if(!(selection)) selection=nb-1;
			else selection=(selection-1)%nb;
			while(!available[selection])
			{
				if(!selection) selection=nb-1;
				else selection=(selection-1)%nb;
			}
		}
		else if(in.key[SDLK_LEFT])
		{
			in.key[SDLK_LEFT]=0;
			if(selection==0) {
				if(cfg->nb_players>0)
				{
					available[cfg->nb_players+1]=0;
					cfg->nb_players=cfg->nb_players-1;
					sprintf(options[1], "%d", cfg->nb_players);
				}
			}
			else if(selection==2 || selection==3) {
				if(cfg->players[selection-2].character == PACMAN) cfg->players[selection-2].character = GHOST;
				else if(cfg->players[selection-2].character == GHOST) cfg->players[selection-2].character = PACMAN;
			}
		}
		else if(in.key[SDLK_RIGHT])
		{
			in.key[SDLK_RIGHT]=0;
			if(selection==0)
			{
				if(cfg->nb_players<2) {
					available[cfg->nb_players+2]=1;
					cfg->nb_players=(cfg->nb_players+1);
					sprintf(options[1], "%d", cfg->nb_players);
				}
			}
			else if(selection==2 || selection==3) {
				if(cfg->players[selection-2].character == PACMAN) cfg->players[selection-2].character = GHOST;
				else if(cfg->players[selection-2].character == GHOST) cfg->players[selection-2].character = PACMAN;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		couleur[selection]=jaune;

		p1.x=300; p1.y=50;
		aff_pol(title, 50, p1, jaune); //Le titre

		p1.x=310; p1.y=150;
		aff_pol(options[0], 30, p1, couleur[0]); //Nb players
		p1.x+=150;
		aff_pol(options[1], 30, p1, couleur[1]); //Nb players
		//Afficher des fleches

		p1.x=250; p1.y=250;
		pos.x=p1.x+50; pos.y=p1.y+50;
		aff_pol(options[2], 30, p1, couleur[2]); //Player 1
		if(cfg->players[0].character == GHOST) SDL_BlitSurface(ghost, NULL, screen, &pos);
		else SDL_BlitSurface(pac, NULL, screen, &pos);
		//Afficher des fleches

		p1.x+=200;
		pos.x=p1.x+50;
		aff_pol(options[3], 30, p1, couleur[3]); //Player 2
		if(cfg->players[1].character == GHOST) SDL_BlitSurface(ghost, NULL, screen, &pos);
		else SDL_BlitSurface(pac, NULL, screen, &pos);
		//Afficher des fleches

		p1.x=250; p1.y+=150;
		aff_pol(options[4], 30, p1, couleur[4]); //Sound
		//Afficher des images pour le son

		p1.x=150; p1.y+=150;
		for(i=5; i<8; i++)
		{
			aff_pol(options[i], 30, p1, couleur[i]);
			p1.x+=225;
		}
		SDL_Flip(screen);
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
