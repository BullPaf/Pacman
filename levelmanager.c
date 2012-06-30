#include "levelmanager.h"

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
 * Tres moche mais ca marche alors pas touche!
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
					if(atoi(nb) == 9) POINTS++; //Si c'est un point on incremente le nombre de POINTS
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
			else if(type=='3') //PACMAN
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
				LEVEL[line][nb_val].position[0]=CENTRE;
				if(s[i]=='0') ghost=ROUGE;
				else if(s[i]=='1') ghost=VIOLET;
				else if(s[i]=='2') ghost=BLEU;
				else if(s[i]=='3') ghost=JAUNE;
				GHOST_START_X[NB_GHOST-1]=nb_val;
				GHOST_START_Y[NB_GHOST-1]=line;
				GHOST_COULEUR[NB_GHOST-1]=ghost;
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

/*Initialize le tableau du niveau
 * et les différentes variables*/
void init_level()
{
	int i,j;
	char level[32];
	for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(j=0; j<NB_BLOCKS_LARGEUR; j++)
			LEVEL[i][j].type = RIEN;
	}
	for(i=0; i<NB_LEVEL; i++)
	{
		sprintf(level, "level/level_%d.txt", i+1);
		strcpy(LEVEL_FILE[i], level);
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

//Si on est pile dans une case
int dans_case(SDL_Rect pos)
{
	if( (pos.x % BLOCK_SIZE==0) && (pos.y % BLOCK_SIZE==0) ) return 1;
	else return 0;
}

//Si on peut bouger
int can_move(SDL_Rect pos, int new_direction, int cur_direction, int *key)
{
	int case_x = pos.x / BLOCK_SIZE, case_y = pos.y / BLOCK_SIZE;
	//fprintf(stderr, "Case x = %d && Case y = %d\n", case_x, case_y);
	switch (new_direction)
	{
		case DROITE: //Vers la droite
			if (!dans_case(pos)) {//Si on est entre deux cases
				if( (cur_direction==GAUCHE) || (cur_direction==DROITE) ) return 1;
			}
			else if(case_x == NB_BLOCKS_LARGEUR-1) return 1; //Si on on est contre le bord droit
			else if(LEVEL[case_y][case_x+1].type != MUR) return 1;
			else if(LEVEL[case_y][case_x+1].elt_type[0] >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x+1, case_y);
				remove_bloc(case_y, case_x+1);
				(*key)--;
				return 1;
			}
			break;
		case GAUCHE: //Vers la gauche
			if(!dans_case(pos)) {
				if( (cur_direction==GAUCHE) || (cur_direction==DROITE) ) return 1;
			}
			else if(case_x == 0) return 1; //Si on on est contre le bord gauche
			else if(LEVEL[case_y][case_x-1].type != MUR) return 1;
			else if(LEVEL[case_y][case_x-1].elt_type[0] >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x-1, case_y);
				remove_bloc(case_y, case_x-1);
				(*key)--;
				return 1;
			}
			break;
		case HAUT: //Vers le haut
			if(!dans_case(pos)) {
				if( (cur_direction==HAUT) || (cur_direction==BAS) ) return 1;
			}
			else if(case_y == 0) return 1; //Si on on est contre le bord haut
			else if(LEVEL[case_y-1][case_x].type != MUR) return 1;
			else if(LEVEL[case_y-1][case_x].elt_type[0] >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x, case_y-1);
				remove_bloc(case_y-1, case_x);
				(*key)--;
				return 1;
			}
			break;
		case BAS: //Vers le bas
			if(!dans_case(pos)) {
				if( (cur_direction==HAUT) || (cur_direction==BAS) ) return 1;
			}
			else if(case_y == NB_BLOCKS_HAUTEUR-1) return 1; //Si on on est contre le bord bas
			else if(LEVEL[case_y+1][case_x].type != MUR) return 1;
			else if(LEVEL[case_y+1][case_x].elt_type[0] >= 14 && *key) //Cadenas
			{
				//fprintf(stderr, "Bloc(%d, %d) avec candenas\n", case_x, case_y+1);
				remove_bloc(case_y+1, case_x);
				(*key)--;
				return 1;
			}
			break;
		default: break;
	}
	return 0;
}

//Si on est sur une intersection de chemins
int in_intersection(SDL_Rect pos, int direction)
{
	int case_x = pos.x / BLOCK_SIZE, case_y = pos.y / BLOCK_SIZE;
	if(direction == DROITE || direction == GAUCHE)
	{
		if(dans_case(pos) && (LEVEL[case_y-1][case_x].type != MUR || LEVEL[case_y+1][case_x].type != MUR)) return 1;
	}
	else
	{
		if(dans_case(pos) && (LEVEL[case_y][case_x+1].type != MUR || LEVEL[case_y][case_x-1].type != MUR)) return 1;
	}
	return 0;
}

//Deplace
void move(SDL_Rect *pos, int direction, int speed)
{
	switch (direction)
	{
		case DROITE:
			pos->x += speed;
			if(pos->x+BLOCK_SIZE > WIDTH) pos->x = -BLOCK_SIZE;
			break;
		case GAUCHE:
			pos->x -= speed;
			if(pos->x+BLOCK_SIZE <= 0) pos->x = WIDTH;
			break;
		case HAUT:
			pos->y -= speed;
			if(pos->y+BLOCK_SIZE <= 0) pos->y = HEIGHT;
			break;
		case BAS:
			pos->y += speed;
			if(pos->y+BLOCK_SIZE > HEIGHT) pos->y = -BLOCK_SIZE;
			break;
		default: break;
	}
}

/*Renvoie les index de case en fonction de la position
 * et de la direction*/
SDL_Rect get_case(SDL_Rect position, int direction)
{
	SDL_Rect pos;
	switch(direction)
	{
		case DROITE: //Vers la droite
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

void remove_bloc(int y, int x)
{
	LEVEL[y][x].type=RIEN;
	switch(LEVEL[y][x].elt_type[0])
	{
		case 14:
			if(x && LEVEL[y][x-1].type==MUR) LEVEL[y][x-1].elt_type[0] = remove_right_bloc(LEVEL[y][x-1].elt_type[0]);
			if(x<NB_BLOCKS_LARGEUR && LEVEL[y][x+1].type==MUR)
				LEVEL[y][x+1].elt_type[0] = remove_left_bloc(LEVEL[y][x+1].elt_type[0]);
			break;
		case 15:
			if(y && LEVEL[y-1][x].type==MUR) LEVEL[y-1][x].elt_type[0] = remove_down_bloc(LEVEL[y-1][x].elt_type[0]);
			if(y<NB_BLOCKS_HAUTEUR && LEVEL[y+1][x].type==MUR)
				LEVEL[y+1][x].elt_type[0] = remove_up_bloc(LEVEL[y+1][x].elt_type[0]);
			break;
		case 16:
			if(y<NB_BLOCKS_HAUTEUR && LEVEL[y+1][x].type==MUR)
				LEVEL[y+1][x].elt_type[0] = remove_up_bloc(LEVEL[y+1][x].elt_type[0]);
			break;
		case 17:
			if(x && LEVEL[y][x-1].type==MUR) LEVEL[y][x-1].elt_type[0] = remove_right_bloc(LEVEL[y][x-1].elt_type[0]);
			break;
		case 18:
			if(y && LEVEL[y-1][x].type==MUR) LEVEL[y-1][x].elt_type[0] = remove_down_bloc(LEVEL[y-1][x].elt_type[0]);
			break;
		case 19:
			if(x<NB_BLOCKS_LARGEUR && LEVEL[y][x+1].type==MUR)
				LEVEL[y][x+1].elt_type[0] = remove_left_bloc(LEVEL[y][x+1].elt_type[0]);
			break;
	}
}

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

/*
 * Lit un fichier et extrait les valeurs
 * pour affecter le tableau LEVEL
*/
void load_level(int level)
{
	NB_GHOST=0;
	FILE *level_file = fopen(LEVEL_FILE[level], "r");
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
void save_level(int level)
{
	int i,j,k;
	FILE *level_file = fopen(LEVEL_FILE[level], "w+");
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
