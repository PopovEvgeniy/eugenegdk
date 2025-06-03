#include "eugenegdk.h"

int main()
{
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Scene city;
 EUGENEGDK::Graphics::Cartoon punk;
 EUGENEGDK::Graphics::Camera camera;
 EUGENEGDK::Input::Keyboard keyboard;
 EUGENEGDK::Input::Mouse mouse;
 keyboard.initialize();
 mouse.hide();
 screen.initialize(640,480);
 city.load("city.tga");
 punk.load("punk.tga");
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
   if (camera.check_horizontal_border(punk.get_box())==true)
   {
    camera.increase_x(12);
   }
   else
   {
    punk.increase_x();
   }

  }
  if (keyboard.check_hold(75)==true)
  {
   if (punk.get_x()<=camera.get_lowest_x())
   {
    camera.decrease_x(12);
   }
   else
   {
    punk.decrease_x();
   }

  }
  camera.update();
  city.draw();
  punk.draw();
 }
 return 0;
}