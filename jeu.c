#include "jeu.h"
#include "input.h"

/*TODO
 * Faire une campagne avec une seule initialisation
 * de pacman et des fantomes.
 */
void campagne(config *cfg)
{
	int level=0, result=1, ok=1;
	init_level();
	init_blocks();
	load_level(level);
	Pacman pac;
	Fantome ftm[NB_GHOST];
	Input in;
	init_pacman(&pac, cfg);
	init_ghosts(ftm, cfg);
	memset(&in,0,sizeof(in));
	/*Certainement pas la bonne
	 * facon de faire */
	while(ok)
	{
		//play_menu(level+1);
		result = jouer(&pac, ftm, in, cfg);
		if(result==1) win_menu();
		else if(!result)
		{
			lost_menu();
			ok=0;
		}
		else if(result==2) ok=0;
		if(level < NB_LEVEL-1)
		{
			level++;
			init_level();
			load_level(level);
			pac_restart(&pac);
			init_ghosts(ftm, cfg);
		}
		else ok=0;
	}
}

/*Cette fonction ne doit servir qu'a
 * jouer une seule partie:
 * 1) Ici on doit pouvoir choisir de jouer
 * soit le pacman soit un fantome.
 * 2) On doit pouvoir aussi jouer à plusieurs
 */
int one_level(int level, config *cfg)
{
	//play_menu(level+1);
	init_level();
	init_blocks();
	load_level(level);
	Pacman pac;
	Fantome ftm[NB_GHOST];
	Input in;
	init_pacman(&pac, cfg);
	init_ghosts(ftm, cfg);
	memset(&in,0,sizeof(in));
	return jouer(&pac, ftm, in, cfg);
}

int jouer(Pacman *pac, Fantome *ftm, Input in, config *cfg)
{
	int i, selection=0;
	while(POINTS)
	{
		SDL_Delay(DELAY);
		UpdateEvents(&in);
		if(in.quit) exit(EXIT_SUCCESS);
		if(in.key[SDLK_ESCAPE]) 
		{
			in.key[SDLK_ESCAPE]=0;
			selection=game_menu();
		}
		else if (in.key[SDLK_w]) return 1; //cheat code for winning!!
		else if (in.key[SDLK_l]) return 0; //cheat code for loosing!!

		set_pac_target(pac);
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
		draw_score();
		affiche_pacman(pac);
		affiche_fantomes(ftm);
		if(!pac->nb_lives) return 0;
		if(selection==3) return 2;
		SDL_Flip(screen);
		action(pac, ftm);
	}
	return 1;
}

void action(Pacman *pac, Fantome *ftm)
{
	int i, col;
	SDL_Rect pos;
	for(i=0; i<NB_GHOST; i++) {
		col=check_colision(pac, ftm[i]);
		if(col==1) {
			pac_death(pac);
			for(i=0; i<NB_GHOST; i++) ghost_restart(ftm+i);
			return;
		}
		if(col==2) {
			ghost_death(ftm+i);
			SCORE+=1000;
			return;
		}
	}
	pos=get_case(pac->position, pac->cur_direction);
	if( LEVEL[pos.y][pos.x].type == BONUS && dans_case(pac->position) )
	{
		if(LEVEL[pos.y][pos.x].elt_type==9) {
			SCORE+=100;
			POINTS--;
		}
		else if(LEVEL[pos.y][pos.x].elt_type==0) {
			set_ghosts_eatable(ftm);
		}
		else if(LEVEL[pos.y][pos.x].elt_type==2) {
			if(pac->nb_lives<5) pac->nb_lives++;
			else SCORE+=1000;
		}
		//Pomme-->accélère le pacman
		else if(LEVEL[pos.y][pos.x].elt_type==4) {
			fprintf(stderr, "Manger une pomme\n");
		}
		//Clé-->debloque des passages secrets
		else if(LEVEL[pos.y][pos.x].elt_type==8) {
			pac->nb_keys++;
		}
		LEVEL[pos.y][pos.x].type=RIEN;
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
		else return 0;
	}
	else return 0;
}

//A deplacer
void set_ghosts_eatable(Fantome *ftm)
{
	int i;
	for(i=0; i<NB_GHOST; i++)
	{
		if( !(ftm[i].dead) )
		{
			ftm[i].invinsible = 0;
			ftm[i].speed      = 2;
			//Initialisation du compteur pour compter 5 sec
			ftm[i].counter = SDL_GetTicks();
			//On charge l'image du fantome vulnérable
			ftm[i].num_image  = 8;
		}
	}
}

//A deplacer
void draw_pac_infos(Pacman *pac)
{
	POINT p1;
	SDL_Rect position;
	int i;
	p1.x=WIDTH+10; p1.y=10;
	aff_pol("Lives :", FONT_SIZE, p1, blanc);
	position.y=p1.y+30;
	for(i=0; i<pac->nb_lives; i++)
	{
		position.x=WIDTH+5+i*BLOCK_SIZE;
		SDL_BlitSurface(pac->image[DROITE*2], NULL, screen, &position);
	}
	p1.y+=75;
	aff_pol("Keys :", FONT_SIZE, p1, blanc);
	position.y=p1.y+30;
	for(i=0; i<pac->nb_keys; i++)
	{
		position.x=WIDTH+5+i*BLOCK_SIZE;
		SDL_BlitSurface(BLOCK_BONUS[8], NULL, screen, &position);
	}
}

//A deplacer
void draw_score()
{
	POINT p1;
	char score[50];
	sprintf(score, "Score : %d", SCORE);
	p1.x=WIDTH+10; p1.y=150;
	aff_pol(score, FONT_SIZE, p1, blanc);
	/*sprintf(score, "Level : %d", level+1);
	p1.x=WIDTH+10; p1.y=200;
	aff_pol(score, FONT_SIZE, p1, blanc);*/
}
