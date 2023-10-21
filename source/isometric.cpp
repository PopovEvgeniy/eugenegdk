#include "eugenegdk.h"

int main()
{
 int x,y,row,column;
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
 isometric.initialize(ground.get_width(),ground.get_height());
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
    x=isometric.get_target_x(row,column);
    y=isometric.get_target_y(row,column);
    ground.draw(cartesian.get_screen_x(x),cartesian.get_screen_y(y));
   }

  }
  text.print("Press Esc to exit");
 }
 return 0;
}
