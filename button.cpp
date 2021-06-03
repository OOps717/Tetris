#include <SDL.h>
#include <iostream>
#include <string>
#include "button.h"

Button::Button( int x, int y, int w, int h, std::string name)
{
  // Position du bouton (x,y) position du coin haut gauche (w,h) dimension du bouton
  button.x = x;
  button.y = y;
  button.w = w;
  button.h = h;
}

bool Button::handle_events(SDL_Event event)
{
    //The mouse offsets
    int x = 0, y = 0;

    if( event.button.button == SDL_BUTTON_LEFT )
    {
        //Get the mouse offsets
        x = event.button.x;
        y = event.button.y;
        //If the mouse is over the button
        if( ( x > button.x ) && ( x < button.x + button.w ) && ( y > button.y ) && ( y < button.y + button.h ) )
        {
            return true;
        }
    }
    return false;
}

int Button::get_x()
{
    return button.x;
}

int Button::get_y()
{
    return button.y;
}

void Button::show(SDL_Surface* button_img, SDL_Surface* menu )
{
    SDL_Rect pos;
    pos.x = button.x;
    pos.y = button.y;
    pos.h = button.h;
    pos.w = button.w;

    //Blit the surface
    SDL_BlitSurface( button_img, NULL, menu, &pos );
}
