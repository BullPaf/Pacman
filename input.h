#ifndef H_INPUT
#define H_INPUT
#include "graphics.h"
#include <string.h>

typedef struct
{
	char key[SDLK_LAST];
	int mousex,mousey;
	//int mousexrel,mouseyrel;
	char mousebuttons[8];
	char touche[2]; //Stocke le nom de la touche appuyée
	char quit;
}Input;

void UpdateEvents(Input*);
void print_key(char*, Input*, unsigned);

#endif
