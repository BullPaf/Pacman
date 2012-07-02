#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "menus.h"
#include "levelmanager.h"
#include "config.h"

int jouer(int, config*);
int campagne_pacman(int);
void action(Pacman*, Fantome*);
void set_ghosts_eatable(Fantome *ftm);
void draw_pac_infos(Pacman*);
void draw_score(int);
/*Detecteur de colision a améliorer*/
int check_colision(Pacman*, Fantome);

#endif
