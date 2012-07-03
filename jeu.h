#ifndef H_JEU
#define H_JEU
#include "pacman.h"
#include "fantome.h"
#include "menus.h"
#include "levelmanager.h"
#include "config.h"

int one_level(int, config*);
int jouer(Pacman*, Fantome*, Input, config*);
void campagne(config*);
void action(Pacman*, Fantome*);
void set_ghosts_eatable(Fantome *ftm);
void draw_pac_infos(Pacman*);
void draw_score();
/*Detecteur de colision a améliorer*/
int check_colision(Pacman*, Fantome);

#endif
