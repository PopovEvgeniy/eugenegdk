/*
Eugene game development kit was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Eugene game development kit license

Copyright (C) 2021 Popov Evgeniy Alekseyevich

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Third-party license

Copyright (c) 2007-2010 The Khronos Group Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and/or associated documentation files (the
"Materials"), to deal in the Materials without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Materials, and to
permit persons to whom the Materials are furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Materials.

THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"winmm.lib")

//Uncomment follow lines if you will compile the code under Visual C++ 2017 or higher
/*
#pragma warning(disable : 4995)
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <cwchar>
#include <new>
#include <windows.h>
#include <unknwn.h>
#include <dshow.h>
#include <mmsystem.h>
#include <GL\gl.h>

#define GETSCANCODE(argument) ((argument >> 16)&0x7f)
#define MOUSE 3

enum MIRROR_STATUS {DISABLE_MIRRORING=1,ENABLE_MIRRORING=-1};
enum BACKGROUND_TYPE {NORMAL_BACKGROUND=0,HORIZONTAL_BACKGROUND=1,VERTICAL_BACKGROUND=2};
enum SPRITE_TYPE {SINGLE_SPRITE=0,HORIZONTAL_STRIP=1,VERTICAL_STRIP=2};
enum MOUSE_BUTTON {MOUSE_LEFT=0,MOUSE_RIGHT=1,MOUSE_MIDDLE=2};
enum GAMEPAD_DIRECTION {GAMEPAD_NEUTRAL_DIRECTION=0,GAMEPAD_NEGATIVE_DIRECTION=-1,GAMEPAD_POSITIVE_DIRECTION=1};
enum GAMEPAD_STICKS {GAMEPAD_LEFT_STICK=0,GAMEPAD_RIGHT_STICK=1};
enum GAMEPAD_DPAD {GAMEPAD_NONE=0,GAMEPAD_UP=1,GAMEPAD_DOWN=2,GAMEPAD_LEFT=3,GAMEPAD_RIGHT=4,GAMEPAD_UPLEFT=5,GAMEPAD_UPRIGHT=6,GAMEPAD_DOWNLEFT=7,GAMEPAD_DOWNRIGHT=8};
enum GAMEPAD_BUTTONS {GAMEPAD_BUTTON1=JOY_BUTTON1,GAMEPAD_BUTTON2=JOY_BUTTON2,GAMEPAD_BUTTON3=JOY_BUTTON3,GAMEPAD_BUTTON4=JOY_BUTTON4,GAMEPAD_BUTTON5=JOY_BUTTON5,GAMEPAD_BUTTON6=JOY_BUTTON6,GAMEPAD_BUTTON7=JOY_BUTTON7,GAMEPAD_BUTTON8=JOY_BUTTON8,GAMEPAD_BUTTON9=JOY_BUTTON9,GAMEPAD_BUTTON10=JOY_BUTTON10,GAMEPAD_BUTTON11=JOY_BUTTON11,GAMEPAD_BUTTON12=JOY_BUTTON12,GAMEPAD_BUTTON113=JOY_BUTTON13,GAMEPAD_BUTTON14=JOY_BUTTON14,GAMEPAD_BUTTON15=JOY_BUTTON15,GAMEPAD_BUTTON16=JOY_BUTTON16,GAMEPAD_BUTTON17=JOY_BUTTON17,GAMEPAD_BUTTON18=JOY_BUTTON18,GAMEPAD_BUTTON19=JOY_BUTTON19,GAMEPAD_BUTTON20=JOY_BUTTON20,GAMEPAD_BUTTON21=JOY_BUTTON21,GAMEPAD_BUTTON22=JOY_BUTTON22,GAMEPAD_BUTTON23=JOY_BUTTON23,GAMEPAD_BUTTON24=JOY_BUTTON24,GAMEPAD_BUTTON25=JOY_BUTTON25,GAMEPAD_BUTTON26=JOY_BUTTON26,GAMEPAD_BUTTON27=JOY_BUTTON27,GAMEPAD_BUTTON28=JOY_BUTTON28,GAMEPAD_BUTTON29=JOY_BUTTON29,GAMEPAD_BUTTON30=JOY_BUTTON30,GAMEPAD_BUTTON31=JOY_BUTTON31,GAMEPAD_BUTTON32=JOY_BUTTON32};

extern BOOL WINAPI wglSwapIntervalEXT (int interval); // This code was taken from wglext.h by The Khronos Group Inc
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval); // This code was taken from wglext.h by The Khronos Group Inc

struct Collision_Box
{
 unsigned int x;
 unsigned int y;
 unsigned int width;
 unsigned int height;
};

namespace EUGENEGDK
{

 void Halt(const char *message);

 namespace Internal
 {

  LRESULT CALLBACK Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam);

  class Synchronization
  {
   private:
   HANDLE timer;
   protected:
   void create_timer();
   void set_timer(const unsigned long int interval);
   void wait_timer();
   public:
   Synchronization();
   ~Synchronization();
  };

  class Display
  {
   private:
   DEVMODE display;
   void get_video_mode();
   void set_video_mode();
   void set_setting(const unsigned long int width,const unsigned long int height);
   protected:
   void check_video_mode();
   void set_resolution(const unsigned long int width,const unsigned long int height);
   unsigned long int get_depth() const;
   public:
   Display();
   ~Display();
  };

  class Engine
  {
   private:
   WNDCLASSEX window_class;
   HWND window;
   HDC context;
   void get_instance();
   void set_backgrond_color();
   void load_icon();
   void load_cursor();
   void register_window_class();
   void take_context();
   void create_window();
   protected:
   HDC get_context();
   void prepare_engine();
   bool process_message();
   unsigned int get_screen_width();
   unsigned int get_screen_height();
   public:
   Engine();
   ~Engine();
  };

  class WINGL
  {
   private:
   HDC device;
   HGLRC render;
   PIXELFORMATDESCRIPTOR setting;
   PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
   int get_pixel_format(HDC target,const unsigned int color);
   void set_pixel_format(const int format);
   void create_render_context();
   void disable_vsync();
   protected:
   void set_render(HDC target,const unsigned int color);
   void Swap();
   bool is_render_accelerated() const;
   public:
   WINGL();
   ~WINGL();
  };

 }

 namespace Core
 {

  struct Vertex
 {
  int x;
  int y;
 };

 struct Point
 {
  float u;
  float v;
 };

  class Unicode_Convertor
  {
   private:
   wchar_t *target;
   void get_memory(const size_t length);
   void clear_buffer(const size_t length);
   void create_buffer(const size_t length);
   void convert_string(const char *source);
   public:
   Unicode_Convertor();
   ~Unicode_Convertor();
   wchar_t *convert(const char *source);
  };

  class Resizer
  {
   private:
   unsigned int *image;
   unsigned int size_limit;
   unsigned int source_width;
   unsigned int source_height;
   unsigned int target_width;
   unsigned int target_height;
   size_t get_source_offset(const unsigned int x,const unsigned int y) const;
   size_t get_target_offset(const unsigned int x,const unsigned int y) const;
   void resize_image(const unsigned int *target);
   void set_setting(const unsigned int width,const unsigned int height,const unsigned int limit);
   void calculate_size();
   void correct_size();
   void create_buffer();
   public:
   Resizer();
   ~Resizer();
   void make_texture(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit);
   unsigned int get_width() const;
   unsigned int get_height() const;
   unsigned int *get_buffer();
  };

  class FPS
  {
   private:
   time_t start;
   unsigned int current;
   unsigned int fps;
   protected:
   void update_counter();
   unsigned int get_fps_amount() const;
   public:
   FPS();
   ~FPS();
  };

  class Shape
  {
   private:
   unsigned int target_width;
   unsigned int target_height;
   unsigned int total_width;
   unsigned int total_height;
   unsigned int current_x;
   unsigned int current_y;
   MIRROR_STATUS horizontal_mirror;
   MIRROR_STATUS vertical_mirror;
   float get_start_offset(const float current,const float total);
   float get_end_offset(const float current,const float total);
   protected:
   Vertex vertex[4];
   Point point[4];
   void reset_data();
   unsigned int get_total_width() const;
   unsigned int get_total_height() const;
   unsigned int get_x() const;
   unsigned int get_y() const;
   public:
   Shape();
   ~Shape();
   void set_total_size(const unsigned int width,const unsigned int height);
   void set_size(const unsigned int width,const unsigned int height);
   void set_position(const unsigned int x,const unsigned int y);
   void set_horizontal_offset(const float current,const float total);
   void set_vertical_offset(const float current,const float total);
   void set_tile_offset(const float row,const float rows,const float column,const float columns);
   void set_mirror_status(const MIRROR_STATUS horizontal,const MIRROR_STATUS vertical);
   MIRROR_STATUS get_horizontal_mirror() const;
   MIRROR_STATUS get_vertical_mirror() const;
  };

  class Rectangle:public Shape
  {
   private:
   unsigned int texture;
   void create_texture(const unsigned int *buffer);
   void delete_texture();
   void check_texture();
   void load_data();
   void draw_rectangle();
   void reset_model_setting();
   void set_model_setting();
   public:
   Rectangle();
   ~Rectangle();
   void enable_transparent();
   void disable_transparent();
   void prepare(const unsigned int *buffer);
   void draw();
   void destroy_texture();
   bool is_texture_exist() const;
  };

  class Render
  {
   private:
   unsigned int get_maximum_texture_size() const;
   void set_perfomance_setting();
   void set_render_hints();
   void set_common_setting();
   void set_perspective(const unsigned int width,const unsigned int height);
   void create_render(const unsigned int width,const unsigned int height);
   protected:
   void clear_stage();
   void start_render(const unsigned int width,const unsigned int height);
   public:
   Render();
   ~Render();
  };

 }

 namespace Input
 {

  class Keyboard
  {
   private:
   unsigned char *preversion;
   void create_buffer();
   void clear_buffer();
   bool check_state(const unsigned char code,const unsigned char state);
   public:
   Keyboard();
   ~Keyboard();
   void initialize();
   bool check_hold(const unsigned char code);
   bool check_press(const unsigned char code);
   bool check_release(const unsigned char code);
   bool is_ready() const;
  };

  class Mouse
  {
   private:
   unsigned char preversion[MOUSE];
   POINT position;
   void get_position();
   bool check_state(const MOUSE_BUTTON button,const unsigned char state);
   public:
   Mouse();
   ~Mouse();
   void show();
   void hide();
   void set_position(const unsigned int x,const unsigned int y);
   unsigned int get_x();
   unsigned int get_y();
   bool check_hold(const MOUSE_BUTTON button);
   bool check_press(const MOUSE_BUTTON button);
   bool check_release(const MOUSE_BUTTON button);
  };

  class Gamepad
  {
   private:
   JOYINFOEX current;
   JOYINFOEX preversion;
   JOYCAPS configuration;
   unsigned int active;
   bool read_configuration();
   bool read_state();
   void clear_state();
   bool check_button(const GAMEPAD_BUTTONS button,const JOYINFOEX &target);
   public:
   Gamepad();
   ~Gamepad();
   unsigned int get_amount();
   unsigned int get_button_amount();
   unsigned int get_last_index();
   bool check_connection();
   void update();
   unsigned long int get_sticks_amount();
   void set_active(const unsigned int gamepad);
   unsigned int get_max_amount() const;
   unsigned int get_active() const;
   GAMEPAD_DPAD get_dpad() const;
   GAMEPAD_DIRECTION get_stick_x(const GAMEPAD_STICKS stick);
   GAMEPAD_DIRECTION get_stick_y(const GAMEPAD_STICKS stick);
   bool check_hold(const GAMEPAD_BUTTONS button);
   bool check_press(const GAMEPAD_BUTTONS button);
   bool check_release(const GAMEPAD_BUTTONS button);
  };

 }

 namespace Misc
 {

  class Multimedia
  {
   private:
   IGraphBuilder *loader;
   IMediaControl *player;
   IMediaSeeking *controler;
   IVideoWindow *video;
   void com_setup();
   void set_screen_mode();
   void load_content(const wchar_t *target);
   void open(const wchar_t *target);
   bool is_play();
   void rewind();
   void play_content();
   void create_loader();
   void create_player();
   void create_controler();
   void create_video_player();
   public:
   Multimedia();
   ~Multimedia();
   void initialize();
   bool check_playing();
   void stop();
   void play();
   void load(const char *target);
   void initialize(const char *target);
  };

  class Memory
  {
   private:
   MEMORYSTATUSEX memory;
   void get_status();
   public:
   Memory();
   ~Memory();
   unsigned long long int get_total_physical();
   unsigned long long int get_free_physical();
   unsigned long long int get_total_virtual();
   unsigned long long int get_free_virtual();
   unsigned long int get_usage();
  };

 }

 namespace File
 {

  class Binary_File
  {
   protected:
   FILE *target;
   public:
   Binary_File();
   ~Binary_File();
   void close();
   void set_position(const long int offset);
   long int get_position();
   long int get_length();
   bool check_error();
   bool is_open() const;
  };

  class Input_File:public Binary_File
  {
   public:
   Input_File();
   ~Input_File();
   void open(const char *name);
   void read(void *buffer,const size_t length);
  };

  class Output_File:public Binary_File
  {
   public:
   Output_File();
   ~Output_File();
   void open(const char *name);
   void create_temp();
   void write(void *buffer,const size_t length);
   void flush();
  };

 }

 namespace Graphics
 {

  class Screen:public Core::FPS, public Core::Render, public Internal::Engine, public Internal::Display, public Internal::WINGL, public Internal::Synchronization
 {
   private:
   bool ready;
   void screen_setup();
   public:
   Screen();
   ~Screen();
   void clear_screen();
   void initialize();
   void initialize(const unsigned int width,const unsigned int height);
   bool update();
   bool sync();
   bool is_ready() const;
   bool is_accelerated() const;
   unsigned long int get_color() const;
   unsigned int get_fps() const;
   unsigned int get_width();
   unsigned int get_height();
   Screen* get_handle();
  };

   class Image
  {
   private:
   unsigned int width;
   unsigned int height;
   unsigned char *data;
   unsigned char *create_buffer(const size_t length);
   void copy_data(const unsigned char *target,const size_t location,const size_t position,const size_t amount);
   void uncompress_tga_data(const unsigned char *target,const size_t length);
   void load_tga(File::Input_File &target);
   public:
   Image();
   ~Image();
   unsigned int get_width() const;
   unsigned int get_height() const;
   size_t get_length() const;
   unsigned char *get_data();
   void destroy_image();
   void load_tga(const char *name);
  };

  class Picture
  {
   private:
   unsigned int *image;
   unsigned int image_width;
   unsigned int image_height;
   size_t length;
   protected:
   void set_image_size(const unsigned int width,const unsigned int height);
   unsigned int *create_buffer();
   void set_buffer(unsigned int *buffer);
   void copy_image(const unsigned int *target);
   unsigned int *get_buffer();
   public:
   Picture();
   ~Picture();
   size_t get_length() const;
   unsigned int *get_image();
   void destroy_image();
   void load_image(Image &buffer);
   bool is_storage_empty() const;
   unsigned int get_image_width() const;
   unsigned int get_image_height() const;
  };

  class Animation
  {
   private:
   unsigned int frames;
   unsigned int frame;
   protected:
   void reset_animation_setting();
   void increase_frame();
   void set_frame(const unsigned int target);
   void set_frames(const unsigned int amount);
   public:
   Animation();
   ~Animation();
   unsigned int get_frames() const;
   unsigned int get_frame() const;
  };

  class Background:public Animation,public Picture
  {
   private:
   Core::Rectangle target;
   BACKGROUND_TYPE current_kind;
   void reset_background_setting();
   void configure_background();
   void set_kind(const BACKGROUND_TYPE kind);
   void prepare(const unsigned int screen_width,const unsigned int screen_height);
   public:
   Background();
   ~Background();
   void prepare(Screen *screen);
   void prepare(Screen &screen);
   void set_setting(const BACKGROUND_TYPE kind,const unsigned int frames);
   void load_background(Image &buffer,const BACKGROUND_TYPE kind,const unsigned int frames);
   void set_target(const unsigned int target);
   void step();
   void horizontal_mirror();
   void vertical_mirror();
   void complex_mirror();
   void draw_background();
   void destroy_background();
   BACKGROUND_TYPE get_kind() const;
  };

  class Sprite:public Animation,public Picture
  {
   private:
   Core::Rectangle target;
   bool transparent;
   unsigned int current_x;
   unsigned int current_y;
   unsigned int sprite_width;
   unsigned int sprite_height;
   SPRITE_TYPE current_kind;
   void reset_sprite_setting();
   void check_transparent();
   void draw_sprite_image();
   void set_sprite_setting();
   void configure_sprite();
   void set_sprite_frame();
   void set_kind(const SPRITE_TYPE kind);
   public:
   Sprite();
   ~Sprite();
   void prepare();
   void set_transparent(const bool enabled);
   bool get_transparent() const;
   void set_width(const unsigned int width);
   void set_height(const unsigned int height);
   void set_size(const unsigned int width,const unsigned int height);
   void set_position(const unsigned int x,const unsigned int y);
   void set_x(const unsigned int x);
   void set_y(const unsigned int y);
   void increase_x();
   void decrease_x();
   void increase_y();
   void decrease_y();
   void increase_x(const unsigned int increment);
   void decrease_x(const unsigned int decrement);
   void increase_y(const unsigned int increment);
   void decrease_y(const unsigned int decrement);
   unsigned int get_x() const;
   unsigned int get_y() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   Sprite* get_handle();
   Collision_Box get_box() const;
   SPRITE_TYPE get_kind() const;
   void set_setting(const SPRITE_TYPE kind,const unsigned int frames);
   void load_sprite(Image &buffer,const SPRITE_TYPE kind,const unsigned int frames);
   void set_target(const unsigned int target);
   void step();
   void clone(Sprite &target);
   void horizontal_mirror();
   void vertical_mirror();
   void complex_mirror();
   void destroy_sprite();
   void draw_sprite();
   void draw_sprite(const unsigned int x,const unsigned int y);
   void draw_sprite(const bool transparency);
   void draw_sprite(const bool transparency,const unsigned int x,const unsigned int y);
  };

  class Tileset:public Picture
  {
   private:
   Core::Rectangle target;
   unsigned int tile_width;
   unsigned int tile_height;
   unsigned int rows;
   unsigned int columns;
   void reset_tileset_setting();
   void prepare();
   void set_tileset_setting(const unsigned int row_amount,const unsigned int column_amount);
   public:
   Tileset();
   ~Tileset();
   unsigned int get_tile_width() const;
   unsigned int get_tile_height() const;
   unsigned int get_rows() const;
   unsigned int get_columns() const;
   void set_tile_size(const unsigned int width,const unsigned int height);
   void select_tile(const unsigned int row,const unsigned int column);
   void destroy_tileset();
   void draw_tile(const unsigned int x,const unsigned int y);
   void draw_tile(const unsigned int row,const unsigned int column,const unsigned int x,const unsigned int y);
   void load_tileset(Image &buffer,const unsigned int row_amount,const unsigned int column_amount);
  };

  class Text
  {
   private:
   unsigned int current_x;
   unsigned int current_y;
   Sprite *font;
   void increase_position();
   void restore_position();
   void print_character(const char target);
   void print_text(const char *text);
   public:
   Text();
   ~Text();
   void set_position(const unsigned int x,const unsigned int y);
   void load_font(Sprite *target);
   void load_font(Sprite &target);
   void draw_character(const char target);
   void draw_text(const char *text);
   void draw_character(const unsigned int x,const unsigned int y,const char target);
   void draw_text(const unsigned int x,const unsigned int y,const char *text);
  };

 }

 namespace Common
 {

  class System
  {
   public:
   System();
   ~System();
   unsigned int get_random(const unsigned int number);
   void quit();
   void run(const char *command);
   char* read_environment(const char *variable);
   bool enable_logging(const char *name);
  };

  class Filesystem
  {
   public:
   Filesystem();
   ~Filesystem();
   bool file_exist(const char *name);
   bool delete_file(const char *name);
  };

  class Timer
  {
   private:
   double interval;
   time_t start;
   public:
   Timer();
   ~Timer();
   void set_timer(const double seconds);
   bool check_timer();
  };

  class Collision
  {
   private:
   Collision_Box first;
   Collision_Box second;
   public:
   Collision();
   ~Collision();
   void set_target(const Collision_Box &first_target,const Collision_Box &second_target);
   bool check_horizontal_collision() const;
   bool check_vertical_collision() const;
   bool check_collision() const;
   bool check_horizontal_collision(const Collision_Box &first_target,const Collision_Box &second_target);
   bool check_vertical_collision(const Collision_Box &first_target,const Collision_Box &second_target);
   bool check_collision(const Collision_Box &first_target,const Collision_Box &second_target);
   Collision_Box generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height) const;
  };

 }

}