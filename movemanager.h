#ifndef H_MOVE
#define H_MOVE
#include "graphics.h"
#include "levelmanager.h"
#include "constantes.h"

enum {CENTRE, HAUT, DROITE, BAS, GAUCHE};

int can_move(SDL_Rect, int, int, int*);
int in_intersection(SDL_Rect, int);
void move(SDL_Rect*, int, int);

#endif
