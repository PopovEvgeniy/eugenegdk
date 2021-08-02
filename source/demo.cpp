#include "eugenegdk.h"

int main()
{
 char perfomance[8];
 EUGENEGDK::Timer timer;
 EUGENEGDK::Screen screen;
 EUGENEGDK::System System;
 EUGENEGDK::Keyboard keyboard;
 EUGENEGDK::Gamepad gamepad;
 EUGENEGDK::Mouse mouse;
 EUGENEGDK::Multimedia media;
 EUGENEGDK::Image image;
 EUGENEGDK::Background space;
 EUGENEGDK::Sprite ship,font;
 EUGENEGDK::Text text;
 System.enable_logging("log.txt");
 keyboard.initialize();
 screen.initialize();
 image.load_tga("space.tga");
 space.load_image(image);
 space.prepare(screen.get_width(),screen.get_height());
 image.load_tga("ship.tga");
 ship.load_sprite(image,HORIZONTAL_STRIP,2);
 ship.prepare();
 ship.set_position(screen.get_width()/2,screen.get_height()/2);
 image.load_tga("font.tga");
 font.load_image(image);
 font.prepare();
 text.load_font(font.get_handle());
 text.set_position(font.get_width(),font.get_width());
 space.set_kind(NORMAL_BACKGROUND);
 mouse.hide();
 timer.set_timer(1);
 media.initialize();
 media.load("space.mp3");
 memset(perfomance,0,8);
 while(screen.sync())
 {
  gamepad.update();
  if (media.check_playing()==false) media.play();
  if (mouse.check_press(MOUSE_LEFT)==true) break;
  if (keyboard.check_hold(57)==true) break;
  if (keyboard.check_hold(72)==true) ship.decrease_y(2);
  if (keyboard.check_hold(80)==true) ship.increase_y(2);
  if (keyboard.check_hold(75)==true) ship.decrease_x(2);
  if (keyboard.check_hold(77)==true) ship.increase_x(2);
  if (keyboard.check_press(71)==true) ship.horizontal_mirror();
  if (keyboard.check_press(79)==true) ship.vertical_mirror();
  if (gamepad.check_press(GAMEPAD_BUTTON4)==true) ship.horizontal_mirror();
  if (gamepad.check_press(GAMEPAD_BUTTON3)==true) ship.vertical_mirror();
  if (gamepad.check_hold(GAMEPAD_BUTTON2)==true) break;
  switch (gamepad.get_dpad())
  {
   case GAMEPAD_UP:
   ship.decrease_y(1);
   break;
   case GAMEPAD_DOWN:
   ship.increase_y(1);
   break;
   case GAMEPAD_UPLEFT:
   ship.decrease_y(1);
   ship.decrease_x(1);
   break;
   case GAMEPAD_UPRIGHT:
   ship.decrease_y(1);
   ship.increase_x(1);
   break;
   case GAMEPAD_DOWNLEFT:
   ship.increase_y(1);
   ship.decrease_x(1);
   break;
   case GAMEPAD_DOWNRIGHT:
   ship.increase_y(1);
   ship.increase_x(1);
   break;
   case GAMEPAD_LEFT:
   ship.decrease_x(1);
   break;
   case GAMEPAD_RIGHT:
   ship.increase_x(1);
   break;
   case GAMEPAD_NONE:
   ;
   break;
  }
  if (gamepad.get_stick_x(GAMEPAD_LEFT_STICK)==GAMEPAD_NEGATIVE_DIRECTION) ship.decrease_x(1);
  if (gamepad.get_stick_x(GAMEPAD_LEFT_STICK)==GAMEPAD_POSITIVE_DIRECTION) ship.increase_x(1);
  if (gamepad.get_stick_y(GAMEPAD_LEFT_STICK)==GAMEPAD_NEGATIVE_DIRECTION) ship.decrease_y(1);
  if (gamepad.get_stick_y(GAMEPAD_LEFT_STICK)==GAMEPAD_POSITIVE_DIRECTION) ship.increase_y(1);
  if (ship.get_x()>screen.get_width()) ship.set_x(screen.get_width()/2);
  if (ship.get_y()>screen.get_height()) ship.set_y(screen.get_height()/2);
  itoa(screen.get_fps(),perfomance,10);
  space.draw_background();
  text.draw_text(perfomance);
  ship.draw_sprite();
  if (timer.check_timer()==true)
  {
   ship.step();
  }

 }
 return 0;
}