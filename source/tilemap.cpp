#include "eugenegdk.h"

int main()
{
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Graphics::Image image;
 EUGENEGDK::Graphics::Tileset tilemap;
 EUGENEGDK::Input::Keyboard keyboard;
 unsigned int x,y;
 keyboard.initialize();
 screen.initialize();
 image.load_tga("font.tga");
 text.load_font(image);
 text.set_position(text.get_font_width(),text.get_font_height());
 image.load_tga("grass.tga");
 tilemap.load_tileset(image,6,3);
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  for (x=0;x<screen.get_width();x+=tilemap.get_tile_width())
 {
  for (y=0;y<screen.get_height();y+=tilemap.get_tile_height())
  {
   tilemap.draw_tile(1,2,x,y);
  }

 }
  text.draw_text("Press Esc to exit");
 }
 return 0;
}