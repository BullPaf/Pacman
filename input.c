#include "input.h"

void UpdateEvents(Input* in)
{
	SDL_EnableUNICODE(1);
	SDL_Event event;
	in->mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	in->mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				in->key[event.key.keysym.sym]=1;
				//strcpy(in->touche, event.key.keysym.unicode & 0x7F);//SDL_GetKeyName(event.key.keysym.sym));
				fprintf(stderr, "touche = %c\n", event.key.keysym.unicode);
				in->touche[0] = event.key.keysym.unicode & 0x7F;
				break;
			case SDL_KEYUP:
				in->key[event.key.keysym.sym]=0;
				break;
			case SDL_MOUSEMOTION:
				in->mousex=event.motion.x;
				in->mousey=event.motion.y;
				//in->mousexrel=event.motion.xrel;
				//in->mouseyrel=event.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				in->mousebuttons[event.button.button]=1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button!=SDL_BUTTON_WHEELUP && event.button.button!=SDL_BUTTON_WHEELDOWN)
				in->mousebuttons[event.button.button]=0;
				break;
			case SDL_QUIT:
				in->quit = 1;
				break;
			default:
				break;
		}
	}
	SDL_EnableUNICODE(0);
}

/*Met à jour chaine en fonction de ce qui est tapé
 * Manque plein de chose pour eviter des bugs*/
void print_key(char* chaine, Input *in, int max_size)
{
	int size = strlen(chaine);
	if(in->key[SDLK_BACKSPACE]) //Pour effacer
	{
		if(size > 0) //Si notre chaine contient au moins un caractere
		{
			in->key[SDLK_BACKSPACE]=0; //eviter la repetition
			chaine[size-1]='\0'; //efface
		}
	}
	else if(in->key[SDLK_SPACE] && (size+1 < max_size) ) //Un espace
	{
		in->key[SDLK_SPACE]=0;
		//chaine[size]=' ';
	}
	/*Si la touche ne fait pas dépasser la taille max de la chaine
	 * on concatene*/
	else if( strlen(in->touche) && (size+strlen(in->touche) < max_size) ) strcat(chaine, in->touche);
	in->touche[0]='\0'; //on vide la touche qui a été appuyée
}

