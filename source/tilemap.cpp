#include "eugenegdk.h"

int main()
{
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Graphics::Sheet tilemap;
 EUGENEGDK::Input::Keyboard keyboard;
 unsigned int x,y;
 keyboard.initialize();
 screen.initialize();
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 tilemap.load("grass.tga",6,3);
 tilemap.set_transparent(false);
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  for (x=0;x<screen.get_width();x+=tilemap.get_width())
 {
  for (y=0;y<screen.get_height();y+=tilemap.get_height())
  {
   tilemap.select(1,2);
   tilemap.draw(x,y);
  }

 }
  text.print("Press Esc to exit");
 }
 return 0;
}