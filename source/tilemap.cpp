#include "eugenegdk.h"

int main()
{
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Graphics::Image image;
 EUGENEGDK::Graphics::Sheet tilemap;
 EUGENEGDK::Input::Keyboard keyboard;
 unsigned int x,y;
 keyboard.initialize();
 screen.initialize();
 image.load_tga("font.tga");
 text.load_font(image);
 text.set_position(text.get_font_width(),text.get_font_height());
 image.load_tga("grass.tga");
 tilemap.load_sheet(image,6,3);
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
   tilemap.select(0,1);
   tilemap.draw_sprite(x,y);
  }

 }
  text.draw_text("Press Esc to exit");
 }
 return 0;
}