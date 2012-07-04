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

int SCORE;

int draw_result(int);
int read_results(One*);
int write_score(One*);
#endif
