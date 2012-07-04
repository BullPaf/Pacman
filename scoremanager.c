#include "scoremanager.h"

int read_results(One *p)
{
	int i=0;
	FILE *result_file = fopen("data/results.txt", "r");
	if(result_file != NULL)
	{
		while (fscanf(result_file, "%s %d", p[i].name, &(p[i].score)) != EOF) i++;
		fclose(result_file);
		return 1;
	}
	else
	{
		fprintf(stderr, "Error while opening results file, sorry no results this time\n");
		return 0;
	}
}

int write_score(One *p)
{
	int i;
	FILE *result_file = fopen("data/results.txt", "w+");
	if(result_file != NULL)
	{
		for(i=0; i<10; i++) fprintf(result_file, "%s %d\n", p[i].name, p[i].score);
		fclose(result_file);
		return 1;
	}
	else
	{
		fprintf(stderr, "Error while opening results file, sorry no results this time\n");
		return 0;
	}
}

int draw_result(int score)
{
	POINT p1;
	int i=0, j=9, newscore=-1;
	One p[10];
	char chaine[128];
	char title[32];
	if(!read_results(p)) return 0;
	for(i=0; i<10; i++)
	{
		if(score > p[i].score)
		{
			newscore=i;
			break;
		}
	}
	if(newscore >= 0)
	{
		while(j != newscore)
		{
			p[j].score=p[j-1].score;
			strcpy(p[j].name, p[j-1].name);
			j--;
		}
		p[newscore].score=score;
		sprintf(p[newscore].name, "%d", newscore);
		strcpy(title, "NEW SCORE!");
		if(!write_score(p)) return 0;
	}
	else strcpy(title, "SCORE");
	Input in;
	memset(&in,0,sizeof(in));
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	p1.x=200; p1.y=50;
	aff_pol(title, 50, p1, jaune);
	for(i=0; i<10; i++)
	{
		p1.y+=50;
		sprintf(chaine, "%s                                                         %d", p[i].name, p[i].score);
		aff_pol(chaine, 30, p1, jaune);
	}
	SDL_Flip(screen);
	while(!in.quit)
	{
		UpdateEvents(&in);
		if(in.key[SDLK_ESCAPE] || in.key[SDLK_RETURN]) return 1;
	}
	return 1;
}
