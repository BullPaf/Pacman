#include "jeu.h"

/*Permet de jouer tout les niveaux
 * à la suite*/
void campagne(config *cfg, int level)
{
	int selection=0;
	SAVE_ENABLE=1;
	Pacman pac;
	Fantome ftm[NB_MAX_GHOSTS];
	score_message *msg_list = NULL;
	Input in;

	init_pacman(&pac, cfg);
	init_blocks();
	memset(&in,0,sizeof(in));
	DELAY = 40-level;

	while(level < NB_LEVEL)
	{
		play_menu(level);
		init_level();
		load_level(level);
		pac_restart(&pac);
		init_ghosts(ftm, cfg);
		while(POINTS) //Tant que l'on a pas mangé toutes les pac-gommes
		{
			UpdateEvents(&in);
			if(in.quit) //Si clique sur croix
			{
				delete(&pac, ftm);
				exit(EXIT_SUCCESS);
			}
			while(in.key[SDLK_ESCAPE])
			{
				selection=game_menu();
				if(selection==0) in.key[SDLK_ESCAPE]=0;
				else if(selection==1) save_game(level);
				else if(selection==2) //Retour menu principal
				{
					delete(&pac, ftm);
					return;
				}
			}
			if (in.key[SDLK_w]) POINTS=0; //cheat code for winning!!
			else if (in.key[SDLK_l] || !(pac.nb_lives)) //cheat code for loosing!!
			{
				lost_menu();
				draw_result("data/results.txt", pac.score);
				delete(&pac, ftm);
				return;
			}
			jouer(&pac, ftm, in, cfg, level, &msg_list);
		}
		level++;
		win_menu();
		DELAY--; //On accélère youpi!*/
	}
	draw_result("data/results.txt", pac.score);
	delete(&pac, ftm);
}

/*Permet de jouer un seul niveau
 * choisi dans la liste des niveaux
 * disponibles*/
void one_level(int level, config *cfg)
{
	if(level>=NB_LEVEL) return;
	int selection;
	SAVE_ENABLE=0;
	play_menu(level);
	Pacman pac;
	Fantome ftm[NB_MAX_GHOSTS];
	score_message *msg_list = NULL;
	Input in;

	init_pacman(&pac, cfg);
	init_level();
	init_blocks();
	load_level(level);
	pac_restart(&pac);
	init_ghosts(ftm, cfg);
	memset(&in,0,sizeof(in));
	DELAY = 40-level;
	while(POINTS) //Tant que l'on a pas mangé toutes les pac-gommes
	{
		UpdateEvents(&in);
		if(in.quit) //Si clique sur croix
		{
			delete(&pac, ftm);
			exit(EXIT_SUCCESS);
		}
		while(in.key[SDLK_ESCAPE])
		{
			selection=game_menu();
			if(selection==0) in.key[SDLK_ESCAPE]=0;
			else if(selection==2) //Retour menu principal
			{
				delete(&pac, ftm);
				return;
			}
		}
		if (in.key[SDLK_w]) POINTS=0; //cheat code for winning!!
		else if (in.key[SDLK_l] || !(pac.nb_lives)) //cheat code for loosing!!
		{
			lost_menu();
			delete(&pac, ftm);
			if(pacmanIsHuman(cfg)) draw_result("data/results.txt", pac.score);
			return;
		}
		jouer(&pac, ftm, in, cfg, level, &msg_list);
	}
	win_menu();
	if(pacmanIsHuman(cfg)) draw_result("data/results.txt", pac.score);
	delete(&pac, ftm);
}

/*Ici les fantomes accélèrent de plus en plus
 * et le but est de tenir le plus longtemps
 * sans se faire dévorer*/
void survivor(int level, config *cfg)
{
	if(level>=NB_LEVEL) return;
	int selection, counter=SDL_GetTicks(), tmp=counter, elapsed;
	SAVE_ENABLE=0;
	play_menu(level);
	Pacman pac;
	Fantome ftm[NB_MAX_GHOSTS];
	score_message *msg_list = NULL;
	Input in;

	init_pacman(&pac, cfg);
	init_level();
	init_blocks();
	load_level(level);
	pac_restart(&pac);
	pac.nb_lives=1;
	init_ghosts(ftm, cfg);
	memset(&in,0,sizeof(in));
	DELAY = 40;
	while(pac.nb_lives && POINTS)
	{
		elapsed = SDL_GetTicks()-tmp;
		if(elapsed > 10000)
		{
			//On accélere les fantomes
			speed_up(ftm, 1);
			tmp=SDL_GetTicks();
		}
		UpdateEvents(&in);
		if(in.quit) //Si clique sur croix
		{
			delete(&pac, ftm);
			exit(EXIT_SUCCESS);
		}
		while(in.key[SDLK_ESCAPE])
		{
			selection=game_menu();
			if(selection==0) in.key[SDLK_ESCAPE]=0;
			else if(selection==2) //Retour menu principal
			{
				delete(&pac, ftm);
				return;
			}
		}
		jouer(&pac, ftm, in, cfg, level, &msg_list);
	}
	counter = SDL_GetTicks() - counter;
	fprintf(stderr, "Wouaw tu as tenu %d ms!\n", counter);
	if(pacmanIsHuman(cfg)) draw_result("data/survivor.txt", counter);
	delete(&pac, ftm);
}

/*Fonction du jeu*/
void jouer(Pacman *pac, Fantome *ftm, Input in, config *cfg, int level, score_message **msg_list)
{
	int i;
	SDL_Delay(DELAY);
	set_pac_target(pac); //Cherche l'endroit ou aller pour pacman
	//Fonction de déplacement de pacman
	pac->controllerFonction(in, *cfg, pac->controlled_by, &(pac->position), &(pac->cur_direction), &(pac->nb_keys), &(pac->speed), &(pac->num_image), pac->target);
	for(i=0; i<NB_GHOST; i++)
	{
		set_ftm_target(ftm+i, pac->position);
		ftm[i].controllerFonction(in, *cfg, ftm[i].controlled_by, &(ftm[i].position), &(ftm[i].cur_direction), &(ftm[i].nb_keys), &(ftm[i].speed), &(ftm[i].num_image), ftm[i].target);
	}
	updatePacman(pac);
	updateGhosts(ftm);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	draw_level();
	draw_pac_infos(pac);
	draw_score(pac->score, level);
	affiche_pacman(pac);
	affiche_fantomes(ftm);
	action(pac, ftm, msg_list);
	display_messages(msg_list);
	SDL_Flip(screen);
}

/* Voit si une action peut etre accomplie*/
void action(Pacman *pac, Fantome *ftm, score_message **msg_list)
{
	int i, col;
	SDL_Rect pos = get_case(pac->position, pac->cur_direction);
	//Gagne une vie
	if(pac->score && (pac->score)%10000 == 0 && pac->nb_lives < 5)
	{
		pac->nb_lives++;
		pac->score+=100;
	}
	//Verifie si pacman et fantomes se rencontrent
	for(i=0; i<NB_GHOST; i++) {
		col=check_colision(pac, ftm[i]);
		if(col==1) {
			pac_death(pac); //Pacman est mort :(
			for(i=0; i<NB_GHOST; i++) ghost_restart(ftm+i);
			return;
		}
		if(col==2) {
			ghost_death(ftm+i); //Fantome meurt
			pac->ghost_eaten++;
			pac->score+=200*(pac->ghost_eaten);
			add_new_message(msg_list, 9+pac->ghost_eaten, pos);
			return;
		}
	}
	if( LEVEL[pos.y][pos.x].type == BONUS && dans_case(pac->position) )
	{
		if(LEVEL[pos.y][pos.x].elt_type==0) //Super Pac-gomme
		{
			set_ghosts_eatable(ftm); 
			pac->counter=SDL_GetTicks();
		}
		else if(LEVEL[pos.y][pos.x].elt_type==1) pac->score+=100; //Cerise
		else if(LEVEL[pos.y][pos.x].elt_type==2) pac->score+=300; //Fraise
		else if(LEVEL[pos.y][pos.x].elt_type==3) pac->score+=500; //Orange
		else if(LEVEL[pos.y][pos.x].elt_type==4) pac->score+=700; //Pomme
		else if(LEVEL[pos.y][pos.x].elt_type==5) pac->score+=1000; //Melon
		else if(LEVEL[pos.y][pos.x].elt_type==6) pac->score+=2000; //Galboss
		else if(LEVEL[pos.y][pos.x].elt_type==7) pac->score+=3000; //Cloche
		else if(LEVEL[pos.y][pos.x].elt_type==8) { //Clé
			pac->score+=5000;
			pac->nb_keys++;
		}
		if(LEVEL[pos.y][pos.x].elt_type==9) POINTS--; //Pac-gomme
		add_new_message(msg_list, LEVEL[pos.y][pos.x].elt_type, pos);
		LEVEL[pos.y][pos.x].type=RIEN; //On l'a mangé bravo!
	}
}

/*Detecteur de colision a améliorer!!*/
int check_colision(Pacman *pac, Fantome f)
{
	SDL_Rect pac_case = get_case(pac->position, pac->cur_direction);
	SDL_Rect ftm_case = get_case(f.position, f.cur_direction);
	//S'ils sont sur la meme case alors colision /!\ En cas de changement de case entre 2 deplacements
	//pas de colision détécté.
	if(pac_case.x == ftm_case.x && pac_case.y == ftm_case.y)
	{
		if(f.invinsible) return 1; //Pacman se fait manger
		else if(!f.dead) return 2; //Fantome se fait manger
		else return 0; //Si Pac croise un fantome déja mort
	}
	else return 0;
}

/*Affiche le score et le niveau actuel*/
void draw_score(int score, int level)
{
	POINT p1;
	char score_c[50];
	sprintf(score_c, "Score : %d", score);
	p1.x=WIDTH+10; p1.y=150;
	aff_pol(score_c, FONT_SIZE, p1, blanc);
	sprintf(score_c, "Level : %d", level+1);
	p1.x=WIDTH+10; p1.y=200;
	aff_pol(score_c, FONT_SIZE, p1, blanc);
}

/*recupere tout les niveaux disponibles
 * dans le dossier level.
 * Ils sont de base dispo en mode partie unique*/
void init_game()
{
	NB_LEVEL=0;
	//CAMPAGNE_LEVEL=0;
	//char dirname[64];
	//strcpy(dirname, "level/");
	DIR *dp = opendir(LEVEL_PATH);
	if (dp) {
		struct dirent *entry;
		while ( (entry = readdir(dp)) ) {
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
					continue;
			else
			{
				//printf("L'entrée lue s'appelle '%s'\n", entry->d_name); 
				strcpy(LEVEL_FILE[NB_LEVEL], entry->d_name);
				NB_LEVEL++;
			}
		}
		closedir(dp);
	}
	/*FILE *cmp=fopen("data/campagne.txt", "r");
	if(cmp != NULL)
	{
		while (fscanf(cmp, "%s", CAMPAGNE[CAMPAGNE_LEVEL]) != EOF) CAMPAGNE_LEVEL++;
	}
	else
	{
		fprintf(stderr, "Cant read campagne file, campagne won't be available\n");
	}*/
}

/*Le nouveau message est toujours inseré en tete de liste*/
void add_new_message(score_message **msg_list, int type, SDL_Rect p)
{
	if(type==0 || type==9) return; //Si ce sont des pac gommes
	score_message *new_msg = malloc(sizeof(score_message));
	char img_point[32];
	sprintf(img_point, "image/score/%d.png", type-1);
	new_msg->img = IMG_Load(img_point);
	new_msg->elapsed = SDL_GetTicks();
	new_msg->pos.x = (p.x)*BLOCK_SIZE;
	new_msg->pos.y = (p.y)*BLOCK_SIZE;
	new_msg->next = *msg_list;
	*msg_list=new_msg;
}

/*Pacours la liste et affiche les messages s'il y en a,
 * quand un message a expiré il est retiré de la liste*/
void display_messages(score_message **msg_list)
{
	score_message *tmp=*msg_list;
	score_message *tmp2=*msg_list;
	while(tmp!=NULL)
	{
		int tempsEcoule = SDL_GetTicks()-(tmp->elapsed);
		if(tempsEcoule < 2000)
		{
			SDL_BlitSurface(tmp->img, NULL, screen, &(tmp->pos));
			(tmp->pos.y)--;
		}
		else if(tmp==*msg_list) //Si c'est l'élément de tete
		{
			free((*msg_list)->img);
			*msg_list = (*msg_list)->next;
		}
		else if(tmp->next==NULL) //Le dernier element
		{
			tmp2->next=NULL;
			free(tmp->img);
			free(tmp);
		}
		tmp2=tmp;
		tmp=tmp->next;
	}
}

int pacmanIsHuman(config *cfg)
{
	int i;
	for(i=0; i<cfg->nb_players; i++)
	{
		if(cfg->players[i].character==PACMAN) return 1;
	}
	return 0;
}

void delete(Pacman *pac, Fantome*ftm)
{
	delete_pacman(pac);
	delete_ghosts(ftm);
	delete_blocks();
}
