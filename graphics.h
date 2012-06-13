#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <SDL.h>
#include <SDL_image.h>
#define WIDTH 840
#define HEIGHT 840
#define EDIT_WIDTH WIDTH+216
#define EDIT_HEIGHT HEIGHT
// la definition d'un point
typedef struct point {int x,y;} POINT;

// la definition d'une couleur
typedef Uint16 COULEUR;

// les constantes de couleur
                       //   r     g     b
#define noir    0x0000 // 0000 000000 000000
#define blanc   0xffff // 1111 111111 111111
#define rouge   0xf000 // 1111 000000 000000
#define vert    0xfc0  // 0000 111111 000000
#define bleu    0x3f   // 0000 000000 111111
#define jaune   0xffc0 // 1111 111111 000000
#define cyan    0xfff  // 0000 111111 111111
#define magenta 0xf03f // 1111 000000 111111

/* fenetre sur laquelle on dessine */
SDL_Surface* screen;
//int WIDTH, HEIGHT;

void init_graphics();
void wait_escape();
int wait_fleche();
// attend que l'on clique à gauche
POINT wait_clic();
// attend un déplacement de souris
POINT get_mouse();

void draw_line(POINT p1, POINT p2, Uint16 color, SDL_Surface* s);
// remplissage de tout l'écran
void fill_screen(Uint16 color);
//dessine un rectangle rempli
void draw_fill_rectangle(POINT p1, POINT p2, Uint16 color, SDL_Surface* s);
// dessine un rectangle non rempli
void draw_rectangle(POINT p1, POINT p2, Uint16 color, SDL_Surface* s);
//dessine une croix
void draw_cross(POINT p1, int T, COULEUR c, SDL_Surface* s);

// Attend le nombre de millisecondes passé en argument
void attendre(int millisecondes);

#endif
