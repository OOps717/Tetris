#include <string>
#ifndef _BUTTON_
#define _BUTTON_


class Button
{
public:
  Button( int x, int y, int w, int h, std::string name);
  bool handle_events(SDL_Event event);
  int get_x();
  int get_y();
  void show(SDL_Surface* button_img, SDL_Surface* menu );
// protected:

private:
  SDL_Rect button;
};

#endif
