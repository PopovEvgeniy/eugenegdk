#include "eugenegdk.h"

int main()
{
 unsigned int source_x,source_y,start_x,start_y,row,column;
 int cartesian_x,cartesian_y,isometric_x,isometric_y;
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Background sky;
 EUGENEGDK::Graphics::Sprite ground;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Transformation::Coordinates cartesian;
 EUGENEGDK::Transformation::Isometric isometric;
 EUGENEGDK::Input::Keyboard keyboard;
 keyboard.initialize();
 screen.initialize();
 sky.load("sky.tga");
 sky.prepare(screen);
 ground.load("ground.tga");
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 cartesian.initialize(screen.get_width(),screen.get_height());
 start_x=screen.get_width()/2;
 start_y=screen.get_height()/2;
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  sky.draw();
  for (row=0;row<5;++row)
  {
   for (column=0;column<5;++column)
   {
    source_x=row*ground.get_width()/2;
    source_y=column*ground.get_height();
    cartesian_x=cartesian.get_cartesian_x(source_x+start_x);
    cartesian_y=cartesian.get_cartesian_y(source_y+start_y);
    isometric_x=isometric.get_isometric_x(cartesian_x,cartesian_y);
    isometric_y=isometric.get_isometric_y(cartesian_x,cartesian_y);
    ground.draw(cartesian.get_screen_x(isometric_x),cartesian.get_screen_y(isometric_y));
   }

  }
  text.print("Press Esc to exit");
 }
 return 0;
}
