#include "editeur.h"
#include "input.h"

/*
 * Met en place l'environement d'edition
*/
int editer()
{
	init_graphics(EDIT_WIDTH, EDIT_HEIGHT, "Editeur Pacman");
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WarpMouse(WIDTH / 2, EDIT_HEIGHT / 2);
	POINT info;
	info.x=WIDTH+10;
	info.y=HEIGHT-100;
	SDL_Rect position;
	position.x=position.y=0;
	int type=0, ok=1, message=AUCUN, tempsPrecedent=0, level=-1, selection=0;
	init_blocks();
	init_editor();
	init_level();
	Input in;
	memset(&in,0,sizeof(in));
	while(ok)
	{
		UpdateEvents(&in);
		position.x = (in.mousex/BLOCK_SIZE)*BLOCK_SIZE;
		position.y = (in.mousey/BLOCK_SIZE)*BLOCK_SIZE;
		if(in.quit) exit(EXIT_SUCCESS);
		if (in.mousebuttons[SDL_BUTTON_LEFT])
		{
			if( (in.mousex >= WIDTH) && (in.mousey < EDIT_WIDTH-1) ) //On click dans le menu de droite
			{
				type = get_block_type(in.mousex, in.mousey, type); //Recupère l'élément choisi
				position.x = in.mousex-BLOCK_SIZE/2; //Centre la texture sur le pointeur
				position.y = in.mousey-BLOCK_SIZE/2;
			}
			else if ( (in.mousex < WIDTH) && (in.mousex >= 0) ) //On veut placer l'objet
				plot_object(in.mousex, in.mousey, type);
		}
		else if (in.mousebuttons[SDL_BUTTON_RIGHT])
		{
			if( (in.mousex < WIDTH) && (in.mousex >= 0) ) {
				if(LEVEL[in.mousey/BLOCK_SIZE][in.mousex/BLOCK_SIZE].type == GHOST) NB_GHOST--;
				LEVEL[in.mousey/BLOCK_SIZE][in.mousex/BLOCK_SIZE].type = RIEN; //On efface la texture
			}
		}
		else if (in.mousebuttons[SDL_BUTTON_WHEELUP])
		{
			type = (type-1)%(NB_ALL_BLOCKS);
			if (type < 0) type = NB_ALL_BLOCKS-1;
		}
		else if (in.mousebuttons[SDL_BUTTON_WHEELDOWN]) type = (type+1)%(NB_ALL_BLOCKS);
		if(in.key[SDLK_ESCAPE])
		{
			in.key[SDLK_ESCAPE]=0;
			selection=edit_menu();
			if(selection==1) //sauver le niveau
			{
				level = select_file_menu();
				if(level!=NB_LEVEL)
				{
					save_level(level);
					message=SAVE;
					tempsPrecedent = SDL_GetTicks();
				}
			}
			else if(selection == 2) //charger un niveau
			{
				level = select_file_menu();
				if(level!=NB_LEVEL)
				{
					load_level(level);
					message=LOAD;
					tempsPrecedent = SDL_GetTicks();
				}
			}
			else if (selection == 3) //supprime tout
			{
				init_level();
				message=DELETE;
				tempsPrecedent = SDL_GetTicks();
			}
			else if (selection == 5) return 0;
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		load_gui(); //Affiche l'interface
		highlight_block(type); //Encadre l'élément actif
		draw_level(); //Dessine le niveau
		if(type<NB_WALL_BLOCKS) SDL_BlitSurface(BLOCK_MUR[type], NULL, screen, &position);
		else if(type<NB_BONUS_BLOCKS+NB_WALL_BLOCKS)
			SDL_BlitSurface(BLOCK_BONUS[type%NB_WALL_BLOCKS], NULL, screen, &position);
		else if(type<NB_BONUS_BLOCKS+NB_WALL_BLOCKS+NB_PACMAN_BLOCK)
			SDL_BlitSurface(BLOCK_PACMAN[type%(NB_WALL_BLOCKS+NB_BONUS_BLOCKS)], NULL, screen, &position);
		else SDL_BlitSurface(BLOCK_GHOST[type%(NB_WALL_BLOCKS+NB_BONUS_BLOCKS+NB_PACMAN_BLOCK)], NULL, screen, &position);
		print_info(&message, tempsPrecedent, info, level); //Affiche un message d'info s'il y en a
		SDL_Flip(screen);
	}
	return 0;
}

void plot_object(int x, int y, int type)
{
	//Si on veut placer un pacman
	if(editor[type].type==PACMAN)
	{
		int i,j;
		//Si un pacman existe déja on doit le supprimer
		for(i=0; i<NB_BLOCKS_HAUTEUR; i++)
		{
			for(j=0; j<NB_BLOCKS_LARGEUR; j++)
			{
				if(LEVEL[i][j].type==PACMAN)
				{
					LEVEL[i][j].type=RIEN;
					break;
				}
			}
		}
	}
	else if(editor[type].type==GHOST)
	{
		if(NB_GHOST == NB_MAX_GHOSTS)
		{
			fprintf(stderr, "Too much ghost, plz remove at least one ghost\n");
			return;
		}
		else if(LEVEL[y/BLOCK_SIZE][x/BLOCK_SIZE].type != GHOST) NB_GHOST++; //Si on plot un fantome sur un autre pas d'incrément
	}
	else if(LEVEL[y/BLOCK_SIZE][x/BLOCK_SIZE].type == GHOST) NB_GHOST--; //Si on efface un fantome
	LEVEL[y/BLOCK_SIZE][x/BLOCK_SIZE] = editor[type];
}

void init_editor()
{
	int i, j;
	for(i=0; i<NB_WALL_BLOCKS; i++)
	{
		editor[i].type=MUR;
		editor[i].nb_elt=1;
		editor[i].position[0]=CENTRE;
		editor[i].elt_type[0]=i;
	}
	for(j=0; j<NB_BONUS_BLOCKS; j++)
	{
		editor[i].type=BONUS;
		editor[i].nb_elt=1;
		editor[i].position[0]=CENTRE;
		editor[i].elt_type[0]=j;
		i++;
	}
	for(j=0; j<NB_PACMAN_BLOCK; j++)
	{
		editor[i].type=PACMAN;
		editor[i].nb_elt=1;
		editor[i].position[0]=CENTRE;
		editor[i].elt_type[0]=j+1;
		i++;
	}
	for(j=0; j<NB_GHOST_BLOCKS; j++)
	{
		editor[i].type=GHOST;
		editor[i].nb_elt=1;
		editor[i].position[0]=CENTRE;
		editor[i].elt_type[0]=j;
		i++;
	}
}

void print_info(int *message, int tempsPrecedent, POINT p, int level)
{
	int tempsActuel;
	char tmp[128];
	sprintf(tmp, "Level %d ", level+1);
	switch(*message)
	{
		case AUCUN:
			break;
		case SAVE:
			tempsActuel = SDL_GetTicks();
			strcat(tmp, "saved !");
			if (tempsActuel - tempsPrecedent < 5000) aff_pol(tmp, FONT_SIZE, p, blanc);
			else *message=AUCUN;
			break;
		case LOAD:
			tempsActuel = SDL_GetTicks();
			strcat(tmp, "loaded !");
			if (tempsActuel - tempsPrecedent < 5000) aff_pol(tmp, FONT_SIZE, p, blanc);
			else *message=AUCUN;
			break;
		case DELETE:
			tempsActuel = SDL_GetTicks();
			if (tempsActuel - tempsPrecedent < 5000) aff_pol("Level destroyed !", FONT_SIZE, p, blanc);
			else *message=AUCUN;
			break;
		default : break;
	}
}

/*
 * Charge l'interface graphique de l'editeur
 * en gros trace des carrée et affiche les
 * textures à l'interieur
*/

/*En version définitive juste créer une texture correspondante*/
void load_gui()
{
	POINT p1, p2;
	SDL_Rect position;
	int i, nb_col = 5, nb_ligne=(NB_ALL_BLOCKS-1)/nb_col + 1;
	p1.x=p2.x=WIDTH; p1.y=HEIGHT-1; p2.y=0;
	draw_line(p1, p2, blanc, screen);
	p2.x=WIDTH+nb_col*(BLOCK_SIZE+3);
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
	for (i=0; i<NB_ALL_BLOCKS; i++)
	{
		position.x=2+WIDTH+((i%5)*(BLOCK_SIZE+3));
		position.y=2+(BLOCK_SIZE+3)*(i/5);
		affiche_une_case(editor[i], &position, screen);
	}
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
int get_block_type(int x, int y, int type)
{
	int col = (x-WIDTH)/(BLOCK_SIZE+3);
	int line = y/(BLOCK_SIZE+3);
	int new_type = line*5 + col;
	if(new_type<NB_ALL_BLOCKS) return new_type;
	else return type;
}
