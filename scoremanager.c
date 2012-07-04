#include "scoremanager.h"

int draw_result()
{
	POINT p1;
	One p[10];
	Input in;
	memset(&in,0,sizeof(in));
	int i=0, j;
	char chaine[512];
	FILE *result_file = fopen("data/results.txt", "r");
	if(result_file != NULL)
	{
		while (fgets(chaine, 512, result_file) != NULL)
		{
			if(fscanf(result_file, "%s %d", p[i].name, &(p[i].score))) i++;
		}
		if(i<10) //On a donc lu moins de 10 score
		{
			for(j=i; j<10; j++)
			{
				strcpy(p[j].name, "A B C");
				p[j].score = 0;
			}
		}
		//Ici on est sur d'avoir 10 scores
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		p1.x=200; p1.y=50;
		aff_pol("SCORE", 50, p1, jaune);
		for(i=0; i<10; i++)
		{
			p1.y+=50;
			sprintf(chaine, "%s..............................................%d", p[i].name, p[i].score);
			aff_pol(chaine, 30, p1, jaune);
		}
		SDL_Flip(screen);
		while(!in.quit)
		{
			UpdateEvents(&in);
			if(in.key[SDLK_ESCAPE]) return 1;
		}
	}
	else
	{
		fprintf(stderr, "Error while opening results file, sorry no results this time\n");
		return 0;
	}
	return 1;
}
