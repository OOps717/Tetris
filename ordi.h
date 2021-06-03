#include <SDL.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include "tetrimino.h"

#ifndef _ORDI_
#define _ORDI_

class Ordi
{

typedef std::vector<Tetrimino*> Tetriminos;

public:
    Ordi(SDL_Surface* main);
    void show_all();
    void show_next();
    void move(char lettre, char jouer);
    void add_score();
    void erase_line(int y, char jouer);
    void tetrimino_rotate(Tetrimino* t, int g[][12]);
    int get_niv();
    int get_frame();
    bool get_is_game_over();
    void set_frame(int x);
    void up_niv();
    Tetrimino* get_last_tetrimino (char jouer);
    int** get_grille(char jouer);

    void add_tetrimino();

    void add_tetrimino_o();
    void decide();
    ~Ordi();
// protected

private:
    void show(SDL_Surface* img, SDL_Rect* src, int x, int y, int w, int h);
    void print_grille();
    SDL_Surface* main_;
    SDL_Surface* next;
    Tetrimino* next_tetrimino;
    char next_tetrimino_l;
    int niv;
    int nbFrame;
    int next_lvl;
    SDL_Surface* game_over;
    SDL_Surface* victory;

    SDL_Surface* frame;
    SDL_Surface* window;
    int grille[22][12];
    Tetriminos tetriminos;
    bool is_game_over;

    SDL_Surface* window_o;
    SDL_Surface* frame_o;
    int grille_o[22][12];
    Tetriminos tetriminos_o;
    bool is_game_over_o;
};

#endif
