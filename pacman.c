#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"

#define STEP 4

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			return *p;
		case 2:
			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32 *)p;
		default:
			return 0;
	}
}

void getPixelColor(int x, int y, SDL_Surface *map, Uint8 *r, Uint8 *g, Uint8 *b)
{
	if(x>=0 && y>=0 && x<WIDTH && y<HEIGHT)
		SDL_GetRGB(getpixel(map, x, y), map->format, r, g, b);
	else // sinon on renvoie noir, pour éviter de planter dans certains cas
		r=g=b=0;
}

void init_pacman(Pacman *pac)
{
	pac->image[1] = SDL_LoadBMP("Pacman_g.bmp");
	pac->image[2] = SDL_LoadBMP("Pacman_h.bmp");
	pac->image[3] = SDL_LoadBMP("Pacman_d.bmp");
	pac->image[4] = SDL_LoadBMP("Pacman_b.bmp");
	pac->position.x = 10;
	pac->position.y = 10;
	pac->position.w = 19;
	pac->position.h = 19;
	pac->cur_direction = 3;
	pac->direction_locked = 0;
	pac->image[0]=pac->image[pac->cur_direction];
}

//Pour afficher Pacman
void affiche_pacman(Pacman *pac, int visible)
{
	if (!visible) SDL_FillRect(screen, &pac->position, noir);
	else SDL_BlitSurface(pac->image[0], NULL, screen, &pac->position); //SDL_FillRect(screen, &pac->position, rouge);
}

int can_go(Pacman *pac, int new_direction)
{
	Uint8 r1,g1,b1,r2,g2,b2;
	switch (new_direction)
	{
		case 3: //Vers la droite
			getPixelColor(pac->position.x+pac->position.w+6, pac->position.y-4, screen, &r1, &g1, &b1);
			getPixelColor(pac->position.x+pac->position.w+6, pac->position.y+pac->position.h+4, screen, &r2, &g2, &b2);
			if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) return 1;
			else return 0;
			break;
		case 1: //Vers la gauche
			getPixelColor(pac->position.x-6, pac->position.y-5, screen, &r1, &g1, &b1);
			getPixelColor(pac->position.x-6, pac->position.y+pac->position.h+5, screen, &r2, &g2, &b2);
			if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) return 1;
			else return 0;
			break;
		case 2: //Vers le haut
			getPixelColor(pac->position.x-5, pac->position.y-6, screen, &r1, &g1, &b1);
			getPixelColor(pac->position.x+pac->position.w+5, pac->position.y-6, screen, &r2, &g2, &b2);
			if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) return 1;
			else return 0;
			break;
		case 4: //Vers le bas
			getPixelColor(pac->position.x-4, pac->position.y+pac->position.h+6, screen, &r1, &g1, &b1);
			getPixelColor(pac->position.x+pac->position.w+4, pac->position.y+pac->position.h+6, screen, &r2, &g2, &b2);
			if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) return 1;
			else return 0;
			break;
		default: break;
	}
	return 0;
}

void test_color()
{
	Uint8 r1,g1,b1;
	POINT click;
	click = wait_clic();
	if (click.x >= 0 && click.y >= 0)
	{
		getPixelColor(click.x, click.y, screen, &r1, &g1, &b1);
		fprintf(stderr, "r=%x, g=%x & b=%x\n", r1, g1, b1);
	}
}

void deplace_pacman(Pacman *pac, int new_direction)
{
	//Si on a rien récupéré
	if (new_direction == 0) new_direction = pac->cur_direction;
	else if (new_direction!=pac->cur_direction) //Si on a pas changé de direction inutile de recharger l'image
	{
		if(can_go(pac, new_direction))
		{
			pac->image[0]=pac->image[new_direction];
			pac->cur_direction = new_direction;
		}
		else new_direction = pac->cur_direction;
	}
	if(new_direction!=pac->direction_locked) //Si la direction n'est pas vérouillée suite à une collision
	{
		Uint8 r1,g1,b1,r2,g2,b2;
		//pac->direction_locked=0; //On débloque l'ancienne direction vérouillée
		switch (new_direction)
		{
			case 3: //Vers la droite
				getPixelColor(pac->position.x+pac->position.w+6, pac->position.y-4, screen, &r1, &g1, &b1);
				getPixelColor(pac->position.x+pac->position.w+6, pac->position.y+pac->position.h+4, screen, &r2, &g2, &b2);
				if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) pac->position.x += STEP;
				//else pac->direction_locked=direction;
				break;
			case 1: //Vers la gauche
				getPixelColor(pac->position.x-6, pac->position.y-5, screen, &r1, &g1, &b1);
				getPixelColor(pac->position.x-6, pac->position.y+pac->position.h+5, screen, &r2, &g2, &b2);
				if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) pac->position.x -= STEP;
				//if (r1!=0x00 || g1!=0x4 || b1!=0xff) pac->position.x -= STEP;
				//else pac->direction_locked=direction;
				break;
			case 2: //Vers le haut
				getPixelColor(pac->position.x-5, pac->position.y-6, screen, &r1, &g1, &b1);
				getPixelColor(pac->position.x+pac->position.w+5, pac->position.y-6, screen, &r2, &g2, &b2);
				if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) pac->position.y -= STEP;
				//else pac->direction_locked=direction;
				break;
			case 4: //Vers le bas
				getPixelColor(pac->position.x-4, pac->position.y+pac->position.h+6, screen, &r1, &g1, &b1);
				getPixelColor(pac->position.x+pac->position.w+4, pac->position.y+pac->position.h+6, screen, &r2, &g2, &b2);
				if( (r1!=0x00 || g1!=0x00 || b1!=0xff) && (r2!=0x00 || g2!=0x00 || b2!=0xff) ) pac->position.y += STEP;
				//else pac->direction_locked=direction;
				break;
			default: break;
		}
	}
}

int isAlive(Pacman *pac)
{
	return 1;
}
