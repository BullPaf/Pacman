#include "jeu.h"

/*Permet de jouer tout les niveaux
 * à la suite*/
void campagne(config *cfg, int level)
{
	int result=1;
	SAVE_ENABLE=1;
	Pacman pac;
	Fantome ftm[NB_MAX_GHOSTS];
	Input in;

	init_pacman(&pac, cfg);
	init_blocks();
	memset(&in,0,sizeof(in));
	DELAY = 40-(level-1);

	while(level < NB_LEVEL && result)
	{
		play_menu(level);
		init_level();
		load_level(level);
		result = jouer(&pac, ftm, in, cfg, level);
		if(result==1) win_menu();
		else if(!result) lost_menu();
		else if(result==2) break;
		level++;
		DELAY--; //On accélère youpi!
	}
	if(result != 2) draw_result(pac.score); //Si ce n'est pas un retour volontaire
	delete_pacman(&pac);
	delete_ghosts(ftm);
	delete_blocks();
}

/*Permet de jouer un seul niveau
 * choisi dans la liste des niveaux
 * disponibles*/
void one_level(int level, config *cfg)
{
	if(level>=NB_LEVEL) return;
	SAVE_ENABLE=0;
	play_menu(level);
	Pacman pac;
	Fantome ftm[NB_MAX_GHOSTS];
	Input in;

	init_pacman(&pac, cfg);
	init_level();
	init_blocks();
	load_level(level);
	memset(&in,0,sizeof(in));
	DELAY = 40-(level-1);

	int result = jouer(&pac, ftm, in, cfg, level);
	if(result==1) win_menu();
	else if(!result) lost_menu();
	if(result != 2 && cfg->nb_players>0) draw_result(pac.score);
	delete_pacman(&pac);
	delete_ghosts(ftm);
	delete_blocks();
}

/*Fonction principale du jeu
 * @return
 * 	0 si perdu
 *	1 si gagné
 * 	2 si retour volontaire*/
int jouer(Pacman *pac, Fantome *ftm, Input in, config *cfg, int level)
{
	pac_restart(pac);
	init_ghosts(ftm, cfg);
	int i, selection=0;
	while(POINTS) //Tant que l'on a pas mangé toutes les pac-gommes
	{
		SDL_Delay(DELAY);
		UpdateEvents(&in);
		if(in.quit) //Si clique sur croix
		{
			delete_pacman(pac);
			delete_ghosts(ftm);
			delete_blocks();
			exit(EXIT_SUCCESS);
		}
		if(in.key[SDLK_ESCAPE])
		{
			in.key[SDLK_ESCAPE]=0;
			selection=game_menu();
		}
		else if (in.key[SDLK_w]) return 1; //cheat code for winning!!
		else if (in.key[SDLK_l]) return 0; //cheat code for loosing!!

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
		if(!pac->nb_lives) return 0;
		if(selection==2) return 2;
		else if(selection==1)
		{
			fprintf(stderr, "Saving game!\n");
			save_game(level);
			selection=game_menu();
		}
		SDL_Flip(screen);
		action(pac, ftm);
	}
	return 1;
}

/* Voit si une action peut etre accomplie*/
void action(Pacman *pac, Fantome *ftm)
{
	int i, col;
	SDL_Rect pos;
	//Gagne une vie
	if(pac->score && (pac->score)%10000 == 0 && pac->nb_lives < 5)
	{
		pac->nb_lives++;
		pac->score+=100;
	}
	for(i=0; i<NB_GHOST; i++) {
		col=check_colision(pac, ftm[i]);
		if(col==1) {
			pac_death(pac); //Pacman est mort :(
			for(i=0; i<NB_GHOST; i++) ghost_restart(ftm+i);
			return;
		}
		if(col==2) {
			ghost_death(ftm+i); //Fantome meurt
			pac->score+=500;
			return;
		}
	}
	pos=get_case(pac->position, pac->cur_direction);
	if( LEVEL[pos.y][pos.x].type == BONUS && dans_case(pac->position) )
	{
		if(LEVEL[pos.y][pos.x].elt_type==0) set_ghosts_eatable(ftm); //Super Pac-gomme
		else if(LEVEL[pos.y][pos.x].elt_type==1) pac->score+=200; //Cerise
		else if(LEVEL[pos.y][pos.x].elt_type==2) pac->score+=300; //Fraise
		else if(LEVEL[pos.y][pos.x].elt_type==3) pac->score+=500; //Orange
		else if(LEVEL[pos.y][pos.x].elt_type==4) pac->score+=700; //Pomme
		else if(LEVEL[pos.y][pos.x].elt_type==5) pac->score+=1000; //Melon
		else if(LEVEL[pos.y][pos.x].elt_type==6) pac->score+=2000; //Galboss
		else if(LEVEL[pos.y][pos.x].elt_type==5) pac->score+=3000; //Cloche
		else if(LEVEL[pos.y][pos.x].elt_type==8) { //Clé
			pac->score+=5000;
			pac->nb_keys++;
		}
		if(LEVEL[pos.y][pos.x].elt_type==9) { //Pac-gomme
			//pac->score+=100;
			POINTS--;
		}
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
