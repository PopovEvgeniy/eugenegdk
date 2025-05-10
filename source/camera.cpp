#include "eugenegdk.h"

int main()
{
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Scene city;
 EUGENEGDK::Graphics::Cartoon punk;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Graphics::Camera camera;
 EUGENEGDK::Input::Keyboard keyboard;
 EUGENEGDK::Input::Mouse mouse;
 keyboard.initialize();
 mouse.hide();
 screen.initialize(640,480);
 city.load("city.tga");
 punk.load("punk.tga");
 text.load_font("font.tga");
 city.prepare(screen);
 camera.initialize(screen);
 camera.set_viewport(320,screen.get_height());
 camera.update();
 punk.set_size(96,140);
 punk.set_position(0,camera.get_viewport_height()-punk.get_height());
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  if (keyboard.check_hold(77)==true)
  {
   punk.increase_x(3);
   camera.increase_x(5);
  }
  if (keyboard.check_hold(75)==true)
  {
   punk.decrease_x(3);
   camera.decrease_x(5);
  }
  if ((punk.get_x()+punk.get_width())>=camera.get_screen_width())
  {
   punk.decrease_x(3);
  }
  camera.update();
  city.draw();
  punk.draw();
 }
 return 0;
}