#include "levelmanager.h"

/*
 * Met en place l'environement d'edition
*/
int enter_edit_mode()
{
	POINT click, info;
	SDL_Rect position;
	SDL_Event event;
	position.x=position.y=0;
	int type=0, ok=1, save=0, load=0, delete=0;
	int tempsPrecedent=0, tempsActuel=0;
	SDL_ShowCursor(SDL_ENABLE);
	SDL_EnableKeyRepeat(0, 10);
	init_blocks();
	init_level();
	load_gui();
	SDL_Flip(screen);
	info.x=WIDTH+10;
	info.y=HEIGHT-100;
	while(ok)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT : exit(0);
				case SDL_MOUSEBUTTONDOWN :
					if (event.button.button == SDL_BUTTON_LEFT) //Click gauche
					{
						click.x = event.button.x;
						click.y = event.button.y;
						if( (click.x >= WIDTH) && (click.x < EDIT_WIDTH-1) ) //On click dans le menu de droite
						{
							type = get_block_type(click, type); //Recupère l'élément choisi
							position.x = click.x-BLOCK_SIZE/2; //Centre la texture sur le pointeur
							position.y = click.y-BLOCK_SIZE/2;
						}
						else if ( (click.x < WIDTH) && (click.x >= 0) ) //On veut placer l'objet
							LEVEL[click.y/BLOCK_SIZE][click.x/BLOCK_SIZE].block_type = type;
					}
					else if (event.button.button == SDL_BUTTON_RIGHT) //Click droit
					{
						click.x = event.button.x;
						click.y = event.button.y;
						if( (click.x < WIDTH) && (click.x >= 0) )
							LEVEL[click.y/BLOCK_SIZE][click.x/BLOCK_SIZE].block_type = -1; //On efface la texture
					}
					//Change d'élément avec le scroll de la souris
					else if (event.button.button == SDL_BUTTON_WHEELDOWN)   type = (type+1)%NB_BLOCKS;
					else if (event.button.button == SDL_BUTTON_WHEELUP)
					{
						type = (type-1)%NB_BLOCKS;
						if (type < 0) type = NB_BLOCKS-1;
					}
					break;
				case SDL_MOUSEMOTION : //On déplace la souris
					if(type >= 0) //Si un élément est séléctionné
					{
						position.x = (event.motion.x/BLOCK_SIZE)*BLOCK_SIZE;
						position.y = (event.motion.y/BLOCK_SIZE)*BLOCK_SIZE;
					}
					break;
				case SDL_KEYDOWN :
					if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
					else if (event.key.keysym.sym == SDLK_s) //'s'--> sauver le niveau
					{
						save_level();
						save=1; load=0; delete=0;
						tempsPrecedent = SDL_GetTicks();
					}
					else if (event.key.keysym.sym == SDLK_l) //'l' --> charger le dernier niveau
					{
						load_level();
						SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
						load_gui();
						draw_level();
						load=1; save=0; delete=0;
						tempsPrecedent = SDL_GetTicks();
					}
					else if (event.key.keysym.sym == SDLK_r) //'r' --> supprime tout
					{
						SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
						init_level();
						load_gui();
						draw_level();
						delete=1; save=0; load=0;
						tempsPrecedent = SDL_GetTicks();
					}
					else if (event.key.keysym.sym == SDLK_p) //'p' --> quitte et joue
					{
						return 1;
					}
					break;
				default : break;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		load_gui(); //Affiche l'interface
		highlight_block(type); //Encadre l'élément actif
		draw_level(); //Dessine le niveau
		SDL_BlitSurface(block[type], NULL, screen, &position); //Dessine l'élément actif au niveau de la souris
		if(save)
		{
			tempsActuel = SDL_GetTicks();
			if (tempsActuel - tempsPrecedent < 5000) aff_pol("Level saved !", 25, info, blanc);
			else save = 0;
		}
		else if(load)
		{
			tempsActuel = SDL_GetTicks();
			if (tempsActuel - tempsPrecedent < 5000) aff_pol("Last level loaded !", 25, info, blanc);
			else load = 0;
		}
		else if(delete)
		{
			tempsActuel = SDL_GetTicks();
			if (tempsActuel - tempsPrecedent < 5000) aff_pol("Level destroyed !", 25, info, blanc);
			else delete = 0;
		}
		SDL_Flip(screen);
	}
	return 0;
}

/*
 * Encadre en rouge l'élément correspondant
 * au type
*/
void highlight_block(int type)
{
	POINT p1, p2;
	p1.x=WIDTH+1+(type%5)*(BLOCK_SIZE+3);
	p2.x=p1.x+BLOCK_SIZE+1;
	
	p1.y=HEIGHT-2-(type/5)*(BLOCK_SIZE+3);
	p2.y=p1.y-(BLOCK_SIZE+1);
	draw_rectangle(p1, p2, rouge, screen);
}

/*
 * En fonction de la zone clické
 * Renvoie le type de l'élément correspondant.
 * Si la zone cliqué ne correspond à aucun élément
 * l'ancien type est retourné
*/
int get_block_type(POINT p, int type)
{
	int col = (p.x-WIDTH)/(BLOCK_SIZE+3);
	int line = p.y/(BLOCK_SIZE+3);
	int new_type = line*5 + col;
	if(new_type<NB_BLOCKS) return new_type;
	else return type;
}

/*
 * Charge l'interface graphique de l'editeur
 * en gros trace des carrée et affiche les
 * textures à l'interieur
*/
void load_gui()
{
	POINT p1, p2;
	SDL_Rect position;
	int i;
	p1.x=p2.x=WIDTH;
	p1.y=HEIGHT-1;
	p2.y=0;
	draw_line(p1, p2, blanc, screen);
	p2.x=EDIT_WIDTH-1;
	int nb_ligne=(NB_BLOCKS-1)/5 + 1;
	int nb_col = 5;
	for(i=0; i<=nb_ligne; i++)
	{
		p1.y=p2.y=HEIGHT-(1+i*(BLOCK_SIZE+3));
		draw_line(p1, p2, blanc, screen);
	}
	p1.y=HEIGHT-1;
	for(i=0; i<=nb_col; i++)
	{
		p1.x=p2.x=WIDTH+i*(BLOCK_SIZE+3);
		draw_line(p1, p2, blanc, screen);
	}
	for (i=0; i<NB_BLOCKS; i++)
	{
		position.x=2+WIDTH+((i%5)*(BLOCK_SIZE+3));
		//p1.x=p2.x=position.x+BLOCK_SIZE+2;
		position.y=2+(BLOCK_SIZE+3)*(i/5);
		SDL_BlitSurface(block[i], NULL, screen, &position);
	}
	p1.x=WIDTH+10;
	p1.y=position.y + 50;
	aff_pol("Sauvegarder : s", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("Charger : l", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("Effacer : Clic droit", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("Tout effacer : r", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("Jouer : p", 25, p1, blanc);
	p1.y=p1.y+50;
	aff_pol("Quitter : Echap", 25, p1, blanc);
}

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
*/
void extract_val(char *s, int line)
{
	if(line>=HEIGHT/BLOCK_SIZE) return;
	char nb[3];
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0,i=0,j=0;
	while(nb_val < WIDTH/BLOCK_SIZE) //Tant qu'on a pas lu autant de valeur qu'il n'y a de case pour le niveau
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
	if (nb_val != WIDTH/BLOCK_SIZE)
	{
		fprintf(stderr, "Error, file level not correctly formated: Expected %d values, only %d values were read\n", WIDTH/BLOCK_SIZE, nb_val);
		exit(EXIT_FAILURE);
	}
}

/*
 * Initialize le tableau du niveau
*/
void init_level()
{
	int i,j;
	for(i=0; i<HEIGHT/BLOCK_SIZE; i++)
	{
		for(j=0; j<WIDTH/BLOCK_SIZE; j++)
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
	for(i=0; i<HEIGHT/BLOCK_SIZE; i++)
	{
		position.y=i*BLOCK_SIZE;
		for(j=0; j<WIDTH/BLOCK_SIZE; j++)
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
		while (fgets(chaine, LINE_SIZE, level_file) != NULL && line < HEIGHT/BLOCK_SIZE)
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
	for(i=0; i<HEIGHT/BLOCK_SIZE; i++)
	{
		for(j=0; j<WIDTH/BLOCK_SIZE; j++)
		{
			fprintf(level_file, "%d ", LEVEL[i][j].block_type);
		}
		fputc('\n', level_file);
	}
	fclose(level_file);
}
