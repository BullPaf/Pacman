#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL/SDL_ttf.h>
#define POLICE_NAME "angelina.ttf"
// la definition d'un point
typedef struct point {int x,y;} POINT;

// la definition d'une couleur
typedef Uint16 COULEUR;

// les constantes de couleur
                       //   r     g     b
#define noir    0x0000 // 0000 000000 000000
#define gris    0xf3cf // 1111 001111 001111
#define blanc   0xffff // 1111 111111 111111
#define rouge   0xf000 // 1111 000000 000000
#define vert    0xfc0  // 0000 111111 000000
#define bleu    0x3f   // 0000 000000 111111
#define jaune   0xffc0 // 1111 111111 000000
#define cyan    0xfff  // 0000 111111 111111
#define magenta 0xf03f // 1111 000000 111111

/* fenetre sur laquelle on dessine */
SDL_Surface* screen;

void init_graphics(int, int, char*);
void draw_line(POINT p1, POINT p2, Uint16 color, SDL_Surface* s);
// dessine un rectangle non rempli
void draw_rectangle(POINT p1, POINT p2, Uint16 color, SDL_Surface* s);
//dessine une croix
void draw_cross(POINT p1, int T, COULEUR c, SDL_Surface* s);
#ifdef POLICE_NAME
// Affiche le texte passé en argument
void aff_pol(char *a_ecrire, int taille, POINT p, COULEUR C);
#endif

#endif
