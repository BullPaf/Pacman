#include "levelmanager.h"

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
*/
void extract_val(char *s, int line)
{
	if(line>=NB_BLOCKS_HAUTEUR) return;
	char nb[3], type;
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0,nb_elt=0,i=0,j=0;
	while(nb_val < NB_BLOCKS_LARGEUR) //Tant qu'on a pas lu autant de valeur qu'il n'y a de case pour le niveau
	{
		if(s[i] != ' ') //Si ce n'est pas espace on conserve le caractere lu
		{
			type = s[i];
			if(type == '0') //CASE VIDE
			{
				LEVEL[line][nb_val].type = RIEN;
				i++;
			}
			else if(type=='1') //MUR
			{
				LEVEL[line][nb_val].type = MUR;
				i+=2; //On saute les ':'
				while(s[i] != ' ') //tant qu'on a pas d'espace on lit le type de bloc
				{
					nb[j]=s[i];
					j++; i++;
				}
				if(j==1) //Si la valeur ne fait qu'un caractere==>decalage
				{
					nb[1]=nb[0];
					nb[0]='0';
				}
				LEVEL[line][nb_val].elt_type[0]=atoi(nb);
				i++; j=0;
			}
			else if(type=='2') //BONUS
			{
				LEVEL[line][nb_val].type = BONUS;
				i+=2; //On saute les ':'
				while(s[i] != ':') //Délimiteur type de bonus et position
				{
					while(s[i] != ',') //tant qu'on a pas de virgule on lit une valeur
					{
						nb[j]=s[i];
						j++; i++;
					}
					if(j==1) //Si la valeur ne fait qu'un caractere==>decalage
					{
						nb[1]=nb[0];
						nb[0]='0';
					}
					LEVEL[line][nb_val].elt_type[nb_elt]=atoi(nb);
					nb_elt++; i++; j=0;
				}
				i++; nb_elt=0;
				while(s[i] != ' ') //tant qu'on a pas de virgule on lit une valeur
				{
					if(s[i]==',') i++;
					else if(s[i]=='0') LEVEL[line][nb_val].position[nb_elt] = CENTRE;
					else if(s[i]=='1') LEVEL[line][nb_val].position[nb_elt] = HAUT;
					else if(s[i]=='2') LEVEL[line][nb_val].position[nb_elt] = DROITE;
					else if(s[i]=='3') LEVEL[line][nb_val].position[nb_elt] = BAS;
					else if(s[i]=='4') LEVEL[line][nb_val].position[nb_elt] = GAUCHE;
					i++; nb_elt++;
				}
				LEVEL[line][nb_val].nb_elt=nb_elt;
			}
			nb_val ++;
		}
		else i++;
	}
	fprintf(stderr, "Nb val lues = %d\n", nb_val);
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
			LEVEL[i][j].type = RIEN;
	}
}

/*Affecte à chaque block une texture*/
int init_blocks()
{
	int i;
	char img[32];
	for (i=0; i<NB_WALL_BLOCKS; i++)
	{
		sprintf(img, "image/level/%d.png", i);
		BLOCK_MUR[i] = IMG_Load(img);
	}
	for (i=0; i<NB_BONUS_BLOCKS; i++)
	{
		sprintf(img, "image/bonus/%d.png", i);
		BLOCK_BONUS[i] = IMG_Load(img);
	}
	return 1;
}

/*Dessine le niveau à l'écran*/
void draw_level()
{
	fprintf(stderr, "On dessine!\n");
	int i,j;
	SDL_Rect position;
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		position.y=i*BLOCK_SIZE;
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			position.x=j*BLOCK_SIZE;
			if(LEVEL[i][j].type == MUR)
			{
				fprintf(stderr, "La case[%d][%d] est un MUR de type %d\n", i, j, LEVEL[i][j].elt_type[0]);
				SDL_BlitSurface(BLOCK_MUR[LEVEL[i][j].elt_type[0]], NULL, screen, &position); // Collage de la surface sur l'écran
			}
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
		fprintf(stderr, "Fichier lu correctement\n");
		fclose(level_file);
	}
	else
	{
		fprintf(stderr, "Error while opening Level file... Bye!\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * Lit le tableau LEVEL est sauvegarde
 * le niveau dans un fichier
*/
/*void save_level()
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
}*/
