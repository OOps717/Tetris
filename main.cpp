#include <SDL.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iterator>
#include <map>
#include "button.h"
#include "solo.h"
#include "ordi.h"

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
SDL_Surface* but1_img = NULL;
SDL_Surface* but2_img = NULL;
SDL_Surface* opt_img = NULL;
SDL_Surface* logo_img = NULL;
SDL_Surface* scoreboard = NULL;
SDL_Surface* bRec = NULL;
SDL_Surface* affSB = NULL;
SDL_Surface* tabSB = NULL;

// coordonnées des sprites dans l'image
// x,y, w,h (0,0) en haut a gauche
SDL_Rect srcBg = { 0,128, 96,128 };

Button* b1;
Button* b2;
Button* opt;
Button* tabScore;

Solo* solo;
Ordi* ordi;

bool added = false;
bool stop = false;
unsigned int lastTime = 0, currentTime;

float speed[30];
int lv;
float timeAdd;
float frameAdd;

char mode;

std::map<int, int> record;

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

void init()
{
	//Remplissage tableau de vitesse en fonction du niveau
	// Les valeurs pour 0, 18 et plus sont les vrais
	//	pour les autres nous n'avons pas trouvé de données.
	for (int i = 0; i < 30; i++) {
		if (i <= 10) {
			speed[i] = 800 - 50 * i;
		}
		else {
			if (i <=17 ) {
				speed[i] = 300 - 32 * (i - 10);
			}
			else {
				if (i == 18)
					speed[i] = 50;
				else {
					if (i != 29)
						speed[i] = 25;
					else
						speed[i] = 12.5;
				}
			}
		}

	}

	// creation fenetre
	pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
	win_surf = SDL_GetWindowSurface(pWindow);

	// creation d'une "surface" à patir d'une image
	plancheSprites = SDL_LoadBMP("background.bmp");
	but1_img = SDL_LoadBMP("solo.bmp");
	but2_img = SDL_LoadBMP("vs.bmp");
	opt_img = SDL_LoadBMP("options.bmp");
	logo_img = SDL_LoadBMP("tetris.bmp");
	bRec = SDL_LoadBMP("record.bmp");

	// modification de taille de bouton pour que il a la même taille que le sprite
	b1 = new Button(150, 500, but1_img->w, but1_img->h, "solo");
	b2 = new Button(550, 500, but2_img->w, but2_img->h, "ordi");
	opt = new Button(0, 0, opt_img->w, opt_img->h, "options");
	tabScore = new Button(340, 800, bRec->w, bRec->h,"Scoreboard");

	// definit la souleur transparente
	SDL_SetColorKey(plancheSprites, true, 0);  // 0: 00/00/00 noir -> transparent
}

// fonction qui met à jour la surface de la fenetre "win_surf"
void draw(double dt)
{
	// remplit le fond
	SDL_Rect dest = { 0,0,0,0 };
	for (int j = 0; j < win_surf->h; j += 150)
		for (int i = 0; i < win_surf->w; i += 150)
		{
			dest.x = i;
			dest.y = j;
			// copie depuis la planche de sprite vers la fenetre
			SDL_BlitSurface(plancheSprites, NULL, win_surf, &dest);
		}

	SDL_Rect pos = { win_surf->w/4+win_surf->w/40,win_surf->h/4,0,0 };
	if(SDL_BlitSurface(logo_img, NULL, win_surf, &pos)==0)
		SDL_BlitSurface(logo_img, NULL, win_surf, &pos);

	if(b1 != NULL)
		b1->show(but1_img, win_surf);
	if(b2 != NULL)
		b2->show(but2_img, win_surf);
	if(opt != NULL)
		opt->show(opt_img, win_surf);
	if (tabScore != NULL)
        tabScore->show(bRec, win_surf);

	if (mode == 's')
	{
		solo->show_all();
		solo->add_score();
		if (!added)
		{
			solo->add_tetrimino();
			solo->show_next();
			added = true;
		}

		lv = solo->get_niv();
		frameAdd = solo->get_frame();
		timeAdd = speed[lv] + frameAdd*12.5;

		stop = solo->get_is_game_over();
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + speed[lv] && !stop){
			solo->move('d');
		  	lastTime = currentTime;
			solo->set_frame(0);
		}
		else if (currentTime > (lastTime + 3000) && stop)
		{
			mode = 'm';
		}
	}
	else if (mode == 'o')
	{
		ordi->show_all();
		if (!added)
		{
			ordi->add_tetrimino();
            ordi->add_tetrimino_o();
            // ordi->decide();
			ordi->show_next();
			added = true;
		}

		lv = ordi->get_niv();
		frameAdd = ordi->get_frame();
		timeAdd = speed[lv] + frameAdd*12.5;

		stop = ordi->get_is_game_over();
		currentTime = SDL_GetTicks();
        if (currentTime > lastTime + speed[lv] && !stop){
			ordi->move('d','j');
		  	lastTime = currentTime;
			ordi->move('d','o');
			ordi->set_frame(0);
		}

		else if (currentTime > lastTime + 3000 && stop)
		{
			mode = 'm';
		}
	}
	else if(mode=='m')
	{
		b1 = new Button(150, 500, but1_img->w, but1_img->h, "solo");
		b2 = new Button(550, 500, but2_img->w, but2_img->h, "ordi");
		opt = new Button(0, 0, opt_img->w, opt_img->h, "options");
		tabScore = new Button(340, 800, bRec->w, bRec->h,"Scoreboard");
		logo_img = SDL_LoadBMP("tetris.bmp");
		if(solo != NULL)
		{
			const int s = solo->get_score();
        	int tet = solo->get_tetris();
        	record.insert(std::pair<int, int>(s, tet));
		}
	}
	else if (mode == 'r')
    {
        SDL_Rect p;
        affSB = SDL_LoadBMP("affSB.bmp");
        p = { 0,0,affSB->w,affSB->h };
        SDL_BlitSurface(affSB, NULL, win_surf, &p);
        tabSB = SDL_LoadBMP("tabSB.bmp");
        p = { 100,150,tabSB->w,tabSB->h };
        SDL_BlitSurface(tabSB, NULL, win_surf, &p);
        scoreboard = SDL_LoadBMP("scoreboard.bmp");
        p = {7,15,scoreboard->w,scoreboard->h };
        SDL_BlitSurface(scoreboard, NULL, win_surf, &p);

        char affScore[10][6] = { "0.bmp","1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp","7.bmp","8.bmp","9.bmp" };
        int size = record.size();
        int nb, taille,position,t;
        char b[10];
        char* nb_char;
        SDL_Surface* nbIm;
        for (int i = size - 1; i >= 0; i--) {
            auto it = record.begin();

            for (int j = 0; j < i; j++)
                it++;

            nb = it->first;
            nb_char = itoa(nb, b);
            taille = std::strlen(nb_char);
            position = 0;
            for (int l= taille - 1; l >= 0; l--) {
                t = (int)nb_char[position] - (int)'0';
                nbIm = SDL_LoadBMP(affScore[t]);
                p = { 450 - (l * 50) , 140+(size-1+i)*60 ,nbIm->w,nbIm->h };
                SDL_BlitSurface(nbIm, NULL, win_surf, &p);
                position++;
            }
            nb = it->second;
            nb_char = itoa(nb, b);
            taille = std::strlen(nb_char);
            position = 0;
            for (int l = taille - 1; l >= 0; l--) {
                t = (int)nb_char[position] - (int)'0';
                nbIm = SDL_LoadBMP(affScore[t]);
                p = { 680 - (l * 50) , 140+(size-1+i)*60,nbIm->w,nbIm->h };
                SDL_BlitSurface(nbIm, NULL, win_surf, &p);
                position++;
            }

        }
    }

}


int main(int argc, char** argv)
{

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init() error" << std::endl;
    }

	init();


	Uint64 prev, now = SDL_GetPerformanceCounter(); // timers
	double delta_t;  // durée frame en ms

	bool quit = false;
	bool destroy_b1 = false;
	bool destroy_b2 = false;
	bool destroy_rec = false;

	while (!quit)
	{
		SDL_Event event;
		while (!quit && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONUP:
				if(b1 != NULL)
				{
					destroy_b1 = b1->handle_events(event);
					if (destroy_b1)
					{
						b1 = NULL;
						b2 = NULL;
						opt = NULL;
						tabScore = NULL;
						logo_img = NULL;
						solo = new Solo(win_surf);
						added=false;
						stop = false;
						mode = 's';
						// lancer le jeu solo
					}
				}
				if(b2 != NULL)
				{
					destroy_b2 = b2->handle_events(event);
					if (destroy_b2)
					{
						b2 = NULL;
						opt = NULL;
						b1 = NULL;
						tabScore = NULL;
						logo_img = NULL;
						ordi = new Ordi(win_surf);
						added=false;
						stop = false;
						mode = 'o';
						// lancer le jeu vs ordi
					}
				}
				if (tabScore != NULL)
                {
                    destroy_rec = tabScore->handle_events(event);
                    if (destroy_rec)
                    {
                        b2 = NULL;
                        opt = NULL;
                        b1 = NULL;
                        logo_img = NULL;
                        tabScore = NULL;
                        mode = 'r';
                        // Lancer tableau des scores
                    }
                }
				break;
			case SDL_KEYDOWN:
				if (mode == 's')
				{
					if(event.key.keysym.sym == SDLK_RIGHT)
						solo->move('r');
					if(event.key.keysym.sym == SDLK_LEFT)
						solo->move('l');
					if(event.key.keysym.sym == SDLK_DOWN && !stop)
						solo->move('d');
					if(event.key.keysym.sym == SDLK_UP)
						solo->tetrimino_rotate();
				}
				else if (mode == 'o')
				{
					if(event.key.keysym.sym == SDLK_RIGHT)
						ordi->move('r','j');
					if(event.key.keysym.sym == SDLK_LEFT)
						ordi->move('l','j');
					if(event.key.keysym.sym == SDLK_DOWN && !stop)
						ordi->move('d','j');
					if(event.key.keysym.sym == SDLK_UP)
                    {
                        auto t = ordi->get_last_tetrimino('j');
                        auto g = ordi->get_grille('j');
                        int array[22][12];
                        for(int a=0;a<22;a++){
                            for(int b=0;b<12;b++){
                                array[a][b]=g[a][b];
                            }
                        }
                        ordi->tetrimino_rotate(t,array);
                    }
				}
				else if (mode == 'r')
				{
					if (event.key.keysym.sym == SDLK_BACKSPACE)
						mode = 'm';
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			default: break;
			}
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		quit |= state[SDL_SCANCODE_ESCAPE];

		prev = now;
		now = SDL_GetPerformanceCounter();
		delta_t = (double)((now - prev) / (double)SDL_GetPerformanceFrequency());

		draw(delta_t);
		// affiche la surface
		SDL_UpdateWindowSurface(pWindow);

	}
	SDL_Quit();
	return 0;
}
