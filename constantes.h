#ifndef H_CONSTANTES
#define H_CONSTANTES

#define BLOCK_SIZE 20 //Taille d'un block
#define NB_BLOCKS_LARGEUR 31 //Nombre de block en largeur
#define NB_BLOCKS_HAUTEUR 31 //Nombre de block en hauteur
#define NB_WALL_BLOCKS 6 //Nombre d'éléments graphique de mur
#define NB_BONUS_BLOCKS 2 //Nombre d'éléments graphique bonus (cerises, points ect...)
#define MAX_ELT_CASE 5 //Nombre d'élément pouvant s'afficher sur une case
#define STEP 10 //Déplacement en pixel de pacman

#define WIDTH BLOCK_SIZE * NB_BLOCKS_LARGEUR //Largeur d'ecran
#define HEIGHT BLOCK_SIZE * NB_BLOCKS_HAUTEUR //Hauteur d'ecran
#define EDIT_WIDTH WIDTH+216 //Largeur en mode edition
#define EDIT_HEIGHT HEIGHT //Hauteur en mode edition

#endif
