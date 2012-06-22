#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "levelmanager.h"

int jouer(int);
void action(Pacman*, Fantome*);
void set_ghosts_eatable(Fantome *ftm);
void draw_lives(Pacman*);
void draw_score(int);
/*Detecteur de colision a améliorer*/
int check_colision(Pacman*, Fantome);
void ghost_death(Fantome*, int, Pacman*);
void pac_death(Pacman*, Fantome*);

#endif
