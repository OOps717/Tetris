#include <SDL.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include "tetrimino.h"

Tetrimino::Tetrimino(SDL_Surface* main, char lettre){
    lettre_ = lettre;
    main_ = main;
    position = 1;
    srand (time(NULL));
    int random_position = 30*(rand() % 9);
    switch (lettre)
    {
        case 'I':
            colorIm= SDL_LoadBMP("cyan.bmp");
            pos.push_back({random_position,0});
            pos.push_back({random_position+30,0});
            pos.push_back({random_position+60,0});
            pos.push_back({random_position+90,0});
            break;
        case 'O':
            colorIm= SDL_LoadBMP("jaune.bmp");
            pos.push_back({random_position,30});
            pos.push_back({random_position+30,30});
            pos.push_back({random_position,0});
            pos.push_back({random_position+30,0});
            break;
        case 'T' :
            colorIm= SDL_LoadBMP("magenta.bmp");
            pos.push_back({random_position,0});
            pos.push_back({random_position+30,0});
            pos.push_back({random_position+60,0});
            pos.push_back({random_position+30,30});
            break;
        case 'L' :
            colorIm= SDL_LoadBMP("orange.bmp");
            pos.push_back({random_position,0});
            pos.push_back({random_position,30});
            pos.push_back({random_position,60});
            pos.push_back({random_position+30,60});
            break;
        case 'J' :
            colorIm= SDL_LoadBMP("bleu.bmp");
            pos.push_back({random_position+30,0});
            pos.push_back({random_position+30,30});
            pos.push_back({random_position+30,60});
            pos.push_back({random_position,60});
            break;
        case 'Z' :
            colorIm= SDL_LoadBMP("rouge.bmp");
            pos.push_back({random_position,0});
            pos.push_back({random_position+30,0});
            pos.push_back({random_position+30,30});
            pos.push_back({random_position+60,30});
            break;
        case 'S' :
            colorIm= SDL_LoadBMP("vert.bmp");
            pos.push_back({random_position,30});
            pos.push_back({random_position+30,30});
            pos.push_back({random_position+30,0});
            pos.push_back({random_position+60,0});
            break;
        default:
            break;
    }
    fondIm = SDL_LoadBMP("fond.bmp");
    show();
}

Tetrimino::Tetrimino(SDL_Surface* main, char lettre, int initX, int initY){
    lettre_ = lettre;
    main_ = main;
    switch (lettre)
    {
        case 'I':
            colorIm= SDL_LoadBMP("cyan.bmp");
            pos.push_back({initX,initY+0});
            pos.push_back({initX+30,initY+0});
            pos.push_back({initX+60,initY+0});
            pos.push_back({initX+90,initY+0});
            break;
        case 'O':
            colorIm= SDL_LoadBMP("jaune.bmp");
            pos.push_back({initX,initY+30});
            pos.push_back({initX+30,initY+30});
            pos.push_back({initX,initY+0});
            pos.push_back({initX+30,initY+0});
            break;
        case 'T' :
            colorIm= SDL_LoadBMP("magenta.bmp");
            pos.push_back({initX,initY+0});
            pos.push_back({initX+30,initY+0});
            pos.push_back({initX+60,initY+0});
            pos.push_back({initX+30,initY+30});
            break;
        case 'L' :
            colorIm= SDL_LoadBMP("orange.bmp");
            pos.push_back({initX,initY+0});
            pos.push_back({initX,initY+30});
            pos.push_back({initX,initY+60});
            pos.push_back({initX+30,initY+60});
            break;
        case 'J' :
            colorIm= SDL_LoadBMP("bleu.bmp");
            pos.push_back({initX+30,initY+0});
            pos.push_back({initX+30,initY+30});
            pos.push_back({initX+30,initY+60});
            pos.push_back({initX,initY+60});
            break;
        case 'Z' :
            colorIm= SDL_LoadBMP("rouge.bmp");
            pos.push_back({initX,initY+0});
            pos.push_back({initX+30,initY+0});
            pos.push_back({initX+30,initY+30});
            pos.push_back({initX+60,initY+30});
            break;
        case 'S' :
            colorIm= SDL_LoadBMP("vert.bmp");
            pos.push_back({initX,initY+30});
            pos.push_back({initX+30,initY+30});
            pos.push_back({initX+30,initY+0});
            pos.push_back({initX+60,initY+0});
            break;
        case 'R':
            // random gray line
            colorIm= SDL_LoadBMP("gris.bmp");
            for (int x=0; x<12; x++)
            {
                pos.push_back({30*x,initY});
            }
        default:
            break;
    }
    fondIm = SDL_LoadBMP("fond.bmp");
    show();
}

void Tetrimino::show()
{
    for (auto it = pos.begin(); it!=pos.end();it++)
    {
        p.x = it->x;
        p.y = it->y;
        p.w = colorIm->w;
        p.h = colorIm->h;

        SDL_BlitSurface(colorIm, NULL, main_, &p);
    }
}

void Tetrimino::hide()
{
    for (auto it = pos.begin(); it!=pos.end();it++)
    {
        p.x = it->x;
        p.y = it->y;
        p.w = fondIm->w;
        p.h = fondIm->h;

        SDL_BlitSurface(fondIm, NULL, main_, &p);
    }
}

std::vector<Tetrimino::carreau> Tetrimino::get_pos()
{
    return pos;
}

char Tetrimino::get_lettre()
{
    return lettre_;
}

int Tetrimino::get_position()
{
    return position;
}

void Tetrimino::move(char direction)
{
    switch (direction)
    {
    case 'r':
        for (auto it=pos.begin(); it!=pos.end();it++)
        {
            hide();
            it->x = it->x+30;
        }
        break;
    case 'd':
        for (std::vector<carreau>::iterator it=pos.begin(); it!=pos.end();it++)
        {
            hide();
            it->y = it->y+30;
        }
        break;
    case 'l':
        for (auto it=pos.begin(); it!=pos.end();it++)
        {
                    hide();
                    it->x = it->x-30;
        }
        break;
    default:
        break;
    }
    show();
}

void Tetrimino::change_position()
{
    int incr = 0;
    switch (lettre_)
    {
        case 'I':
            incr = 90;
            if (position==1)
            {   
                hide();
                for (auto it = pos.begin(); it != pos.end(); it++)
                {
                    it->x += incr;
                    it->y -= incr;
                    incr -= 30;
                }
                position = 2;
                
            }else
            {
                hide();
                for (auto it=pos.begin(); it!=pos.end();it++)
                {
                    it->x -= incr;
                    it->y += incr;
                    incr -= 30;
                }
                position = 1;
            }
            break;
        case 'T':
            if (position==1)
            {
                hide();
                for (auto it=pos.begin(); it!=pos.end();it++)
                {
                    it->y += incr;
                    it->x += 60-incr;
                    incr += 30;
                    if(incr>60)
                        break;
                }
                position = 2;
            }
            else if (position==2)
            {
                hide();
                for (auto it=pos.begin(); it!=pos.end();it++)
                {
                    it->y += 60-incr;
                    it->x -= incr;
                    incr += 30;
                    if(incr>60)
                        break;
                }
                position = 3;
            }
            else if (position==3)
            {
                hide();
                for (auto it=pos.begin(); it!=pos.end();it++)
                {
                    it->y -= incr;
                    it->x -= 60-incr;
                    incr += 30;
                    if(incr>60)
                        break;
                }
                position = 4;
            }
            else if(position==4)
            {
                hide();
                for (auto it=pos.begin(); it!=pos.end();it++)
                {
                    it->x += incr;
                    it->y -= 60-incr;
                    incr += 30;
                    if(incr>60)
                        break;
                }
                position = 1;
            }
            break;
        case 'L':
            if (position==1)
            {
                hide();
                pos[0].y += 60;
                pos[1].x += 30;
                pos[1].y += 30;
                pos[2].x += 60;
                pos[3].x += 30;
                pos[3].y -= 30;
                position = 2;
            }else if(position == 2){
                hide();
                pos[0].x += 60;
                pos[1].x += 30;
                pos[1].y -= 30;
                pos[2].y -= 60;
                pos[3].x -= 30;
                pos[3].y -= 30;
                position = 3;
            }else if(position == 3){
                hide();
                pos[0].y -= 30;
                pos[1].x -= 30;
                pos[2].x -= 60;
                pos[2].y += 30;
                pos[3].x -= 30;
                pos[3].y += 60;
                position = 4;
            }else if(position == 4){
                hide();
                pos[0].x -= 60;
                pos[0].y -= 30;
                pos[1].x -= 30;
                pos[2].y += 30;
                pos[3].x += 30;
                position = 1;
            }
            break;
        case 'J':
            if (position==1)
            {
                hide();
                pos[0].x -= 30;
                pos[0].y += 30;
                pos[2].x += 30;
                pos[2].y -= 30;
                pos[3].x += 60;
                position = 2;
            }else if(position == 2){
                hide();
                pos[0].x += 30;
                pos[0].y += 30;
                pos[2].x -= 30;
                pos[2].y -= 30;
                pos[3].y -= 60;
                position = 3;
            }else if(position == 3){
                hide();
                pos[0].x += 30;
                pos[1].y += 30;
                pos[2].x -= 30;
                pos[2].y += 60;
                pos[3].x -= 60;
                pos[3].y += 30;
                position = 4;
            }else if(position == 4){
                hide();
                pos[0].x -= 30;
                pos[0].y -= 60;
                pos[1].y -= 30;
                pos[2].x += 30;
                pos[3].y += 30;
                position = 1;
            }
            break;
        case 'Z':
            if (position==1)
            {
                hide();
                pos[0].x += 30;
                pos[1].x += 30;
                pos[1].y -= 30;
                pos[3].y -= 30;
                position = 2;
            }
            else
            {
                hide();
                pos[0].x -= 30;
                pos[1].x -= 30;
                pos[1].y += 30;
                pos[3].y += 30;
                position = 1;
            }
            break;
        case 'S':
            if (position==1)
            {
                hide();
                pos[0].x += 30;
                pos[0].y -= 30;
                pos[1].x += 30;
                pos[2].y -= 30;
                position = 2;
            }
            else
            {
                hide();
                pos[0].x -= 30;
                pos[0].y += 30;
                pos[1].x -= 30;
                pos[2].y += 30;
                position = 1;
            }
            break;
        default:
            break;
    }
    show();
}
