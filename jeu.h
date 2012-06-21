#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "levelmanager.h"

void jouer();
void action(Pacman*, Fantome*);
void set_ghosts_eatable(Fantome *ftm);
void draw_lives(Pacman*);
void draw_score();
int check_colision(Pacman*, Fantome);

#endif
