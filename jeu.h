#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "menus.h"
#include "levelmanager.h"
#include "config.h"
#include "input.h"
#include "scoremanager.h"

void one_level(int, config*);
int jouer(Pacman*, Fantome*, Input, config*, int);
void campagne(config*, int);
void action(Pacman*, Fantome*);
void draw_score(int, int);
int check_colision(Pacman*, Fantome);

#endif
