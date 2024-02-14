#include "eugenegdk.h"

int main()
{
 int x,y,row,column;
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Scene sky;
 EUGENEGDK::Graphics::Cartoon ground;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Transformation::Coordinates cartesian;
 EUGENEGDK::Transformation::World level;
 EUGENEGDK::Input::Keyboard keyboard;
 keyboard.initialize();
 screen.initialize();
 sky.load("sky.tga");
 sky.prepare(screen);
 ground.load("ground.tga");
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 cartesian.initialize(screen.get_width(),screen.get_height());
 level.initialize(ground.get_width(),ground.get_height(),screen.get_width(),screen.get_height());
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  sky.draw();
  for (row=-5;row<5;++row)
  {
   for (column=-5;column<5;++column)
   {
    x=level.get_target_x(row,column);
    y=level.get_target_y(row,column);
    ground.draw(cartesian.get_screen_x(x),cartesian.get_screen_y(y));
   }

  }
  text.print("Press Esc to exit");
 }
 return 0;
}
