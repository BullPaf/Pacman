#include "scoremanager.h"

int read_results(char *file, One *p)
{
	int i=0;
	FILE *result_file = fopen(file, "r");
	if(result_file != NULL)
	{
		while (fscanf(result_file, "%s %d", p[i].name, &(p[i].score)) != EOF) i++;
		if(i!=10)
		{
			fclose(result_file);
			reset_score(file);
			fprintf(stderr, "Oups, format file for score file is wrong... Reset all score plz!\n");
			return 0;
		}
		fclose(result_file);
		return 1;
	}
	else
	{
		fprintf(stderr, "Error while opening results file, sorry no results this time\n");
		return 0;
	}
}

int write_score(char *file, One *p)
{
	int i;
	FILE *result_file = fopen(file, "w+");
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

int reset_score(char *file)
{
	int i;
	FILE *result_file = fopen(file, "w+");
	if(result_file != NULL)
	{
		for(i=0; i<10; i++) fputs("ABC 0\n", result_file);
		fclose(result_file);
		fprintf(stderr, "Score successfully reset\n");
		return 1;
	}
	else
	{
		fprintf(stderr, "Error cannot reset score...\n");
		return 0;
	}
}

int draw_result(char *file, int score)
{
	POINT p1,p2;
	int i=0, j=9, newscore=-1;
	One p[10];
	char chaine[128];
	char title[32];
	if(!read_results(file, p)) return 0;
	//Cherche si on a fait un record
	for(i=0; i<10; i++)
	{
		if(score > p[i].score)
		{
			newscore=i;
			break;
		}
	}
	//Si record on decale tout le monde
	if(newscore >= 0)
	{
		while(j != newscore)
		{
			p[j].score=p[j-1].score;
			strcpy(p[j].name, p[j-1].name);
			j--;
		}
		p[newscore].score=score;
		for(i=0; i<8; i++) p[newscore].name[i]='\0';
		strcpy(title, "NEW SCORE!");
	}
	else strcpy(title, "SCORE");
	//Affichage des scores
	Input in;
	memset(&in,0,sizeof(in));
	while(!in.quit)
	{
		UpdateEvents(&in);
		if(in.key[SDLK_RETURN])
		{
			if(!write_score(file, p)) return 0;
			else return 1;
		}
		else if(in.key[SDLK_ESCAPE]) return 0;
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		p1.x=200; p1.y=50;
		p2.x=400; p2.y=p1.y;
		aff_pol(title, 50, p1, jaune);
		if(newscore >= 0) print_key(p[newscore].name, &in, 8); //permet de taper son nom
		for(i=0; i<10; i++)
		{
			p1.y+=50; p2.y=p1.y;
			if(i==newscore) aff_pol(p[newscore].name, 30, p1, jaune);
			else aff_pol(p[i].name, 30, p1, jaune);
			sprintf(chaine, "%d", p[i].score);
			aff_pol(chaine, 30, p2, jaune);
		}
		SDL_Flip(screen);
	}
	fprintf(stderr, "Score not saved, reason: Unknown!\n");
	return 0;
}
