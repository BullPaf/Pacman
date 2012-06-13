#include <stdio.h>
#include <time.h>
#include "graphics.h"
#include "pacman.h"

/* initialise le graphique */
void init_graphics(int W, int H)
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(W, H, 16, SDL_SWSURFACE|SDL_DOUBLEBUF);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption("Pacman", NULL);
	SDL_EnableKeyRepeat(10, 10);
	if ( screen == NULL )
	{
		fprintf(stderr, "Impossible de passer en %dx%d en 16 bpp: %s\n", WIDTH, HEIGHT, SDL_GetError());
		exit(1);
	}
}

/* on vérifie que ça ne déborde pas */
int dans_ecran(int x, int y, SDL_Surface* s)
{
	if (x<0) return 0;
	if (x>=s->w) return 0;
	if (y<0) return 0;
	if (y>=s->h) return 0;
	return 1;
}

/* attend que l'on tape Echap */
void wait_escape()
{
	int display = 1;
	SDL_Event event;
	while (SDL_WaitEvent(&event) && display)
	{
		/* si l'utilisateur a demande a fermer la fenetre, on quitte */
		if (event.type == SDL_QUIT) exit(0);

		/* si l'utilisateur a appuye sur une touche... */
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE : display=0;	break;
				default :	break;
			}
		}
	}
	SDL_Quit();
}

/* dessine un segment */
void draw_line(POINT p1, POINT p2, Uint16 color, SDL_Surface* s)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;
	float a,b,ii,jj;

	if (p1.x < p2.x) {xmin=p1.x; xmax=p2.x;} else{xmin=p2.x; xmax=p1.x;}
	if (p1.y < p2.y) {ymin=p1.y; ymax=p2.y;} else{ymin=p2.y; ymax=p1.y;}

	if (xmin==xmax) for (j=ymin;j<=ymax;j++) if (dans_ecran(xmin,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + xmin) = color;
	if (ymin==ymax) for (i=xmin;i<=xmax;i++) if (dans_ecran(i,ymin,s)) *((Uint16 *)s->pixels + (s->h-ymin-1) * s->w + i) = color;


	/* On affiche la droite en faisant varier la coordonnée de plus grande pente */
	if ((xmax-xmin >= ymax-ymin) && (ymax-ymin>0))
	{
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (i=xmin;i<=xmax;i++)
		{
			jj = a*i+b;
			j = jj;
			if (((jj-j) > 0.5) && (j < s->h-1)) j++;
			if (dans_ecran(i,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + i) = color;
		}
	}

	if ((ymax-ymin > xmax-xmin) && (xmax-xmin>0))
	{
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (j=ymin;j<=ymax;j++)
		{
			/*dessine un segment */
			ii = (j-b)/a;
			i = ii;
			if (((ii-i) > 0.5) && (i < s->w-1)) i++;
			if (dans_ecran(i,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + i) = color;
		}
	}
	//SDL_Flip(screen);
}

/* dessine un rectangle rempli */
void draw_fill_rectangle(POINT p1, POINT p2, Uint16 color, SDL_Surface* s)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;

	if (p1.x < p2.x) {xmin=p1.x; xmax=p2.x;} else{xmin=p2.x; xmax=p1.x;}
	if (p1.y < p2.y) {ymin=p1.y; ymax=p2.y;} else{ymin=p2.y; ymax=p1.y;}

	for (i=xmin;i<=xmax;i++)
		for (j=ymin;j<=ymax;j++) 
		{
			if (dans_ecran(i,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + i) = color;
		}
}

/* dessine un rectangle non rempli */
void draw_rectangle(POINT p1, POINT p2, Uint16 color, SDL_Surface* s)
{
	POINT tmp;
	 
	tmp.x=p1.x;
	tmp.y=p2.y;
	draw_line(p1, tmp, color, s);
	draw_line(tmp, p2, color, s);
	tmp.x=p2.x;
	tmp.y=p1.y;
	draw_line(p1, tmp, color, s);
	draw_line(tmp, p2, color, s);
}

//dessine une croix
void draw_cross(POINT p1, int T, COULEUR c, SDL_Surface* s)
{
	POINT p2;
	POINT p3;
	POINT p4;
	POINT p5;
	
	p2.x = p1.x + T/2; p2.y = p1.y - T/2;
	p3.x = p1.x - T/2; p3.y = p1.y - T/2;
	p4.x = p1.x - T/2; p4.y = p1.y + T/2;
	p5.x = p1.x + T/2; p5.y = p1.y + T/2;
	
	draw_line(p1, p2, c, s);
	draw_line(p1, p3, c, s);
	draw_line(p1, p4, c, s);
	draw_line(p1, p5, c, s);
}

int wait_fleche()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) exit(0);
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE : exit(0);
				case SDLK_LEFT   : return 1;
				case SDLK_UP     : return 2;
				case SDLK_RIGHT  : return 3;
				case SDLK_DOWN   : return 4;
				case SDLK_s      : return 5;
				case SDLK_l      : return 6;
				case SDLK_p      : return 7;
				case SDLK_r      : return 8;
				default : break;
			}
		}
	}
	return 0;
}

/* remplissage de tout l'écran */
void fill_screen(Uint16 color)
{
	int i,j;
	for (i=0;i<WIDTH;i++)
		for (j=0;j<HEIGHT;j++) *((Uint16 *)screen->pixels + (HEIGHT-j-1) * WIDTH + i) = color;
}

void attendre(int millisecondes)
{
	SDL_Delay(millisecondes);
}

/* attend que l'on clique et renvoie le point cliqué */
POINT wait_clic()
{
	POINT P;
	SDL_Event event;
	P.x = P.y = -1;
	while (SDL_PollEvent(&event))
	{
		/* si l'utilisateur a demande a fermer la fenetre, on quitte */
		if (event.type == SDL_QUIT) exit(0);

		/* si l'utilisateur clique avec la souris */
		if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT :
					P.x = event.button.x;
					P.y = event.button.y;
					break;
				default : break;
			}
		}
	}
	return P;
}

/* 
attend que l'on bouge la souris et renvoie la valeur du déplacement
sinon fonction renvoie une valeur de déplacement nulle
*/
POINT get_mouse()
{
	POINT dep;
	dep.x = dep.y = -1;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		//SDL_WaitEvent(&event);
		/* si l'utilisateur a demande a fermer la fenetre, on quitte */
		if (event.type == SDL_QUIT) exit(0);
		/* si l'utilisateur a appuye sur une touche... */
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE : exit(0);
				default :	break;
			}
		}
		/* si l'utilisateur bouge la souris... */
		if (event.type == SDL_MOUSEMOTION)
		{
			dep.x = event.motion.x;
			dep.y = event.motion.y;
		}
	}
	return dep;
}

#ifdef POLICE_NAME
// Affiche le texte passé en argument
// necessite l'installation  de SDL_ttf.h
void aff_pol(char *a_ecrire, int taille, POINT p, COULEUR C)
{
	SDL_Surface *texte = NULL;
	SDL_Rect position;
	TTF_Font *police = NULL;
	SDL_Color Noire   = {  0,   0,   0};
	SDL_Color Blanche = {255, 255, 255};

	SDL_Color Rouge   = {255,   0,   0};
	SDL_Color Verte   = {  0, 255,   0};
	SDL_Color Bleue   = {  0,   0, 255};

	SDL_Color Jaune   = {255, 255,   0};
	SDL_Color Cyan    = {  0, 255, 255};
	SDL_Color Magenta = {255,   0, 255};
	SDL_Color sdl_C;

	sdl_C = Noire;
	if (C == blanc) sdl_C = Blanche;

	if (C == rouge) sdl_C = Rouge;
	if (C == vert)  sdl_C = Verte;
	if (C == bleu)  sdl_C = Bleue;

	if (C == jaune)   sdl_C = Jaune;
	if (C == cyan)    sdl_C = Cyan;
	if (C == magenta) sdl_C = Magenta;

	TTF_Init();

	/* Chargement de la police */
	police = TTF_OpenFont(POLICE_NAME, taille);
	/* Ecriture du texte dans la SDL_Surface "texte" en mode Blended (optimal) */
	if (police) texte = TTF_RenderText_Blended(police, a_ecrire, sdl_C);

	position.x = p.x;
	position.y = p.y;
	if (texte) SDL_BlitSurface(texte, NULL, screen, &position); /* Blit du texte par-dessus */

	//SDL_Flip(screen);
	if (police) TTF_CloseFont(police);
	TTF_Quit();
	if (texte) SDL_FreeSurface(texte);
}
#endif
