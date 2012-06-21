#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "levelmanager.h"

int jouer();
void action(Pacman*, Fantome*);
void set_ghosts_eatable(Fantome *ftm);
void draw_lives(Pacman*);
void draw_score();
int check_colision(Pacman*, Fantome);
SDL_Rect get_case(SDL_Rect, int);

#endif
