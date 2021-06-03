#include <vector>
#ifndef _TETRIMINO_
#define _TETRIMINO_

class Tetrimino
{

typedef struct carreau
{
    int  x;
    int  y;
}carreau;

public:
    Tetrimino(SDL_Surface* main, char lettre);
    Tetrimino(SDL_Surface* main, char lettre, int initX, int initY);

    std::vector<carreau> get_pos();
    char get_lettre();
    int get_position();
    
    void show();
    void hide();
    void move(char direction);
    void change_position();

// protected:

private:
    char lettre_;
    int position;
    SDL_Rect p;
    SDL_Surface* main_;
    SDL_Surface* colorIm;
    SDL_Surface* fondIm;
    std::vector<carreau> pos;
    std::vector<carreau*> tetrimino;
};

#endif
