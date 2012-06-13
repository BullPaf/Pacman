#include "levelmanager.h"

int enter_edit_mode()
{
	POINT click;
	SDL_Rect position;
	SDL_Event event;
	position.x=position.y=0;
	int type=-1, ok=1;
	SDL_ShowCursor(SDL_ENABLE);
	SDL_EnableKeyRepeat(0, 10);
	init_blocks();
	init_level();
	load_gui();
	SDL_Flip(screen);
	while(ok)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT : exit(0);
				case SDL_MOUSEBUTTONDOWN :
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						click.x = event.button.x;
						click.y = event.button.y;
						if( (click.x >= WIDTH) && (click.x < EDIT_WIDTH-1) ) //On a choisi un nouvel element
						{
							type = get_block_type(click);
							position.x = click.x-BLOCK_SIZE/2;
							position.y = click.y-BLOCK_SIZE/2;
						}
						else if ( (click.x < WIDTH) && (click.x >= 0) ) //On veut placer l'objet
							LEVEL[click.x/BLOCK_SIZE][click.y/BLOCK_SIZE].block_type = type;
					}
					else if (event.button.button == SDL_BUTTON_RIGHT)
					{
						click.x = event.button.x;
						click.y = event.button.y;
						if( (click.x < WIDTH) && (click.x >= 0) ) //On a choisi un nouvel element
						{
							LEVEL[click.x/BLOCK_SIZE][click.y/BLOCK_SIZE].block_type = -1;
						}
					}
					else if (event.button.button == SDL_BUTTON_WHEELDOWN)   type = (type+1)%NB_BLOCKS;
					else if (event.button.button == SDL_BUTTON_WHEELUP)
					{
						type = (type-1)%NB_BLOCKS;
						if (type < 0) type = 10;
					}
					break;
				case SDL_MOUSEMOTION :
					if(type >= 0)
					{
						position.x = (event.motion.x/BLOCK_SIZE)*BLOCK_SIZE;
						position.y = (event.motion.y/BLOCK_SIZE)*BLOCK_SIZE;
					}
					break;
				case SDL_KEYDOWN :
					if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
					else if (event.key.keysym.sym == SDLK_s)
					{
						save_level();
						fprintf(stdout, "Level saved successfully!\n");
					}
					else if (event.key.keysym.sym == SDLK_l)
					{
						load_level();
						SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
						load_gui();
						draw_level();
						SDL_Flip(screen);
						fprintf(stdout, "Last level loaded successfully!\n");
					}
					else if (event.key.keysym.sym == SDLK_r)
					{
						SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
						init_level();
						load_gui();
						draw_level();
						SDL_Flip(screen);
						fprintf(stdout, "Level destroyed successfully!\n");
					}
					else if (event.key.keysym.sym == SDLK_p)
					{
						return 1;
					}
					break;
				default : break;
			}
		}
		if(type >= 0)
		{
			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
			load_gui();
			highlight_block(type);
			draw_level();
			SDL_BlitSurface(block[type], NULL, screen, &position);
			SDL_Flip(screen);
		}
	}
	return 0;
}

void highlight_block(int type)
{
	POINT p1, p2;
	p1.x=WIDTH+2+(type%5)*(BLOCK_SIZE+3);
	p2.x=p1.x+BLOCK_SIZE+1;
	
	p1.y=HEIGHT-2-(type/5)*(BLOCK_SIZE+3);
	p2.y=p1.y-(BLOCK_SIZE+1);
	draw_rectangle(p1, p2, rouge, screen);
}

int get_block_type(POINT p)
{
	int col = (p.x-WIDTH)/(BLOCK_SIZE+3);
	int line = p.y/(BLOCK_SIZE+3);
	int type = line*5 + col;
	if(type<NB_BLOCKS) return type;
	else return -1;
}

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
		p1.x=p2.x=WIDTH+1+i*(BLOCK_SIZE+3);
		draw_line(p1, p2, blanc, screen);
	}
	for (i=0; i<NB_BLOCKS; i++)
	{
		position.x=2+WIDTH+((i%5)*(BLOCK_SIZE+3));
		p1.x=p2.x=position.x+BLOCK_SIZE+2;
		position.y=2+(BLOCK_SIZE+3)*(i/5);
		SDL_BlitSurface(block[i], NULL, screen, &position);
	}
}

void extract_val(char *s, int line)
{
	if(line>=HEIGHT/BLOCK_SIZE) return;
	char nb[3];
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0,i=0,j=0;
	while(nb_val < WIDTH/BLOCK_SIZE)
	{
		if(s[i] != ' ')
		{
			nb[j] = s[i];
			j++; i++;
		}
		else
		{
			if(j==1)
			{
				nb[1]=nb[0];
				nb[0]='0';
			}
			LEVEL[nb_val][line].block_type = atoi(nb);
			nb_val++; i++; j=0;
		}
	}
}

void init_level()
{
	int i,j;
	for(i=0; i<HEIGHT/BLOCK_SIZE; i++)
	{
		for(j=0; j<WIDTH/BLOCK_SIZE; j++) LEVEL[i][j].block_type = -1;
	}
}

int init_blocks()
{
	int i;
	char img[32];
	for (i=0; i<NB_BLOCKS; i++)
	{
		sprintf(img, "%d.bmp", i);
		block[i] = SDL_LoadBMP(img);
		block[i] = IMG_Load(img);
	}
	return 1;
}

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
			if(LEVEL[j][i].block_type != -1)
				SDL_BlitSurface(block[LEVEL[j][i].block_type], NULL, screen, &position); // Collage de la surface sur l'écran
		}
	}
}

void load_level()
{
	FILE *level_file = fopen("level.txt", "r");
	char chaine[LINE_SIZE];
	int line=0;
	if (level_file != NULL)
	{
		while (fgets(chaine, LINE_SIZE, level_file) != NULL && line < HEIGHT/BLOCK_SIZE)
		{
			extract_val(chaine, line);
			line++;
		}
		fclose(level_file);
	}
}

void save_level()
{
	int i,j;
	FILE *level_file = fopen("level.txt", "w+");
	for(i=0; i<HEIGHT/BLOCK_SIZE; i++)
	{
		for(j=0; j<WIDTH/BLOCK_SIZE; j++)
		{
			fprintf(level_file, "%d ", LEVEL[j][i].block_type);
		}
		fputc('\n', level_file);
	}
	fclose(level_file);
}
