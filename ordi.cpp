#include <SDL.h>
#include <iostream>
#include <time.h>
#include "ordi.h"

Ordi::Ordi(SDL_Surface* main)
{
    srand (time(NULL));

    window_o = SDL_LoadBMP("game.bmp");
    window = SDL_LoadBMP("game.bmp");
    frame_o = SDL_LoadBMP("frame.bmp");
    frame = SDL_LoadBMP("frame.bmp");
    next = SDL_LoadBMP("next.bmp");
    game_over = SDL_LoadBMP("gameover.bmp");
    victory = SDL_LoadBMP("victory.bmp");

    main_ = main;
    niv = 0;
    nbFrame = 0;
    next_lvl = 0;


    for (int y = 0; y < 22; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            grille[y][x] = 0;
            grille_o[y][x] = 0;
        }
    }

    char listL[7] = {'L','I','O','T','S','J','Z'};
    next_tetrimino_l = listL[rand()%7];
    next_tetrimino = NULL;

    is_game_over = false;
    is_game_over_o = false;
}

void Ordi::show(SDL_Surface* img, SDL_Rect* src, int x, int y, int w, int h)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = h;
    pos.h = w;

    SDL_BlitSurface( img, src, main_, &pos);
}

void Ordi::show_all()
{
    show(window_o,NULL,65,100,window->w,window->h);
    show(window,NULL,575,100,window->w,window->h);
    show(frame_o,NULL,45,80,frame->w,frame->h);
    show(frame,NULL,555,80,frame->w,frame->h);
    show(next,NULL,680,800,next->w,next->h);
}

void Ordi::show_next()
{
    if (next_tetrimino != NULL)
        next_tetrimino->hide();
    switch (next_tetrimino_l)
    {
        case 'I':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,30,75);
            break;
        case 'O':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,60,60);
            break;
        case 'T':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,45,60);
            break;
        case 'L':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,60,45);
            break;
        case 'J':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,60,45);
            break;
        case 'Z':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,45,60);
            break;
        case 'S':
            next_tetrimino = new Tetrimino(next,next_tetrimino_l,45,60);
            break;
        default:
            break;
    }
}

void Ordi::add_tetrimino_o()
{
    char listL[7] = {'L','I','O','T','S','J','Z'};
    Tetrimino* t = new Tetrimino(window_o,listL[rand()%7]);
    tetriminos_o.push_back(t);

    auto pos = t->get_pos();
    for (int j=0; j<22; j++)
    {
        for (int i=0; i<12; i++)
        {
            if(grille_o[j][i] == 1)
            {
                for (auto it=pos.begin(); it!=pos.end(); it++)
                {
                    if (it->y/30==j && it->x/30==i)
                    {
                        is_game_over_o = true;
                        break;
                    }
                }
            }
            if(is_game_over_o)
                break;
        }
    }


    if (!is_game_over_o)
    {
        next_tetrimino_l = listL[rand()%7];
    }
    else
    {
        for (int t = 0; t < tetriminos_o.size(); t++)
        {
            tetriminos_o[t]->hide();
        }
        for (int t = 0; t < tetriminos.size(); t++)
        {
            tetriminos[t]->hide();
        }
    }
}

void Ordi::add_tetrimino()
{
    char listL[7] = {'L','I','O','T','S','J','Z'};
    Tetrimino* t = new Tetrimino(window,next_tetrimino_l);
    tetriminos.push_back(t);

    auto pos = t->get_pos();
    for (int j=0; j<22; j++)
    {
        for (int i=0; i<12; i++)
        {
            if(grille[j][i] == 1)
            {
                for (auto it=pos.begin(); it!=pos.end(); it++)
                {
                    if (it->y/30==j && it->x/30==i)
                    {
                        is_game_over = true;
                        break;
                    }
                }
            }
            if(is_game_over)
                break;
        }
    }


    if (!is_game_over)
    {
        next_tetrimino_l = listL[rand()%7];
    }
    else
    {
        for (int t = 0; t < tetriminos.size(); t++)
        {
            tetriminos[t]->hide();
        }
        for (int t = 0; t < tetriminos_o.size(); t++)
        {
            tetriminos_o[t]->hide();
        }
    }
}


bool Ordi::get_is_game_over()
{
    if (is_game_over == true)
    {
        SDL_Rect p;
        p = {60,window->h/2-10,game_over->w,game_over->h};
        SDL_BlitSurface( game_over, NULL, window, &p);

        p = {75,window->h/2-10,victory->w,victory->h};
        SDL_BlitSurface( victory, NULL, window_o, &p);

        return true;
    }
    else if (is_game_over_o == true)
    {
        SDL_Rect p;
        p = {60,window->h/2-10,game_over->w,game_over->h};
        SDL_BlitSurface( game_over, NULL, window_o, &p);

        p = {100,window->h/2-10,victory->w,victory->h};
        SDL_BlitSurface( victory, NULL, window, &p);

        return true;
    }
    else
    {
        return false;
    }
}

void Ordi::print_grille()
{
    for (int y = 0; y < 22; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            std::cout << grille_o[y][x] << ' ';
        }
        std::cout << " " << '\n';
    }
    std::cout << "-----------------------------" << '\n';
}

void Ordi::move(char lettre, char jouer) {

    SDL_Surface* colorIm = SDL_LoadBMP("gris.bmp");
    SDL_Rect p;
    Tetrimino* t;
    if (jouer == 'j')
        t = tetriminos.back();
    else
        t = tetriminos_o.back();

    auto pos = t->get_pos();
    bool test=1;

    switch (lettre)
    {
    case 'r':
        for (auto it=pos.begin(); it!=pos.end();it++)
        {
            if (it->x+30 >= window->w)
            {
                test=0;
            }
            if (jouer == 'j')
            {
                if (grille[(it->y)/30][(it->x+30)/30]==1)
                    test=0;
            }
            else{
                if (grille_o[(it->y)/30][(it->x+30)/30]==1)
                    test=0;
            }
        }
        if(test){
            t->move(lettre);
        }
        break;
    case 'd':
        for (auto it=pos.begin(); it!=pos.end();it++)
        {
            if (jouer == 'j')
            {
                if ((it->y+30 >= window->h) || (grille[(it->y+30)/30][(it->x)/30]==1))
                {
                    test=0;
                    break;
                }
            }
            else{
                if ((it->y+30 >= window_o->h) || (grille_o[(it->y+30)/30][(it->x)/30]==1))
                {
                    test=0;
                    break;
                }
            }
        }
        if(test){
            t->move(lettre);
        }else{

            t->hide();
            nbFrame += 16;
            int nbLine = -1;

            for (auto it=pos.begin(); it!=pos.end();it++)
            {
                p.x = it->x;
                p.y = it->y;
                p.w = colorIm->w;
                p.h = colorIm->h;

                if (jouer == 'j')
                {
                    grille[(it->y)/30][(it->x)/30] = 1;
                    SDL_BlitSurface(colorIm, NULL, window, &p);
                }
                else
                {
                    grille_o[(it->y)/30][(it->x)/30] = 1;
                    SDL_BlitSurface(colorIm, NULL, window_o, &p);
                }
            }
            // regarder si y'a que des 1 sur une des lignes
            int test_suppr = 1;
            for (int y = 21; y>=0; y--)
            {
                test_suppr = 1;
                for (int x = 0; x<12; x++)
                {
                    if (jouer == 'j')
                    {
                        if (grille[y][x]==1)
                        {
                            test_suppr += 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (grille_o[y][x]==1)
                        {
                            test_suppr += 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                if (test_suppr>12){
                    if(jouer='j')
                    {
                        nbLine++;
                        nbFrame += 20;
                    }
                    erase_line(y,jouer);

                    y++;
                }
            }
            if (jouer=='j')
            {
                if (nbLine != -1) {
                   next_lvl += (nbLine + 1);
                    if (next_lvl >= 10) {
                        next_lvl = next_lvl % 10;
                        up_niv();
                    }
                }
                add_tetrimino();
                show_next();
            }
            else
            {
                add_tetrimino_o();
                decide();
            }
        }
        break;

    case 'l':
        for (auto it=pos.begin(); it!=pos.end();it++)
        {
            if (it->x <= 0)
            {
                test=0;
            }
            if (jouer == 'j')
            {
                if (grille[(it->y)/30][(it->x-30)/30]==1)
                    test=0;
            }
            else{
                if (grille_o[(it->y)/30][(it->x-30)/30]==1)
                    test=0;
            }
        }
        if(test){
            t->move(lettre);
        }
        break;
    default:
        break;
    }
}

void Ordi::erase_line(int y,char jouer) {

    // pour dessiner
    SDL_Surface* colorIm = SDL_LoadBMP("gris.bmp");
    SDL_Rect p;
    int random_zero = rand()%12;

    // boucle principal
    if(jouer=='j')
    {
        for (int t = 0; t < tetriminos.size(); t++)                                     // throurgh every tetrimino
        {
            tetriminos[t]->hide();
        }
        for (int j=21; j>=0; j--)
        {
            for (int i=0; i<12;i++)
            {
                if (j==y)
                {
                    grille[j][i] = 0;
                }
                else if (j<y && grille[j][i]==1)
                {
                    grille[j][i] = 0;
                    grille[j+1][i] = 1;
                }
            }
        }
        for (int t = 0; t < tetriminos.size(); t++)                                     // throurgh every tetrimino
        {
            auto pos = tetriminos[t]->get_pos();                                        // through every square of tetrimino
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (it->y/30 < y && grille[(it->y/30)+1][it->x/30]==1)
                {
                    it->y += 30;
                    p.x = it->x;
                    p.y = it->y;
                    p.w = colorIm->w;
                    p.h = colorIm->h;

                    SDL_BlitSurface(colorIm, NULL, window, &p);
                }
                else if (it->y/30 > y && grille[(it->y/30)][it->x/30]==1)
                {
                    p.x = it->x;
                    p.y = it->y;
                    p.w = colorIm->w;
                    p.h = colorIm->h;

                    SDL_BlitSurface(colorIm, NULL, window, &p);
                }
            }
        }

        for (int t = 0; t < tetriminos.size(); t++)
        {
            auto pos = tetriminos[t]->get_pos();
            for (auto it=pos.begin(); it!=pos.end(); it++)
            {
                if (it->y == y)
                {
                    pos.erase(it);
                }
            }
            if (pos.empty())
                tetriminos.erase(tetriminos.begin()+t);
        }

        for (int x = 0; x<12; x++)
        {
            if (grille_o[0][x] == 1)
            {
                is_game_over_o = true;
                break;
            }
        }
            
        if (!is_game_over_o)
        {
            for (int y = 1; y<22; y++)
            {
                for (int x = 0; x<12; x++)
                {
                    grille_o[y-1][x] = grille_o[y][x];
                }
            }

            for (int x=0; x<12; x++)
            {
                if ( x!=random_zero )
                    grille_o[21][x] = 1;
                else
                    grille_o[21][x] = 0;
            }

            for (int t = 0; t < tetriminos_o.size(); t++)                                     // throurgh every tetrimino
            {
                tetriminos_o[t]->hide();
            }

            for (int y = 21; y>=0; y--)
            {
                for (int x = 0; x<12; x++)
                {
                    if (grille_o[y][x]==1)
                    {
                        p.x = x*30;
                        p.y = y*30;
                        p.w = colorIm->w;
                        p.h = colorIm->h;
                        SDL_BlitSurface(colorIm, NULL, window_o, &p);
                    }
                }
            }

            for (int t = 0; t < tetriminos_o.size(); t++)                                     // throurgh every tetrimino
            {
                auto pos = tetriminos_o[t]->get_pos();                                        // through every square of tetrimino
                for (auto it = pos.begin(); it != pos.end(); it++)
                {
                    if (grille_o[(it->y/30)+1][it->x/30]==1)
                    {
                        it->y -= 30;
                    }
                }
            }
        }
        Tetrimino* t = new Tetrimino(window_o,'R', 0, 660);
    }
    else
    {
        for (int t = 0; t < tetriminos_o.size(); t++)                                     // throurgh every tetrimino
        {
            tetriminos_o[t]->hide();
        }
        for (int j=21; j>=0; j--)
        {
            for (int i=0; i<12;i++)
            {
                if (j==y)
                {
                    grille_o[j][i] = 0;
                }
                else if (j<y && grille_o[j][i]==1)
                {
                    grille_o[j][i] = 0;
                    grille_o[j+1][i] = 1;
                }
            }
        }

        for (int t = 0; t < tetriminos_o.size(); t++)                                     // throurgh every tetrimino
        {
            auto pos = tetriminos_o[t]->get_pos();                                        // through every square of tetrimino
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (it->y/30 < y && grille_o[(it->y/30)+1][it->x/30]==1)
                {
                    it->y += 30;
                    p.x = it->x;
                    p.y = it->y;
                    p.w = colorIm->w;
                    p.h = colorIm->h;

                    SDL_BlitSurface(colorIm, NULL, window_o, &p);
                }
                else if (it->y/30 > y && grille_o[(it->y/30)][it->x/30]==1)
                {
                    p.x = it->x;
                    p.y = it->y;
                    p.w = colorIm->w;
                    p.h = colorIm->h;

                    SDL_BlitSurface(colorIm, NULL, window_o, &p);
                }
            }
        }

        for (int t = 0; t < tetriminos_o.size(); t++)
        {
            auto pos = tetriminos_o[t]->get_pos();
            for (auto it=pos.begin(); it!=pos.end(); it++)
            {
                if (it->y == y)
                {
                    pos.erase(it);
                }
            }
            if (pos.empty())
                tetriminos_o.erase(tetriminos_o.begin()+t);
        }

        for (int x = 0; x<12; x++)
        {
            if (grille[0][x] == 1)
            {
                is_game_over = true;
                break;
            }
        }
            
        if (!is_game_over)
        {
            for (int y = 1; y<22; y++)
            {
                for (int x = 0; x<12; x++)
                {
                    grille[y-1][x] = grille[y][x];
                }
            }

            for (int x=0; x<12; x++)
            {
                if ( x!=random_zero )
                    grille[21][x] = 1;
                else
                    grille[21][x] = 0;
            }

            for (int t = 0; t < tetriminos.size(); t++)                                     // throurgh every tetrimino
            {
                tetriminos[t]->hide();
            }

            for (int y = 21; y>=0; y--)
            {
                for (int x = 0; x<12; x++)
                {
                    if (grille[y][x]==1)
                    {
                        p.x = x*30;
                        p.y = y*30;
                        p.w = colorIm->w;
                        p.h = colorIm->h;
                        SDL_BlitSurface(colorIm, NULL, window, &p);
                    }
                }
            }

            for (int t = 0; t < tetriminos.size(); t++)                                     // throurgh every tetrimino
            {
                auto pos = tetriminos[t]->get_pos();                                        // through every square of tetrimino
                for (auto it = pos.begin(); it != pos.end(); it++)
                {
                    if (grille[(it->y/30)+1][it->x/30]==1)
                    {
                        it->y -= 30;
                    }
                }
            }
        }
        Tetrimino* t = new Tetrimino(window_o,'R', 0, 660);
    }
}

void Ordi::tetrimino_rotate(Tetrimino* t, int g[][12])
{
    //auto t = tetriminos.back();
    char l = t->get_lettre();
    int position = t->get_position();
    auto pos = t->get_pos();
    bool test = true;
    int incr=0;
    switch (l)
    {
    case 'I':
        incr = 90;
        if (position == 1)
        {
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (g[(it->y - incr) / 30][(it->x + incr) / 30] == 1
                    || (it->y - incr) / 30 < 0
                    || (it->x + incr) / 30 > 11
                    )
                {
                    test = false;
                }
                incr -= 30;
            }
        }
        else
        {
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (g[(it->y + incr) / 30][(it->x - incr) / 30] == 1
                    || (it->y + incr) / 30 > 21
                    || (it->x - incr) / 30 < 0
                    )
                {
                    test = false;
                }
                incr -= 30;
            }
        }
        break;
    case 'T':
        if (position == 1)
        {
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (g[(it->y + incr) / 30][(it->x + 60 - incr) / 30] == 1
                    || (it->y + incr) / 30 > 21
                    || (it->x + 60 - incr) / 30 > 11
                    )
                {
                    test = false;
                }
                incr += 30;
                if (incr > 60)
                    break;
            }
        }
        else if (position == 2)
        {
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (g[(it->y + 60 - incr) / 30][(it->x - incr) / 30] == 1
                    || (it->y + 60 - incr) / 30 > 21
                    || (it->x - incr) / 30 < 0
                    )
                {
                    test = false;
                }
                incr += 30;
                if (incr > 60)
                    break;
            }
        }
        else if (position == 3)
        {
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (g[(it->y - incr) / 30][(it->x - 60 +incr) / 30] == 1
                    || (it->y - incr) / 30 < 0
                    || (it->x - 60 + incr) / 30 < 0
                    )
                {
                    test = false;
                }
                incr += 30;
                if (incr > 60)
                    break;
            }
        }
        else if (position == 4)
        {
            for (auto it = pos.begin(); it != pos.end(); it++)
            {
                if (g[(it->y - 60 + incr) / 30][(it->x + incr) / 30] == 1
                    || (it->y - 60 + incr) / 30 > 21
                    || (it->x + incr) / 30 < 0
                    )
                {
                    test = false;
                }
                incr += 30;
                if (incr > 60)
                    break;
            }
        }
        break;
    case 'L':
        if (position == 1)
        {
            if (
                (pos[0].y + 60)/30 <= 21
                && (pos[1].x + 30)/30 <= 11
                && (pos[1].y + 30)/30 <= 21
                && (pos[2].x + 60)/30 <= 11
                && (pos[3].x + 30)/30 <= 11
                && (pos[3].y - 30)/30 >= 0
                )
            {
                if (
                    g[(pos[0].y + 60) / 30][pos[0].x / 30] == 1
                    || g[(pos[1].y + 30) / 30][(pos[1].x + 30) / 30] == 1
                    || g[pos[2].y / 30][(pos[2].x + 60) / 30] == 1
                    || g[(pos[3].y - 30) / 30][(pos[3].x + 30) / 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        else if (position == 2) {
            if (
                (pos[0].x + 60) / 30 <= 11
                && (pos[1].x + 30) / 30 <= 11
                && (pos[1].y - 30) / 30 >= 0
                && (pos[2].y - 60) / 30 >= 0
                && (pos[3].x - 30) / 30 >= 0
                && (pos[3].y - 30) / 30 >= 0
                )
            {
                if (
                    g[pos[0].y/30][(pos[0].x + 60) / 30] == 1
                    || g[(pos[1].y - 30)/30][(pos[1].x + 30) / 30] == 1
                    || g[(pos[2].y-60)/30][pos[2].x/30] == 1
                    || g[(pos[3].y-30)/30][(pos[3].x-30) / 30] == 1
                    )
                {
                    test = false;
                }
            }

            else {
                test = false;
            }

        }
        else if (position == 3) {
            if (
                (pos[0].y - 30) / 30 >=0
                && (pos[1].x - 30) / 30 >= 0
                && (pos[2].x - 60) / 30 >= 0
                && (pos[2].y + 30) / 30 <= 21
                && (pos[3].x - 30) / 30 >= 0
                && (pos[3].y + 60) / 30 <= 21
                )
            {
                if (
                    g[(pos[0].y - 30)/30][pos[0].x/ 30] == 1
                    || g[pos[1].y/ 30][(pos[1].x - 30) / 30] == 1
                    || g[(pos[2].y + 30) / 30][(pos[2].x - 60) / 30] == 1
                    || g[(pos[3].y + 60) / 30][(pos[3].x - 30) / 30] == 1
                    )
                {
                    test = false;
                }
            }

            else {
                test = false;
            }
        }
        else if (position == 4) {
            if (
                (pos[0].x - 60)/30 >= 0
                && (pos[0].y - 30)/30 >= 0
                && (pos[1].x - 30)/30 >= 0
                && (pos[2].y + 30)/30 <= 21
                && (pos[3].x + 30)/30 <= 11
                )
            {
                if (
                    g[(pos[0].y-30)/30][(pos[0].x-60)/30] == 1
                    || g[pos[1].y/ 30][(pos[1].x-30)/30] == 1
                    || g[(pos[2].y +30) / 30][pos[2].x / 30] == 1
                    || g[pos[3].y/30][(pos[3].x + 30) / 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        break;
    case 'J':
        if (position == 1)
        {
            if (
                (pos[0].x - 30) / 30 >= 0
                && (pos[0].y + 30) / 30 <= 21
                && (pos[2].x + 30) / 30 <= 11
                && (pos[2].y - 30) / 30 >= 0
                && (pos[3].x + 60) / 30 <= 11
                )
            {
                if (
                    g[(pos[0].y + 30) / 30][(pos[0].x - 30) / 30] == 1
                    || g[pos[1].y/ 30][pos[1].x/ 30] == 1
                    || g[(pos[2].y - 30) / 30][(pos[2].x + 30) / 30] == 1
                    || g[pos[3].y/ 30][(pos[3].x + 60) / 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        else if (position == 2) {
            if (
                (pos[0].x + 30) / 30 <= 11
                && (pos[0].y + 30) / 30 <= 21
                && (pos[2].x - 30) / 30 >= 0
                && (pos[2].y - 30) / 30 >= 0
                && (pos[3].y - 60) / 30 >= 0
                )
            {
                if (
                    g[(pos[0].y + 30) / 30][(pos[0].x + 30) / 30] == 1
                    || g[pos[1].y/ 30][pos[1].x/ 30] == 1
                    || g[(pos[2].y - 30) / 30][(pos[2].x - 30) / 30] == 1
                    || g[(pos[3].y - 60) / 30][pos[3].x/ 30] == 1
                    )
                {
                    test = false;
                }
            }

            else {
                test = false;
            }

        }
        else if (position == 3) {
            if (
                (pos[0].x + 30) / 30 <= 11
                && (pos[1].y + 30) / 30 <= 21
                && (pos[2].x - 30) / 30 >= 0
                && (pos[2].y + 60) / 30 <= 21
                && (pos[3].x - 60) / 30 >= 0
                && (pos[3].y + 30) / 30 <= 21
                )
            {
                if (
                    g[pos[0].y/ 30][(pos[0].x + 30) / 30] == 1
                    || g[(pos[1].y+30)/30][pos[1].x/30] == 1
                    || g[(pos[2].y + 60) / 30][(pos[2].x - 30) / 30] == 1
                    || g[(pos[3].y + 30) / 30][(pos[3].x - 60) / 30] == 1
                    )
                {
                    test = false;
                }
            }

            else {
                test = false;
            }
        }
        else if (position == 4) {
            if (
                (pos[0].x - 30) / 30 >= 0
                && (pos[0].y - 60) / 30 >= 0
                && (pos[1].y - 30) / 30 >= 0
                && (pos[2].x + 30) / 30 <= 11
                && (pos[3].y + 30) / 30 <= 21
                )
            {
                if (
                    g[(pos[0].y - 60) / 30][(pos[0].x - 30) / 30] == 1
                    || g[(pos[1].y - 30) / 30][pos[1].x/ 30] == 1
                    || g[pos[2].y/30][(pos[2].x + 30) / 30] == 1
                    || g[(pos[3].y + 30) / 30][pos[3].x/ 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        break;
    case 'Z':
        if (position == 1)
        {
            if (
                (pos[0].x + 30) / 30 <= 11
                && (pos[1].x + 30) / 30 <= 11
                && (pos[1].y - 30) / 30 >= 0
                && (pos[3].y - 30) / 30 >= 0
                )
            {
                if (
                    g[pos[0].y/30][(pos[0].x + 30) / 30] == 1
                    || g[(pos[1].y - 30) / 30][(pos[1].x + 30) / 30] == 1
                    || g[pos[2].y/30][pos[2].x/30] == 1
                    || g[(pos[3].y-30)/30][pos[3].x/30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        else if (position == 2) {
            if (
                (pos[0].x - 30) / 30 >= 0
                && (pos[1].x - 30) / 30 >= 0
                && (pos[1].y + 30) / 30 <= 21
                && (pos[3].y + 30) / 30 <= 21
                )
            {
                if (
                    g[pos[0].y / 30][(pos[0].x - 30) / 30] == 1
                    || g[(pos[1].y + 30) / 30][(pos[1].x - 30) / 30] == 1
                    || g[pos[2].y / 30][pos[2].x / 30] == 1
                    || g[(pos[3].y + 30) / 30][pos[3].x / 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        break;
    case 'S':
        if (position == 1)
        {
            if (
                (pos[0].x + 30) / 30 <= 11
                && (pos[0].y - 30) / 30 >= 0
                && (pos[1].x + 30) / 30 <= 11
                && (pos[2].y - 30) / 30 >= 0
                )
            {
                if (
                    g[(pos[0].y - 30) / 30][(pos[0].x + 30) / 30] == 1
                    || g[pos[1].y/ 30][(pos[1].x + 30) / 30] == 1
                    || g[(pos[2].y - 30) / 30][pos[2].x / 30] == 1
                    || g[pos[3].y / 30][pos[3].x / 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        else if (position == 2) {
            if (
                (pos[0].x - 30) / 30 >= 0
                && (pos[0].y + 30) / 30 <= 21
                && (pos[1].x - 30) / 30 >= 0
                && (pos[2].y + 30) / 30 <= 21
                )
            {
                if (
                    g[(pos[0].y + 30) / 30][(pos[0].x - 30) / 30] == 1
                    || g[pos[1].y/ 30][(pos[1].x - 30) / 30] == 1
                    || g[(pos[2].y + 30) / 30][pos[2].x / 30] == 1
                    || g[pos[3].y / 30][pos[3].x / 30] == 1
                    )
                {
                    test = false;
                }
            }
            else {
                test = false;
            }
        }
        break;
    }
    if (test) {
        t->change_position();
    }
}

void Ordi::decide()
{

    auto t = tetriminos_o.back();
    auto pos = t->get_pos();
    auto check_square = pos.begin();
    int dmin,posMax=0;
    int max_contact=0;
    int contact=0;
    bool testCol = true, test = true;
    bool x_check[12];
    for(int i = 0; i<12;i++){
        x_check[i]=0;
    }
    int diff_x;
    auto it_x = pos.begin();
    bool ll0 = false;
    for (int tx=0;tx<12;tx++)
    {   
        testCol = true;
        test = true;
        contact=0;
        testCol = true;
        if(grille_o[21][tx]==0)
        {
            for (int i = 0; i < 21; i++) 
            {
                if (grille_o[i][tx] == 1)
                {
                    testCol = false;
                    break;
                }
            }
            if(testCol)
            {   
                dmin = (21-check_square->y/30);
                for(auto it = pos.begin();it<pos.end();it++)
                {
                    diff_x = it->x/30 - it_x->x/30;

                    for (int i = it->y / 30; i < (it->y/30+dmin); i++)
                    {
                        if(grille_o[i][tx + diff_x] == 1)
                        {
                            testCol = false;
                            break;
                        }
                    }
                    
                    if (tx + diff_x < 0 || tx + diff_x > 11|| testCol==false)
                    {
                        test = false;
                    }
                }
                    
                if (test) 
                {
                    for (auto it = pos.begin(); it < pos.end(); it++)
                    {
                        if (grille_o[it->y/30+dmin][tx + 1 + diff_x] == 1)
                            contact++;
                        if (grille_o[it->y/30+dmin][tx - 1 + diff_x] == 1)
                            contact++;
                        if (grille_o[it->y/30+dmin+1][tx + diff_x] == 1)
                            contact++;
                        if ((it->y/30+dmin) == 21)
                            contact++;
                    }
                    if (contact > max_contact)
                    {
                        posMax = tx;
                        max_contact = contact;
                    }

                    contact = 0;
                }
            }
        }
     }
    for (int y=0;y<22;y++)
    {
        for(int x=0; x<12; x++)
        {
            contact = 0;
            test = true;
            it_x = pos.begin();
            if(x_check[x]==0)
            {
                if(grille_o[y][x]==1)
                {
                    x_check[x]=1;
                    dmin = (y-check_square->y/30-1);
                    for(auto it = pos.begin();it<pos.end();it++)
                    {
                        diff_x = it->x/30 - it_x->x/30;
                        if ((x + (diff_x)) < 0 || x + diff_x > 11 || grille_o[it->y / 30 + dmin][x + diff_x] == 1)
                        {
                            test = false;
                            break;
                        }
                    
                    }
                    if(test)
                    {
                        for(auto it = pos.begin();it<pos.end();it++)
                        {
                            if(grille_o[it->y/30+dmin][x+1 + diff_x]==1)
                                contact++;
                            if(grille_o[it->y/30+dmin][x-1 + diff_x]==1)
                                contact++;
                            if(grille_o[it->y/30+dmin+1][x + diff_x]==1)
                                contact++;
                        }
                        if(contact>max_contact)
                        {
                            posMax = x;
                            max_contact=contact;
                        }
                        contact = 0;
                    }
                }
            }
        }
    }
    check_square = pos.begin();
    if(check_square->x/30 > posMax)
    {
        while (check_square->x/30 > posMax) {
            t->move('l');
            pos = t->get_pos();
            check_square = pos.begin();
        }
    }
    else if(check_square->x/30 < posMax)
    {
        while (check_square->x / 30 < posMax)
        {
            t->move('r');
            pos = t->get_pos();
            check_square = pos.begin();
        }
    }
}

Tetrimino* Ordi::get_last_tetrimino (char jouer)
{
    Tetrimino *t;
    if (jouer == 'j')
        t = tetriminos.back();
    else
        t = tetriminos_o.back();

    return t;
}


int Ordi::get_niv()
{
    return niv;
}

int Ordi::get_frame()
{
    return nbFrame;
}

void Ordi::set_frame(int x)
{
    nbFrame = x;
}

void Ordi::up_niv()
{
    niv++;
}

int** Ordi::get_grille(char jouer)
{

    int** g = 0;
    g = new int*[22];
    if(jouer=='j')
    {
        for (int h = 0; h < 22; h++)
        {
            g[h] = new int[12];
            for (int w = 0; w < 12; w++)
            {
              g[h][w] = grille[h][w];
            }
        }
    }
    else
    {
        for (int h = 0; h < 22; h++)
        {
            g[h] = new int[12];
            for (int w = 0; w < 12; w++)
            {
              g[h][w] = grille_o[h][w];
            }
        }
    }

    return g;
}

Ordi::~Ordi()
{
    auto glambda = [](auto i) { delete[] i; };
    for (int i=0; i<22; i++)
        glambda(grille[i]);
    auto glambda_o= [](auto i) { delete[] i; };
    for (int i=0; i<22; i++)
        glambda_o(grille_o[i]);
    delete frame;
    delete window;
    delete next;
    delete game_over;
    delete main_;
    delete next_tetrimino;
    // delete[] scoreLine;
}
