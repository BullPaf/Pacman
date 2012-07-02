#ifndef H_MOVE
#define H_MOVE
#include "graphics.h"
#include "config.h"
#include "levelmanager.h"
#include "input.h"
#include "constantes.h"

int can_move(SDL_Rect, int, int, int*);
int in_intersection(SDL_Rect, int);
int find_direction(SDL_Rect, int, SDL_Rect*, int*);
void move(SDL_Rect*, int, int);

void human_controller(Input, config, int, SDL_Rect*, int*, int*, int*, int*, SDL_Rect*);
void ia_controller(Input, config, int, SDL_Rect*, int*, int*, int*, int*, SDL_Rect*);

#endif
