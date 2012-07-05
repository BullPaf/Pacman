#include "input.h"

void UpdateEvents(Input* in)
{
	SDL_Event event;
	in->mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	in->mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				in->key[event.key.keysym.sym]=1;
				//fprintf(stderr, "Appuyer : %s\n", SDL_GetKeyName(event.key.keysym.sym));
				strcpy(in->touche, SDL_GetKeyName(event.key.keysym.sym));
				break;
			case SDL_KEYUP:
				in->key[event.key.keysym.sym]=0;
				//fprintf(stderr, "Relacher : %s\n", SDL_GetKeyName(event.key.keysym.sym));
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
}

void print_key(char* chaine, Input *in, int max_size)
{
	int size = strlen(chaine);
	if(in->key[SDLK_BACKSPACE])
	{
		if(size > 0)
		{
			in->key[SDLK_BACKSPACE]=0;
			chaine[size-1]='\0';
		}
	}
	else if(in->key[SDLK_SPACE] && (size+1 < max_size) )
	{
		in->key[SDLK_SPACE]=0;
		 chaine[size]=' ';
	}
	else if( strlen(in->touche) && (size+strlen(in->touche) < max_size) ) strcat(chaine, in->touche);
	in->touche[0]='\0';
}

