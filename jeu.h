#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "levelmanager.h"

void jouer();
void action(Pacman*, Fantome*);
void set_ghosts_eatable(Fantome *ftm);
int check_colision(Pacman*, Fantome);

#endif
