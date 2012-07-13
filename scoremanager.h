#ifndef H_SCORE
#define H_SCORE
#include <string.h>
#include <stdio.h>
#include "graphics.h"
#include "input.h"

typedef struct
{
	char name[8];
	int score;
}One;

int draw_result(char*, int);
//int survivor_results(int);
int read_results(char*, One*);
int write_score(char*, One*);
int reset_score(char*);
#endif
