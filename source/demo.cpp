#include "eugenegdk.h"

int main()
{
 char perfomance[8];
 EUGENEGDK::Common::Timer timer;
 EUGENEGDK::Input::Keyboard keyboard;
 EUGENEGDK::Input::Gamepad gamepad;
 EUGENEGDK::Input::Mouse mouse;
 EUGENEGDK::Misc::Audio media;
 EUGENEGDK::Graphics::Screen screen;
 EUGENEGDK::Graphics::Background space;
 EUGENEGDK::Graphics::Sprite ship;
 EUGENEGDK::Graphics::Text text;
 EUGENEGDK::Tools::enable_logging("log.txt");
 keyboard.initialize();
 screen.initialize();
 space.load("space.tga");
 space.prepare(screen);
 ship.load("ship.tga",EUGENEGDK::HORIZONTAL_ANIMATED,2);
 ship.set_position(screen.get_width()/2,screen.get_height()/2);
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 mouse.hide();
 timer.set_timer(1);
 media.initialize();
 media.load("space.mp3");
 memset(perfomance,0,8);
 while(screen.sync())
 {
  gamepad.update();
  media.play_loop();
  if (mouse.check_press(EUGENEGDK::MOUSE_LEFT)==true)
  {
   break;
  }
  if (keyboard.check_hold(57)==true)
  {
   break;
  }
  if (keyboard.check_hold(59)==true)
  {
   space.disable_mirror();
  }
  if (keyboard.check_hold(60)==true)
  {
   space.horizontal_mirror();
  }
  if (keyboard.check_hold(61)==true)
  {
   space.vertical_mirror();
  }
  if (keyboard.check_hold(62)==true)
  {
   space.complex_mirror();
  }
  if (keyboard.check_hold(72)==true)
  {
   ship.decrease_y(2);
  }
  if (keyboard.check_hold(80)==true)
  {
   ship.increase_y(2);
  }
  if (keyboard.check_hold(75)==true)
  {
   ship.decrease_x(2);
  }
  if (keyboard.check_hold(77)==true)
  {
   ship.increase_x(2);
  }
  if (gamepad.check_hold(EUGENEGDK::GAMEPAD_BUTTON2)==true)
  {
   break;
  }
  switch (gamepad.get_dpad())
  {
   case EUGENEGDK::GAMEPAD_UP:
   ship.decrease_y();
   break;
   case EUGENEGDK::GAMEPAD_DOWN:
   ship.increase_y();
   break;
   case EUGENEGDK::GAMEPAD_UPLEFT:
   ship.decrease_y();
   ship.decrease_x();
   break;
   case EUGENEGDK::GAMEPAD_UPRIGHT:
   ship.decrease_y();
   ship.increase_x();
   break;
   case EUGENEGDK::GAMEPAD_DOWNLEFT:
   ship.increase_y();
   ship.decrease_x();
   break;
   case EUGENEGDK::GAMEPAD_DOWNRIGHT:
   ship.increase_y();
   ship.increase_x();
   break;
   case EUGENEGDK::GAMEPAD_LEFT:
   ship.decrease_x();
   break;
   case EUGENEGDK::GAMEPAD_RIGHT:
   ship.increase_x();
   break;
   default:
   ;
   break;
  }
  if (gamepad.get_stick_x(EUGENEGDK::GAMEPAD_RIGHT_STICK)==EUGENEGDK::GAMEPAD_NEGATIVE_DIRECTION)
  {
   ship.decrease_x();
  }
  if (gamepad.get_stick_x(EUGENEGDK::GAMEPAD_RIGHT_STICK)==EUGENEGDK::GAMEPAD_POSITIVE_DIRECTION)
  {
   ship.increase_x();
  }
  if (gamepad.get_stick_y(EUGENEGDK::GAMEPAD_RIGHT_STICK)==EUGENEGDK::GAMEPAD_NEGATIVE_DIRECTION)
  {
   ship.decrease_y();
  }
  if (gamepad.get_stick_y(EUGENEGDK::GAMEPAD_RIGHT_STICK)==EUGENEGDK::GAMEPAD_POSITIVE_DIRECTION)
  {
   ship.increase_y();
  }
  if (ship.get_x()>screen.get_width())
  {
   ship.set_x(screen.get_width()/2);
  }
  if (ship.get_y()>screen.get_height())
  {
   ship.set_y(screen.get_height()/2);
  }
  itoa(screen.get_fps(),perfomance,10);
  space.draw();
  text.print(perfomance);
  ship.draw();
  if (timer.check_timer()==true)
  {
   ship.step();
  }

 }
 return 0;
}