#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include "tetrimino.h"
#ifndef _SOLO_
#define _SOLO_

class Solo
{

typedef std::vector<Tetrimino*> Tetriminos;

public:
    Solo(SDL_Surface* main);
    void show_all();
    void add_tetrimino();
    void move(char lettre);
    void add_score();
    void erase_line(int y);
    void tetrimino_rotate();
    void show_next();
    int get_niv();
    int get_frame();
    int get_score();
    int get_tetris();
    bool get_is_game_over();
    void set_frame(int x);
    void up_niv();
    char* itoa(int i, char b[]);
    ~Solo();
// protected

private:
    void show(SDL_Surface* img, SDL_Rect* src, int x, int y, int w, int h);
    void print_grille();
    SDL_Surface* frame;
    SDL_Surface* window;
    SDL_Surface* next;
    SDL_Surface* game_over;
    SDL_Surface* scoreIM;
    SDL_Surface* niveauIM;
    SDL_Surface* main_;
    Tetrimino* next_tetrimino;
    int grille[22][12];
    int scoreLine[4];
    char next_tetrimino_l;
    Tetriminos tetriminos;
    int score;
    int tetris;
    int niv;
    int nbFrame;
    int next_lvl;
    bool is_game_over;
};

#endif
