#ifndef H_CONSTANTES
#define H_CONSTANTES

#define BLOCK_SIZE 40 //Taille d'un block
#define NB_BLOCKS_LARGEUR 21 //Nombre de block en largeur
#define NB_BLOCKS_HAUTEUR 21 //Nombre de block en hauteur
#define OFFSET 4 //Ecart entre les murs et les bords des blocks
#define NB_BLOCKS 13 //Nombre d'éléments graphique de décor (mur, cerise ect...)
#define STEP 4 //Déplacement en pixel de pacman

#define WIDTH BLOCK_SIZE * NB_BLOCKS_LARGEUR
#define HEIGHT BLOCK_SIZE * NB_BLOCKS_HAUTEUR
#define EDIT_WIDTH WIDTH+216
#define EDIT_HEIGHT HEIGHT

#endif
