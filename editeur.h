#ifndef H_EDITEUR
#define H_EDITEUR
#include "levelmanager.h"

enum {AUCUN, SAVE, LOAD, DELETE}; //Pour les messages de l'editeur

CASE editor[NB_WALL_BLOCKS+NB_BONUS_BLOCKS+4];

int editer();
void load_gui();
void init_editor();
void highlight_block(int);
int get_block_type(int, int, int);
void print_info(int*, int, POINT);

#endif
