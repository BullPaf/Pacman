#include "levelmanager.h"

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
*/
void extract_val(char *s, int line)
{
	if(line>=NB_BLOCKS_HAUTEUR) return;
	char nb[3];
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0,i=0,j=0;
	while(nb_val < NB_BLOCKS_LARGEUR) //Tant qu'on a pas lu autant de valeur qu'il n'y a de case pour le niveau
	{
		if(s[i] != ' ') //Si ce n'est pas espace on conserve le caractere lu
		{
			nb[j] = s[i];
			j++; i++;
		}
		else //Si le caractere courant est espace, alors on a lu une valeur
		{
			if(j==1) //Si la valeur ne fait qu'un caractere==>decalage
			{
				nb[1]=nb[0];
				nb[0]='0';
			}
			LEVEL[line][nb_val].block_type = atoi(nb);
			nb_val++; i++; j=0;
		}
	}
	if (nb_val != NB_BLOCKS_LARGEUR)
	{
		fprintf(stderr, "Error, file level not correctly formated: Expected %d values, only %d values were read\n", NB_BLOCKS_LARGEUR, nb_val);
		exit(EXIT_FAILURE);
	}
}

/*
 * Initialize le tableau du niveau
*/
void init_level()
{
	int i,j;
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
			LEVEL[i][j].block_type = -1;
	}
}

/*Affecte à chaque block une texture*/
/*A REVOIR!!!!!!*/
int init_blocks()
{
	int i;
	char img[32];
	for (i=0; i<NB_BLOCKS-2; i++)
	{
		sprintf(img, "%d.bmp", i);
		block[i] = IMG_Load(img);
		//SDL_SetColorKey(block[i], SDL_SRCCOLORKEY, SDL_MapRGB(block[i]->format, 0, 0, 0));
	}
	block[i] = IMG_Load("11.png");
	i++;
	block[i] = IMG_Load("12.png");
	return 1;
}

/*Dessine le niveau à l'écran*/
void draw_level()
{
	int i,j;
	SDL_Rect position;
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		position.y=i*BLOCK_SIZE;
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			position.x=j*BLOCK_SIZE;
			if(LEVEL[i][j].block_type != -1)
				SDL_BlitSurface(block[LEVEL[i][j].block_type], NULL, screen, &position); // Collage de la surface sur l'écran
		}
	}
}

/*
 * Lit un fichier et extrait les valeurs
 * pour affecter le tableau LEVEL
*/
void load_level()
{
	FILE *level_file = fopen("level.txt", "r");
	char chaine[LINE_SIZE];
	int line=0;
	if (level_file != NULL)
	{
		//Lit le fichier ligne par ligne et s'assure qu'on ne lit pas trop de ligne
		while (fgets(chaine, LINE_SIZE, level_file) != NULL && line < NB_BLOCKS_HAUTEUR)
		{
			extract_val(chaine, line); //Recupere les valeurs dans la ligne
			line++;
		}
		fclose(level_file);
	}
}

/*
 * Lit le tableau LEVEL est sauvegarde
 * le niveau dans un fichier
*/
void save_level()
{
	int i,j;
	FILE *level_file = fopen("level.txt", "w+");
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			fprintf(level_file, "%d ", LEVEL[i][j].block_type);
		}
		fputc('\n', level_file);
	}
	fclose(level_file);
}
