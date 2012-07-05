#ifndef H_CONSTANTES
#define H_CONSTANTES

enum {HAUT, DROITE, BAS, GAUCHE};

#define VERSION "0.5"
#define BLOCK_SIZE 20 //Taille d'un block
#define NB_BLOCKS_LARGEUR 31 //Nombre de block en largeur
#define NB_BLOCKS_HAUTEUR 31 //Nombre de block en hauteur
#define NB_WALL_BLOCKS 20 //Nombre d'éléments graphique de mur
#define NB_BONUS_BLOCKS 10 //Nombre d'éléments graphique bonus (cerises, points ect...)
#define NB_PACMAN_BLOCK 4
#define NB_GHOST_BLOCKS 4
#define NB_MAX_GHOSTS 16
#define NB_ALL_BLOCKS NB_BONUS_BLOCKS+NB_WALL_BLOCKS+NB_PACMAN_BLOCK+NB_GHOST_BLOCKS
#define DELAY 40 //Diminuer pour accélérer le jeu
#define NB_LEVEL 3

#define WIDTH BLOCK_SIZE * NB_BLOCKS_LARGEUR //Largeur d'ecran
#define HEIGHT BLOCK_SIZE * NB_BLOCKS_HAUTEUR //Hauteur d'ecran
#define EDIT_WIDTH WIDTH+200 //Largeur en mode edition
#define EDIT_HEIGHT HEIGHT //Hauteur en mode edition

#define FONT_SIZE 22

#endif
