#include "levelmanager.h"

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
 * Tres moche mais ca marche alors pas touche!
 * Tres sensible au format du fichier!
*/
void extract_val(char *s, int line)
{
	char nb[3], type;
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0,i=0,j=0,ghost=0;
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
				LEVEL[line][nb_val].elt_type=atoi(nb);
				i++; j=0;
			}
			else if(type=='2') //BONUS
			{
				LEVEL[line][nb_val].type = BONUS;
				i+=2; //On saute les ':'
				while(s[i] != ' ') //Délimiteur type de bonus
				{
					nb[j]=s[i];
					j++; i++;
				}
				if(j==1) //Si la valeur ne fait qu'un caractere==>decalage
				{
					nb[1]=nb[0];
					nb[0]='0';
				}
				LEVEL[line][nb_val].elt_type=atoi(nb);
				if(atoi(nb) == 9) POINTS++; //Si c'est un point on incremente le nombre de POINTS
				i++; j=0;
			}
			else if(type=='3') //PACMAN
			{
				i+=2;
				LEVEL[line][nb_val].type = PACMAN;
				PAC_START_X=nb_val;
				PAC_START_Y=line;
				if(s[i]=='0') PAC_START_DIRECTION = HAUT;
				else if(s[i]=='1') PAC_START_DIRECTION = DROITE;
				else if(s[i]=='2') PAC_START_DIRECTION = BAS;
				else if(s[i]=='3') PAC_START_DIRECTION = GAUCHE;
				LEVEL[line][nb_val].elt_type=PAC_START_DIRECTION;
				i++;
			}
			else if(type=='4') //GHOSTS
			{
				i+=2;
				if(NB_GHOST < NB_MAX_GHOSTS) NB_GHOST++;
				else
				{
					fprintf(stderr, "Fatal Error: Too much ghosts!\n");
					exit(EXIT_FAILURE);
				}
				LEVEL[line][nb_val].type = GHOST;
				if(s[i]=='0') ghost=ROUGE;
				else if(s[i]=='1') ghost=VIOLET;
				else if(s[i]=='2') ghost=BLEU;
				else if(s[i]=='3') ghost=JAUNE;
				GHOST_START_X[NB_GHOST-1]=nb_val;
				GHOST_START_Y[NB_GHOST-1]=line;
				GHOST_COULEUR[NB_GHOST-1]=ghost;
				LEVEL[line][nb_val].elt_type=ghost;
				i++; 
			}
			nb_val ++;
		}
		else i++;
	}
	if (nb_val != NB_BLOCKS_LARGEUR)
	{
		fprintf(stderr, "Error, file level not correctly formated: Expected %d values, only %d values were read\n", NB_BLOCKS_LARGEUR, nb_val);
		exit(EXIT_FAILURE);
	}
}

/*Initialize le tableau du niveau
 * et les différentes variables*/
void init_level()
{
	int i,j;
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
			LEVEL[i][j].type = RIEN;
	}
	for(i=0; i<NB_MAX_GHOSTS; i++) GHOST_START_Y[i]=GHOST_START_X[i]=-1;
	POINTS=0;
	NB_GHOST=0;
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
	for (i=0; i<NB_PACMAN_BLOCK; i++)
	{
		sprintf(img, "image/pacman/%d.png", i*2);
		BLOCK_PACMAN[i] = IMG_Load(img);
	}
	for (i=0; i<NB_GHOST_BLOCKS; i++)
	{
		sprintf(img, "image/ghosts/%d.png", i*8);
		BLOCK_GHOST[i] = IMG_Load(img);
	}
	return 1;
}

//Affiche une case... ^^
void affiche_une_case(CASE c, SDL_Rect *pos, SDL_Surface *s)
{
	if(c.type==MUR) SDL_BlitSurface(BLOCK_MUR[c.elt_type], NULL, s, pos);
	else if(c.type==BONUS) SDL_BlitSurface(BLOCK_BONUS[c.elt_type], NULL, s, pos);
	else if(c.type==PACMAN) SDL_BlitSurface(BLOCK_PACMAN[c.elt_type], NULL, s, pos);
	else if(c.type==GHOST) SDL_BlitSurface(BLOCK_GHOST[c.elt_type], NULL, s, pos);
}

/*Dessine le niveau à l'écran*/
void draw_level()
{
	int i,j;
	POINT p1, p2;
	SDL_Rect position;
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		position.y=i*BLOCK_SIZE;
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			position.x=j*BLOCK_SIZE;
			affiche_une_case(LEVEL[i][j], &position, screen);
		}
	}
	p1.x=p2.x=WIDTH; p1.y=HEIGHT-1; p2.y=0;
	draw_line(p1, p2, blanc, screen);
}

//Si on est pile dans une case
int dans_case(SDL_Rect pos)
{
	if( (pos.x % BLOCK_SIZE==0) && (pos.y % BLOCK_SIZE==0) ) return 1;
	else return 0;
}

/*Renvoie les index de case en fonction de la position
 * et de la direction*/
SDL_Rect get_case(SDL_Rect position, int direction)
{
	SDL_Rect pos;
	switch(direction)
	{
		case DROITE:
			pos.x = (position.x+BLOCK_SIZE-1)/BLOCK_SIZE;
			pos.y = position.y/BLOCK_SIZE;
			break;
		case BAS:
			pos.x = position.x/BLOCK_SIZE;
			pos.y = (position.y+BLOCK_SIZE-1)/BLOCK_SIZE;
			break;
		default:
			pos.x = position.x/BLOCK_SIZE;
			pos.y = position.y/BLOCK_SIZE;
			break;
	}
	return pos;
}

/*Quand un bloc est supprimé durant la partie
 * modifie les blocs alentours pour conserver
 * une cohérence dans l'agencement des murs*/
void remove_bloc(int y, int x)
{
	LEVEL[y][x].type=RIEN;
	switch(LEVEL[y][x].elt_type)
	{
		case 14:
			if(x && LEVEL[y][x-1].type==MUR) LEVEL[y][x-1].elt_type = remove_right_bloc(LEVEL[y][x-1].elt_type);
			if(x<NB_BLOCKS_LARGEUR && LEVEL[y][x+1].type==MUR)
				LEVEL[y][x+1].elt_type = remove_left_bloc(LEVEL[y][x+1].elt_type);
			break;
		case 15:
			if(y && LEVEL[y-1][x].type==MUR) LEVEL[y-1][x].elt_type = remove_down_bloc(LEVEL[y-1][x].elt_type);
			if(y<NB_BLOCKS_HAUTEUR && LEVEL[y+1][x].type==MUR)
				LEVEL[y+1][x].elt_type = remove_up_bloc(LEVEL[y+1][x].elt_type);
			break;
		case 16:
			if(y<NB_BLOCKS_HAUTEUR && LEVEL[y+1][x].type==MUR)
				LEVEL[y+1][x].elt_type = remove_up_bloc(LEVEL[y+1][x].elt_type);
			break;
		case 17:
			if(x && LEVEL[y][x-1].type==MUR) LEVEL[y][x-1].elt_type = remove_right_bloc(LEVEL[y][x-1].elt_type);
			break;
		case 18:
			if(y && LEVEL[y-1][x].type==MUR) LEVEL[y-1][x].elt_type = remove_down_bloc(LEVEL[y-1][x].elt_type);
			break;
		case 19:
			if(x<NB_BLOCKS_LARGEUR && LEVEL[y][x+1].type==MUR)
				LEVEL[y][x+1].elt_type = remove_left_bloc(LEVEL[y][x+1].elt_type);
			break;
	}
}

/*********Fonctions de remplacement des blocs**************/
int remove_right_bloc(int elt)
{
	if(elt==0) return 11;
	else if(elt==3) return 10;
	else if(elt==5) return 12;
	else if(elt==6) return 2;
	else if(elt==7) return 1;
	else if(elt==8) return 4;
	else if(elt==14) return 17;
	else return elt;
}

int remove_left_bloc(int elt)
{
	if(elt==0) return 13;
	else if(elt==2) return 12;
	else if(elt==4) return 10;
	else if(elt==6) return 5;
	else if(elt==8) return 3;
	else if(elt==9) return 1;
	else if(elt==14) return 19;
	else return elt;
}

int remove_up_bloc(int elt)
{
	if(elt==1) return 10;
	else if(elt==2) return 11;
	else if(elt==5) return 13;
	else if(elt==6) return 0;
	else if(elt==7) return 3;
	else if(elt==9) return 4;
	else if(elt==15) return 16;
	else return elt;
}

int remove_down_bloc(int elt)
{
	if(elt==1) return 12;
	else if(elt==3) return 13;
	else if(elt==4) return 11;
	else if(elt==7) return 5;
	else if(elt==8) return 0;
	else if(elt==9) return 2;
	else if(elt==15) return 18;
	else return elt;
}
/*******************FIN Fonctions de remplacement de blocs*********************/

/*
 * Lit un fichier et extrait les valeurs
 * pour affecter le tableau LEVEL
*/
void load_level(int level)
{
	NB_GHOST=0;
	char myfile[64];
	sprintf(myfile, "%s%s", LEVEL_PATH, LEVEL_FILE[level]);
	FILE *level_file = fopen(myfile, "r");
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
	else
	{
		fprintf(stderr, "Error while opening Level file... Bye!\n");
		exit(EXIT_FAILURE);
	}
}

/*Sauve juste le numéro du niveau actuel pour le moment*/
void save_game(int cur_level)
{
	FILE *save_file = fopen("save/save.txt", "w+");
	if (save_file != NULL)
	{
		fprintf(save_file, "%d", cur_level);
		fclose(save_file);
	}
	else fprintf(stderr, "Error while saving game...\n");
}

/*Charge le dernier niveau sauvegardé*/
int load_game()
{
	int level;
	FILE *save_file = fopen("save/save.txt", "r");
	if (save_file != NULL)
	{
		if(fscanf(save_file, "%d", &level)==EOF) return -1;
	}
	else fprintf(stderr, "No game saved...\n");
	return level;
}

/*Si une sauvegarde existe*/
int has_saved_game()
{
	FILE *save_file = fopen("save/save.txt", "r");
	if (save_file != NULL)
	{
		fclose(save_file);
		return 1;
	}
	else return 0;
}

/*
 * Lit le tableau LEVEL est sauvegarde
 * le niveau dans un fichier
*/
void save_level(int level)
{
	int i,j;
	char myfile[64];
	sprintf(myfile, "%s%s", LEVEL_PATH, LEVEL_FILE[level]);
	FILE *level_file = fopen(myfile, "w+");
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			if(LEVEL[i][j].type==MUR) fprintf(level_file, "1:%d ", LEVEL[i][j].elt_type);
			else if(LEVEL[i][j].type==PACMAN) fprintf(level_file, "3:%d ", LEVEL[i][j].elt_type);
			else if(LEVEL[i][j].type==GHOST) fprintf(level_file, "4:%d ", LEVEL[i][j].elt_type);
			else if(LEVEL[i][j].type==BONUS) fprintf(level_file, "2:%d ", LEVEL[i][j].elt_type);
			else fputs("0 ", level_file);
		}
		fputc('\n', level_file);
	}
	fclose(level_file);
}

void delete_blocks()
{
	int i;
	for (i=0; i<NB_WALL_BLOCKS; i++)
		free(BLOCK_MUR[i]);
	for (i=0; i<NB_BONUS_BLOCKS; i++)
		free(BLOCK_BONUS[i]);
	for (i=0; i<NB_PACMAN_BLOCK; i++)
		free(BLOCK_PACMAN[i]);
	for (i=0; i<NB_GHOST_BLOCKS; i++)
		free(BLOCK_GHOST[i]);
}
