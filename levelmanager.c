#include "levelmanager.h"

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
*/
void extract_val(char *s, int line)
{
	char nb[3], type;
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0,nb_elt=0,i=0,j=0,ghost=0;
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
				while(s[i] != ':') //Délimiteur type de bonus
				{
					while( (s[i] != ',') && (s[i] != ':') ) //tant qu'on a pas de virgule ou de deux points on lit une valeur
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
					if(atoi(nb) == 0) POINTS++; //Si c'est un point on incremente le nombre de POINTS
					nb_elt++; j=0;
				}
				i++; nb_elt=0;
				while(s[i] != ' ') //tant qu'on ai pas arrivé à la fin on lit les positions
				{
					if(s[i]=='0') LEVEL[line][nb_val].position[nb_elt] = CENTRE;
					else if(s[i]=='1') LEVEL[line][nb_val].position[nb_elt] = HAUT;
					else if(s[i]=='2') LEVEL[line][nb_val].position[nb_elt] = DROITE;
					else if(s[i]=='3') LEVEL[line][nb_val].position[nb_elt] = BAS;
					else if(s[i]=='4') LEVEL[line][nb_val].position[nb_elt] = GAUCHE;
					i++; nb_elt++;
				}
				LEVEL[line][nb_val].nb_elt=nb_elt;
				nb_elt=0;
			}
			else if(type=='3') //pac start
			{
				i+=2;
				LEVEL[line][nb_val].type = PACMAN;
				LEVEL[line][nb_val].position[0]=CENTRE;
				PAC_START_X=nb_val;
				PAC_START_Y=line;
				if(s[i]=='1') PAC_START_DIRECTION = HAUT;
				else if(s[i]=='2') PAC_START_DIRECTION = DROITE;
				else if(s[i]=='3') PAC_START_DIRECTION = BAS;
				else if(s[i]=='4') PAC_START_DIRECTION = GAUCHE;
				LEVEL[line][nb_val].elt_type[0]=PAC_START_DIRECTION;
				i++;
			}
			else if(type=='4') //fantome start
			{
				i+=2;
				LEVEL[line][nb_val].type = GHOST;
				LEVEL[line][nb_val].position[0]=CENTRE;
				if(s[i]=='0') ghost=ROUGE;
				else if(s[i]=='1') ghost=VIOLET;
				else if(s[i]=='2') ghost=BLEU;
				else if(s[i]=='3') ghost=JAUNE;
				GHOST_START_X[ghost]=nb_val;
				GHOST_START_Y[ghost]=line;
				LEVEL[line][nb_val].elt_type[0]=ghost;
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
	POINTS=0;
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
	for (i=1; i<NB_PACMAN_BLOCK+1; i++)
	{
		sprintf(img, "image/pacman/%d.png", i);
		BLOCK_PACMAN[i-1] = IMG_Load(img);
	}
	BLOCK_GHOST[0] = IMG_Load("image/ghosts/0.png");
	BLOCK_GHOST[1] = IMG_Load("image/ghosts/8.png");
	BLOCK_GHOST[2] = IMG_Load("image/ghosts/16.png");
	BLOCK_GHOST[3] = IMG_Load("image/ghosts/24.png");
	return 1;
}

void affiche_une_case(CASE c, SDL_Rect *pos, SDL_Surface *s)
{
	if(c.type==MUR) SDL_BlitSurface(BLOCK_MUR[c.elt_type[0]], NULL, s, pos);
	else if(c.type==BONUS)
	{
		int i;
		for (i=0; i<c.nb_elt; i++)
		{
			if(c.position[i]==HAUT) (*pos).y-=BLOCK_SIZE/2;
			else if(c.position[i]==DROITE) (*pos).x+=BLOCK_SIZE/2;
			else if(c.position[i]==BAS) (*pos).y+=BLOCK_SIZE/2;
			else if(c.position[i]==GAUCHE) (*pos).x-=BLOCK_SIZE/2;
			SDL_BlitSurface(BLOCK_BONUS[c.elt_type[i]], NULL, s, pos);
		}
	}
	else if(c.type==PACMAN) SDL_BlitSurface(BLOCK_PACMAN[c.elt_type[0]-1], NULL, s, pos);
	else if(c.type==GHOST) SDL_BlitSurface(BLOCK_GHOST[c.elt_type[0]], NULL, s, pos);
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

int dans_case(SDL_Rect pos)
{
	if( (pos.x % BLOCK_SIZE==0) && (pos.y % BLOCK_SIZE==0) ) return 1;
	else return 0;
}

int can_move(SDL_Rect pos, int new_direction, int cur_direction)
{
	int case_x = pos.x / BLOCK_SIZE, case_y = pos.y / BLOCK_SIZE;
	switch (new_direction)
	{
		case DROITE: //Vers la droite
			if (!dans_case(pos)) {//Si on est entre deux cases
				if( (cur_direction==GAUCHE) || (cur_direction==DROITE) ) return 1;
			}
			else if(LEVEL[case_y][case_x+1].type != MUR) return 1;
			break;
		case GAUCHE: //Vers la gauche
			if(!dans_case(pos)) {
				if( (cur_direction==GAUCHE) || (cur_direction==DROITE) ) return 1;
			}
			else if(LEVEL[case_y][case_x-1].type != MUR) return 1;
			break;
		case HAUT: //Vers le haut
			if(!dans_case(pos)) {
				if( (cur_direction==HAUT) || (cur_direction==BAS) ) return 1;
			}
			else if(LEVEL[case_y-1][case_x].type != MUR) return 1;
			break;
		case BAS: //Vers le bas
			if(!dans_case(pos)) {
				if( (cur_direction==HAUT) || (cur_direction==BAS) ) return 1;
			}
			else if(LEVEL[case_y+1][case_x].type != MUR) return 1;
			break;
		default: break;
	}
	return 0;
}

void move(SDL_Rect *pos, int direction)
{
	switch (direction)
	{
		case DROITE:
			pos->x += STEP;
			break;
		case GAUCHE:
			pos->x -= STEP;
			break;
		case HAUT:
			pos->y -= STEP;
			break;
		case BAS:
			pos->y += STEP;
			break;
		default: break;
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
void save_level()
{
	int i,j,k;
	FILE *level_file = fopen("level.txt", "w+");
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			if(LEVEL[i][j].type==MUR) fprintf(level_file, "1:%d ", LEVEL[i][j].elt_type[0]);
			else if(LEVEL[i][j].type==PACMAN) fprintf(level_file, "3:%d ", LEVEL[i][j].elt_type[0]);
			else if(LEVEL[i][j].type==GHOST) fprintf(level_file, "4:%d ", LEVEL[i][j].elt_type[0]);
			else if(LEVEL[i][j].type==BONUS)
			{
				fprintf(level_file, "2:%d", LEVEL[i][j].elt_type[0]);
				for(k=1; k<LEVEL[i][j].nb_elt; k++) fprintf(level_file, ",%d", LEVEL[i][j].elt_type[k]);
				fprintf(level_file, ":%d", LEVEL[i][j].position[0]);
				for(k=1; k<LEVEL[i][j].nb_elt; k++) fprintf(level_file, ":%d", LEVEL[i][j].position[k]);
				fputc(' ', level_file);
			}
			else fputs("0 ", level_file);
		}
		fputc('\n', level_file);
	}
	fclose(level_file);
}
