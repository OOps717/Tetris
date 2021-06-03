#include <SDL.h>
#include <iostream>
#include <time.h>
#include "solo.h"

Solo::Solo(SDL_Surface* main)
{
    srand (time(NULL));

    window = SDL_LoadBMP("game.bmp");
    frame = SDL_LoadBMP("frame.bmp");
    next = SDL_LoadBMP("next.bmp");
    game_over = SDL_LoadBMP("gameover.bmp");
    scoreIM = SDL_LoadBMP("score.bmp");
    niveauIM = SDL_LoadBMP("niveau.bmp");
    main_ = main;
    score = 0;
    niv = 0;
    nbFrame = 0;
    scoreLine[0] = 40;
    scoreLine[1] = 100;
    scoreLine[2] = 300;
    scoreLine[3] = 1200;
    next_lvl = 0;
    tetris = 0;

    for (int y = 0; y < 22; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            grille[y][x] = 0;
        }
    }

    char listL[7] = {'L','I','O','T','S','J','Z'};
    next_tetrimino_l = listL[rand()%7];
    next_tetrimino = NULL;

    is_game_over = false;
}

void Solo::show(SDL_Surface* img, SDL_Rect* src, int x, int y, int w, int h)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = h;
    pos.h = w;

    SDL_BlitSurface( img, src, main_, &pos);
}


void Solo::show_all()
{
    show(window,NULL,575,200,window->w,window->h);
    show(frame,NULL,555,180,frame->w,frame->h);
    show(next,NULL,200,180,next->w,next->h);
    show(scoreIM, NULL, 40, 725, scoreIM->w, scoreIM->h);
    show(niveauIM, NULL, 165, 450, niveauIM->w, niveauIM->h);
}

void Solo::show_next()
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

void Solo::add_tetrimino()
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
        SDL_Rect p;
        p = {60,window->h/2-10,game_over->w,game_over->h};
        SDL_BlitSurface( game_over, NULL, window, &p);
    }
}

bool Solo::get_is_game_over()
{
    return is_game_over;
}



void Solo::print_grille()
{
    for (int y = 0; y < 22; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            std::cout << grille[y][x] << ' ';
        }
        std::cout << " " << '\n';
    }
    std::cout << "-----------------------------" << '\n';
}

void Solo::move(char lettre) {

    SDL_Surface* colorIm = SDL_LoadBMP("gris.bmp");
    SDL_Rect p;
    auto t = tetriminos.back();
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
            if (grille[(it->y)/30][(it->x+30)/30]==1)
            {
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
            if ((it->y+30 >= window->h) || (grille[(it->y+30)/30][(it->x)/30]==1))
            {
                test=0;
                break;
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
                grille[(it->y)/30][(it->x)/30] = 1;
                p.x = it->x;
                p.y = it->y;
                p.w = colorIm->w;
                p.h = colorIm->h;
                SDL_BlitSurface(colorIm, NULL, window, &p);
            }
            // regarder si y'a que des 1 sur une des lignes
            int test_suppr = 1;
            for (int y = 21; y>=0; y--)
            {
                test_suppr = 1;
                for (int x = 0; x<12; x++)
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
                if (test_suppr>12){
                    erase_line(y);
                    y++;
                    nbLine++;
                    nbFrame += 20;
                }
            }
            if (nbLine != -1) {
                score += scoreLine[nbLine] * (niv + 1);
                add_score();
               next_lvl += (nbLine + 1);
                if (next_lvl >= 10) {
                    next_lvl = next_lvl % 10;
                    up_niv();
                }
                if (nbLine == 3)
                    tetris++;

            }
            add_tetrimino();
            show_next();
        }
        break;

    case 'l':
        for (auto it=pos.begin(); it!=pos.end();it++)
        {
            if (it->x <= 0)
            {
                test=0;
            }
            if (grille[(it->y)/30][(it->x-30)/30]==1)
            {
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

char* Solo::itoa(int i, char b[]) {
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

void Solo::add_score()
{
    SDL_Rect p;
    SDL_Surface* nbIm;
    char affScore[10][6] = {"0.bmp","1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp","7.bmp","8.bmp","9.bmp" };
    int nb = score;
    char* nb_char;
    char b[10];
    nb_char = itoa(nb, b);
    int taille = std::strlen(nb_char);
    int t;
    int j = 0;
    for (int i = taille-1 ; i >= 0 ;i--) {
        t = (int)nb_char[j] - (int)'0';
        nbIm = SDL_LoadBMP(affScore[t]);
        p = { 450 - (i*50) , 775 ,nbIm->w,nbIm->h};
        SDL_BlitSurface(nbIm, NULL, main_, &p);
        j++;
    }
    nb = niv;
    nb_char = itoa(nb, b);
    taille = std::strlen(nb_char);
    j = 0;
    for (int i = taille - 1; i >= 0; i--) {
        t = (int)nb_char[j] - (int)'0';
        nbIm = SDL_LoadBMP(affScore[t]);
        p = { 280 - (i * 50) , 525 ,nbIm->w,nbIm->h };
        SDL_BlitSurface(nbIm, NULL, main_, &p);
        j++;
    }

}

void Solo::erase_line(int y) {

    // pour dessiner
    SDL_Surface* colorIm = SDL_LoadBMP("gris.bmp");
    SDL_Rect p;


    // boucle principal
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

}


void Solo::tetrimino_rotate() {
    auto t = tetriminos.back();
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
                if (grille[(it->y - incr) / 30][(it->x + incr) / 30] == 1
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
                if (grille[(it->y + incr) / 30][(it->x - incr) / 30] == 1
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
                if (grille[(it->y + incr) / 30][(it->x + 60 - incr) / 30] == 1
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
                if (grille[(it->y + 60 - incr) / 30][(it->x - incr) / 30] == 1
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
                if (grille[(it->y - incr) / 30][(it->x - 60 +incr) / 30] == 1
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
                if (grille[(it->y - 60 + incr) / 30][(it->x + incr) / 30] == 1
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
                    grille[(pos[0].y + 60) / 30][pos[0].x / 30] == 1
                    || grille[(pos[1].y + 30) / 30][(pos[1].x + 30) / 30] == 1
                    || grille[pos[2].y / 30][(pos[2].x + 60) / 30] == 1
                    || grille[(pos[3].y - 30) / 30][(pos[3].x + 30) / 30] == 1
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
                    grille[pos[0].y/30][(pos[0].x + 60) / 30] == 1
                    || grille[(pos[1].y - 30)/30][(pos[1].x + 30) / 30] == 1
                    || grille[(pos[2].y-60)/30][pos[2].x/30] == 1
                    || grille[(pos[3].y-30)/30][(pos[3].x-30) / 30] == 1
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
                    grille[(pos[0].y - 30)/30][pos[0].x/ 30] == 1
                    || grille[pos[1].y/ 30][(pos[1].x - 30) / 30] == 1
                    || grille[(pos[2].y + 30) / 30][(pos[2].x - 60) / 30] == 1
                    || grille[(pos[3].y + 60) / 30][(pos[3].x - 30) / 30] == 1
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
                    grille[(pos[0].y-30)/30][(pos[0].x-60)/30] == 1
                    || grille[pos[1].y/ 30][(pos[1].x-30)/30] == 1
                    || grille[(pos[2].y +30) / 30][pos[2].x / 30] == 1
                    || grille[pos[3].y/30][(pos[3].x + 30) / 30] == 1
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
                    grille[(pos[0].y + 30) / 30][(pos[0].x - 30) / 30] == 1
                    || grille[pos[1].y/ 30][pos[1].x/ 30] == 1
                    || grille[(pos[2].y - 30) / 30][(pos[2].x + 30) / 30] == 1
                    || grille[pos[3].y/ 30][(pos[3].x + 60) / 30] == 1
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
                    grille[(pos[0].y + 30) / 30][(pos[0].x + 30) / 30] == 1
                    || grille[pos[1].y/ 30][pos[1].x/ 30] == 1
                    || grille[(pos[2].y - 30) / 30][(pos[2].x - 30) / 30] == 1
                    || grille[(pos[3].y - 60) / 30][pos[3].x/ 30] == 1
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
                    grille[pos[0].y/ 30][(pos[0].x + 30) / 30] == 1
                    || grille[(pos[1].y+30)/30][pos[1].x/30] == 1
                    || grille[(pos[2].y + 60) / 30][(pos[2].x - 30) / 30] == 1
                    || grille[(pos[3].y + 30) / 30][(pos[3].x - 60) / 30] == 1
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
                    grille[(pos[0].y - 60) / 30][(pos[0].x - 30) / 30] == 1
                    || grille[(pos[1].y - 30) / 30][pos[1].x/ 30] == 1
                    || grille[pos[2].y/30][(pos[2].x + 30) / 30] == 1
                    || grille[(pos[3].y + 30) / 30][pos[3].x/ 30] == 1
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
                    grille[pos[0].y/30][(pos[0].x + 30) / 30] == 1
                    || grille[(pos[1].y - 30) / 30][(pos[1].x + 30) / 30] == 1
                    || grille[pos[2].y/30][pos[2].x/30] == 1
                    || grille[(pos[3].y-30)/30][pos[3].x/30] == 1
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
                    grille[pos[0].y / 30][(pos[0].x - 30) / 30] == 1
                    || grille[(pos[1].y + 30) / 30][(pos[1].x - 30) / 30] == 1
                    || grille[pos[2].y / 30][pos[2].x / 30] == 1
                    || grille[(pos[3].y + 30) / 30][pos[3].x / 30] == 1
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
                    grille[(pos[0].y - 30) / 30][(pos[0].x + 30) / 30] == 1
                    || grille[pos[1].y/ 30][(pos[1].x + 30) / 30] == 1
                    || grille[(pos[2].y - 30) / 30][pos[2].x / 30] == 1
                    || grille[pos[3].y / 30][pos[3].x / 30] == 1
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
                    grille[(pos[0].y + 30) / 30][(pos[0].x - 30) / 30] == 1
                    || grille[pos[1].y/ 30][(pos[1].x - 30) / 30] == 1
                    || grille[(pos[2].y + 30) / 30][pos[2].x / 30] == 1
                    || grille[pos[3].y / 30][pos[3].x / 30] == 1
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

int Solo::get_niv() {
    return niv;
}

int Solo::get_frame() {
    return nbFrame;
}

void Solo::set_frame(int x) {
    nbFrame = x;
}

void Solo::up_niv() {
    niv++;
}

int Solo::get_score() {
    return score;
}

int Solo::get_tetris() {
    return tetris;
}

Solo::~Solo()
{
    auto glambda = [](auto i) { delete[] i; };
    for (int i=0; i<22; i++)
        glambda(grille[i]);
    delete frame;
    delete window;
    delete next;
    delete game_over;
    delete scoreIM;
    delete niveauIM;
    delete main_;
    delete next_tetrimino;
    // delete[] scoreLine;
}
