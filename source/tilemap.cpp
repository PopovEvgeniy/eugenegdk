#include "eugenegdk.h"

int main()
{
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Graphics::Sheet tile;
 EUGENEGDK::Input::Keyboard keyboard;
 EUGENEGDK::Common::Tilemap tilemap;
 unsigned int row,column,row_amount,column_amount;
 keyboard.initialize();
 screen.initialize();
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 tile.load("grass.tga",6,3);
 tilemap.initialize(tile.get_width(),tile.get_height());
 row_amount=tilemap.get_row_amount(screen.get_width());
 column_amount=tilemap.get_column_amount(screen.get_height());
 tile.set_transparent(false);
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  for (row=0;row<row_amount;++row)
 {
  for (column=0;column<column_amount;++column)
  {
   tile.select(1,2);
   tile.draw(tilemap.get_x(row),tilemap.get_y(column));
  }

 }
  text.print("Press Esc to exit");
 }
 return 0;
}