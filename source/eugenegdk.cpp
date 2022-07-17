/*
Eugene game development kit was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Eugene game development kit license

Copyright (C) 2021 - 2022 Popov Evgeniy Alekseyevich

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

#include "eugenegdk.h"

namespace
{

 typedef struct
 {
  unsigned char id:8;
  unsigned char color_map:8;
  unsigned char type:8;
 } TGA_head;

 typedef struct
 {
  unsigned short int index:16;
  unsigned short int length:16;
  unsigned char map_size:8;
 } TGA_map;

 typedef struct
 {
  unsigned short int x:16;
  unsigned short int y:16;
  unsigned short int width:16;
  unsigned short int height:16;
  unsigned char color:8;
  unsigned char alpha:3;
  unsigned char direction:5;
 } TGA_image;

 unsigned int MAXIMUM_TEXTURE_SIZE=0;
 const size_t KEYBOARD=256;
 const size_t MOUSE=3;
 const unsigned char KEY_RELEASE=0;
 const unsigned char KEY_PRESS=1;
 const unsigned long int JOYSTICK_UPLEFT=31500;
 const unsigned long int JOYSTICK_UPRIGHT=4500;
 const unsigned long int JOYSTICK_DOWNLEFT=22500;
 const unsigned long int JOYSTICK_DOWNRIGHT=13500;
 const unsigned int RECTANGLE_VERTEXES=4;
 const unsigned char IMAGE_COLOR=32;

 unsigned char Keys[KEYBOARD]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};
 unsigned char Buttons[MOUSE]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};
}

namespace EUGENEGDK
{

 void Halt(const char *message)
 {
  puts(message);
  exit(EXIT_FAILURE);
 }

 namespace Internal
 {

  LRESULT CALLBACK Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam)
  {
   switch (Message)
   {
    case WM_CLOSE:
    DestroyWindow(window);
    break;
    case WM_DESTROY:
    PostQuitMessage(0);
    break;
    case WM_LBUTTONDOWN:
    Buttons[EUGENEGDK::MOUSE_LEFT]=KEY_PRESS;
    break;
    case WM_LBUTTONUP:
    Buttons[EUGENEGDK::MOUSE_LEFT]=KEY_RELEASE;
    break;
    case WM_RBUTTONDOWN:
    Buttons[EUGENEGDK::MOUSE_RIGHT]=KEY_PRESS;
    break;
    case WM_RBUTTONUP:
    Buttons[EUGENEGDK::MOUSE_RIGHT]=KEY_RELEASE;
    break;
    case WM_MBUTTONDOWN:
    Buttons[EUGENEGDK::MOUSE_MIDDLE]=KEY_PRESS;
    break;
    case WM_MBUTTONUP:
    Buttons[EUGENEGDK::MOUSE_MIDDLE]=KEY_RELEASE;
    break;
    case WM_KEYDOWN:
    Keys[LOBYTE(HIWORD(lParam))]=KEY_PRESS;
    break;
    case WM_KEYUP:
    Keys[LOBYTE(HIWORD(lParam))]=KEY_RELEASE;
    break;
    default:
    ;
    break;
   }
   return DefWindowProc(window,Message,wParam,lParam);
  }

  Synchronization::Synchronization()
  {
   timer=NULL;
  }

  Synchronization::~Synchronization()
  {
   if (timer!=NULL)
   {
    CancelWaitableTimer(timer);
    CloseHandle(timer);
    timer=NULL;
   }

  }

  void Synchronization::create_timer()
  {
   timer=CreateWaitableTimer(NULL,FALSE,NULL);
   if (timer==NULL)
   {
    EUGENEGDK::Halt("Can't create synchronization timer");
   }

  }

  void Synchronization::set_timer(const unsigned long int interval)
  {
   LARGE_INTEGER start;
   start.QuadPart=0;
   if (SetWaitableTimer(timer,&start,interval,NULL,NULL,FALSE)==FALSE)
   {
    EUGENEGDK::Halt("Can't set timer");
   }

  }

  void Synchronization::wait_timer()
  {
   if (timer!=NULL)
   {
    WaitForSingleObjectEx(timer,INFINITE,TRUE);
   }

  }

  Display::Display()
  {
   memset(&display,0,sizeof(DEVMODE));
   display.dmSize=sizeof(DEVMODE);
  }

  Display::~Display()
  {
   ChangeDisplaySettingsEx(NULL,NULL,NULL,0,NULL);
  }

  void Display::set_video_mode()
  {
   if (ChangeDisplaySettingsEx(NULL,&display,NULL,CDS_FULLSCREEN,NULL)!=DISP_CHANGE_SUCCESSFUL)
   {
    EUGENEGDK::Halt("Can't change video mode");
   }

  }

  void Display::get_video_mode()
  {
   if (EnumDisplaySettingsEx(NULL,ENUM_CURRENT_SETTINGS,&display,EDS_RAWMODE)==FALSE)
   {
    EUGENEGDK::Halt("Can't get display setting");
   }

  }

  void Display::correct_depth()
  {
   if (display.dmBitsPerPel<16)
   {
    display.dmBitsPerPel=16;
   }

  }

  void Display::set_setting(const unsigned long int width,const unsigned long int height)
  {
   display.dmPelsWidth=width;
   display.dmPelsHeight=height;
  }

  unsigned long int Display::get_depth() const
  {
   return display.dmBitsPerPel;
  }

  unsigned long int Display::get_display_width() const
  {
   return display.dmPelsWidth;
  }

  unsigned long int Display::get_display_height() const
  {
   return display.dmPelsHeight;
  }

  Engine::Engine()
  {
   window_class.lpszClassName=TEXT("EUGENEGDK");
   window_class.style=CS_OWNDC;
   window_class.cbSize=sizeof(WNDCLASSEX);
   window_class.lpfnWndProc=Internal::Process_Message;
   window_class.hInstance=NULL;
   window_class.hbrBackground=NULL;
   window_class.hIcon=NULL;
   window_class.hCursor=NULL;
   window_class.hIconSm=NULL;
   window_class.cbClsExtra=0;
   window_class.cbWndExtra=0;
   window=NULL;
   context=NULL;
  }

  Engine::~Engine()
  {
   if (context!=NULL)
   {
    ReleaseDC(window,context);
    context=NULL;
   }
   if (window!=NULL)
   {
    CloseWindow(window);
    window=NULL;
   }
   if (window_class.hbrBackground!=NULL)
   {
    DeleteObject(window_class.hbrBackground);
    window_class.hbrBackground=NULL;
   }
   UnregisterClass(window_class.lpszClassName,window_class.hInstance);
  }

  void Engine::get_instance()
  {
   if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,NULL,&window_class.hInstance)==FALSE)
   {
    EUGENEGDK::Halt("Can't get the application instance");
   }

  }

  void Engine::set_backgrond_color()
  {
   window_class.hbrBackground=CreateSolidBrush(RGB(0,0,0));
   if (window_class.hbrBackground==NULL)
   {
    EUGENEGDK::Halt("Can't set background color");
   }

  }

  void Engine::load_icon()
  {
   window_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
   if (window_class.hIcon==NULL)
   {
    EUGENEGDK::Halt("Can't load the standart program icon");
   }

  }

  void Engine::load_cursor()
  {
   window_class.hCursor=LoadCursor(NULL,IDC_ARROW);
   if (window_class.hCursor==NULL)
   {
    EUGENEGDK::Halt("Can't load the standart cursor");
   }

  }

  void Engine::register_window_class()
  {
   if (RegisterClassEx(&window_class)==0)
   {
    EUGENEGDK::Halt("Can't register window class");
   }

  }

  void Engine::take_context()
  {
   context=GetWindowDC(window);
   if (context==NULL)
   {
    EUGENEGDK::Halt("Can't take window context");
   }

  }

  void Engine::create_window()
  {
   window=CreateWindowEx(WS_EX_APPWINDOW,window_class.lpszClassName,NULL,WS_VISIBLE|WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL,NULL,window_class.hInstance,NULL);
   if (window==NULL)
   {
    EUGENEGDK::Halt("Can't create window");
   }
   SetFocus(window);
  }

  HDC Engine::get_context()
  {
   return context;
  }

  void Engine::prepare_engine()
  {
   this->get_instance();
   this->set_backgrond_color();
   this->load_icon();
   this->load_cursor();
   this->register_window_class();
   this->create_window();
   this->take_context();
  }

  bool Engine::process_message()
  {
   bool run;
   MSG Message;
   run=true;
   while(PeekMessage(&Message,window,0,0,PM_NOREMOVE)==TRUE)
   {
    if (GetMessage(&Message,window,0,0)==TRUE)
    {
     DispatchMessage(&Message);
    }
    else
    {
     run=false;
     break;
    }

   }
   return run;
  }

  WINGL::WINGL()
  {
   memset(&setting,0,sizeof(PIXELFORMATDESCRIPTOR));
   device=NULL;
   render=NULL;
   wglSwapIntervalEXT=NULL;
   setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
   setting.nVersion=1;
   setting.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
   setting.iPixelType=PFD_TYPE_RGBA;
   setting.iLayerType=PFD_MAIN_PLANE;
  }

  WINGL::~WINGL()
  {
   if (render!=NULL)
   {
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(render);
    render=NULL;
   }

  }

  bool WINGL::check_flag(const unsigned long int flag) const
  {
   return (setting.dwFlags&flag)!=0;
  }

  int WINGL::get_pixel_format(HDC target,const unsigned long int color)
  {
   device=target;
   setting.cColorBits=color;
   return ChoosePixelFormat(device,&setting);
  }

  void WINGL::set_pixel_format(const int format)
  {
   if (format==0)
   {
    EUGENEGDK::Halt("Invalid pixel format");
   }
   DescribePixelFormat(device,format,setting.nSize,&setting);
   if (SetPixelFormat(device,format,&setting)==FALSE)
   {
    EUGENEGDK::Halt("Can't set pixel format");
   }

  }

  void WINGL::create_render_context()
  {
   render=wglCreateContext(device);
   if (render==NULL)
   {
    EUGENEGDK::Halt("Can't create render context");
   }
   wglMakeCurrent(device,render);
  }

  void WINGL::disable_vsync()
  {
   wglSwapIntervalEXT=reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
   if (wglSwapIntervalEXT!=NULL)
   {
    wglSwapIntervalEXT(0);
   }

  }

  void WINGL::set_render(HDC target,const unsigned long int color)
  {
   this->set_pixel_format(this->get_pixel_format(target,color));
   this->create_render_context();
   this->disable_vsync();
  }

  void WINGL::Swap()
  {
   SwapBuffers(device);
  }

  bool WINGL::is_software_render() const
  {
   return (this->check_flag(PFD_GENERIC_FORMAT)==true) && (this->check_flag(PFD_GENERIC_ACCELERATED)==false);
  }

 }

 namespace Core
 {

  double get_start_offset(const double current,const double total)
  {
   return (1.0/total)*(current-1.0);
  }

  double get_end_offset(const double current,const double total)
  {
   return (1.0/total)*current;
  }

  Unicode_Convertor::Unicode_Convertor()
  {
   target.set_length(0);
  }

  Unicode_Convertor::~Unicode_Convertor()
  {
   target.destroy_buffer();
  }

  void Unicode_Convertor::convert_string(const char *source)
  {
   size_t index;
   for (index=0;index<target.get_length();++index)
   {
    target[index]=std::btowc(source[index]);
   }

  }

  wchar_t *Unicode_Convertor::convert(const char *source)
  {
   target.set_length(strlen(source)+1);
   target.create_buffer("Can't allocate memory for string buffer");
   target.fill_buffer(0);
   this->convert_string(source);
   return target.get_buffer();
  }

  Resizer::Resizer()
  {
   image.set_length(0);
   size_limit=0;
   source_width=0;
   source_height=0;
   target_width=1;
   target_height=1;
  }

  Resizer::~Resizer()
  {
   image.destroy_buffer();
  }

  size_t Resizer::get_source_offset(const unsigned int x,const unsigned int y) const
  {
   return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(source_width);
  }

  void Resizer::resize_image(const unsigned int *target)
  {
   size_t index;
   unsigned int x,y,x_ratio,y_ratio;
   x=0;
   y=0;
   x_ratio=(source_width*USHRT_MAX)/target_width;
   y_ratio=(source_height*USHRT_MAX)/target_height;
   for (index=0;index<image.get_length();++index)
   {
    image[index]=target[this->get_source_offset((x*x_ratio)/USHRT_MAX,(y*y_ratio)/USHRT_MAX)];
    ++x;
    if (x==target_width)
    {
     x=0;
     ++y;
    }

   }

  }

  void Resizer::set_setting(const unsigned int width,const unsigned int height,const unsigned int limit)
  {
   source_width=width;
   source_height=height;
   size_limit=limit;
  }

  void Resizer::calculate_size()
  {
   while (target_width<source_width)
   {
    target_width*=2;
   }
   while (target_height<source_height)
   {
    target_height*=2;
   }

  }

  void Resizer::correct_size()
  {
   if (target_width>size_limit)
   {
    target_width=size_limit;
   }
   if (target_height>size_limit)
   {
    target_height=size_limit;
   }

  }

  void Resizer::create_texture()
  {
   size_t length;
   length=static_cast<size_t>(target_width)*static_cast<size_t>(target_height);
   image.set_length(length);
   image.create_buffer("Can't allocate memory for texture buffer");
  }

  void Resizer::make_texture(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit)
  {
   this->set_setting(width,height,limit);
   this->calculate_size();
   this->correct_size();
   this->create_texture();
   this->resize_image(target);
  }

  unsigned int Resizer::get_width() const
  {
   return target_width;
  }

  unsigned int Resizer::get_height() const
  {
   return target_height;
  }

  unsigned int *Resizer::get_buffer()
  {
   return image.get_buffer();
  }

  FPS::FPS()
  {
   start=time(NULL);
   current=0;
   fps=0;
  }

  FPS::~FPS()
  {

  }

  void FPS::update_counter()
  {
   ++current;
   if (difftime(time(NULL),start)>=1.0)
   {
    fps=current;
    current=0;
    start=time(NULL);
   }

  }

  unsigned int FPS::get_fps_amount() const
  {
   return fps;
  }

  Shape::Shape()
  {
   target_width=0;
   target_height=0;
   total_width=0;
   total_height=0;
   current_x=0;
   current_y=0;
   vertex[0].x=0;
   vertex[0].y=0;
   vertex[1].x=0;
   vertex[1].y=0;
   vertex[2].x=0;
   vertex[2].y=0;
   vertex[3].x=0;
   vertex[3].y=0;
   point[0].u=0.0;
   point[0].v=1.0;
   point[1].u=1.0;
   point[1].v=1.0;
   point[2].u=1.0;
   point[2].v=0.0;
   point[3].u=0.0;
   point[3].v=0.0;
  }

  Shape::~Shape()
  {

  }

  void Shape::set_data()
  {
   vertex[0].x=current_x;
   vertex[0].y=current_y+target_height;
   vertex[1].x=current_x+target_width;
   vertex[1].y=current_y+target_height;
   vertex[2].x=current_x+target_width;
   vertex[2].y=current_y;
   vertex[3].x=current_x;
   vertex[3].y=current_y;
  }

  unsigned int Shape::get_total_width() const
  {
   return total_width;
  }

  unsigned int Shape::get_total_height() const
  {
   return total_height;
  }

  void Shape::set_total_size(const unsigned int width,const unsigned int height)
  {
   total_width=width;
   total_height=height;
  }

  void Shape::set_size(const unsigned int width,const unsigned int height)
  {
   target_width=width;
   target_height=height;
  }

  void Shape::set_position(const unsigned int x,const unsigned int y)
  {
   current_x=x;
   current_y=y;
  }

  void Shape::set_tile_offset(const double row,const double rows,const double column,const double columns)
  {
   point[0].u=Core::get_start_offset(row,rows);
   point[0].v=Core::get_end_offset(column,columns);
   point[1].u=Core::get_end_offset(row,rows);
   point[1].v=Core::get_end_offset(column,columns);
   point[2].u=Core::get_end_offset(row,rows);
   point[2].v=Core::get_start_offset(column,columns);
   point[3].u=Core::get_start_offset(row,rows);
   point[3].v=Core::get_start_offset(column,columns);
  }

  void Shape::set_horizontal_offset(const double current,const double total)
  {
   this->set_tile_offset(current,total,1.0,1.0);
  }

  void Shape::set_vertical_offset(const double current,const double total)
  {
   this->set_tile_offset(1.0,1.0,current,total);
  }

  Rectangle::Rectangle()
  {
   texture=0;
  }

  Rectangle::~Rectangle()
  {
   if (texture!=0)
   {
    glBindTexture(GL_TEXTURE_2D,0);
    glDeleteTextures(1,&texture);
    texture=0;
   }

  }

  void Rectangle::create_texture(const unsigned int *buffer)
  {
   Resizer resizer;
   resizer.make_texture(buffer,this->get_total_width(),this->get_total_height(),MAXIMUM_TEXTURE_SIZE);
   glPixelStorei(GL_UNPACK_ALIGNMENT,1);
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,resizer.get_width(),resizer.get_height(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,resizer.get_buffer());
  }

  void Rectangle::delete_texture()
  {
   if (texture!=0)
   {
    glBindTexture(GL_TEXTURE_2D,0);
    glDeleteTextures(1,&texture);
    texture=0;
   }

  }

  void Rectangle::check_texture()
  {
   if (glGetError()!=GL_NO_ERROR)
   {
    EUGENEGDK::Halt("Can't create the target texture");
   }

  }

  void Rectangle::load_data()
  {
   glVertexPointer(2,GL_INT,0,vertex);
   glTexCoordPointer(2,GL_DOUBLE,0,point);
  }

  void Rectangle::draw_rectangle()
  {
   glBindTexture(GL_TEXTURE_2D,texture);
   glDrawArrays(GL_TRIANGLE_FAN,0,RECTANGLE_VERTEXES);
  }

  void Rectangle::enable_transparent()
  {
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_BLEND);
  }

  void Rectangle::disable_transparent()
  {
   glDisable(GL_ALPHA_TEST);
   glDisable(GL_BLEND);
  }

  void Rectangle::prepare(const unsigned int *buffer)
  {
   if (buffer!=NULL)
   {
    this->delete_texture();
    this->create_texture(buffer);
    this->check_texture();
   }

  }

 void Rectangle::draw()
 {
  if (texture!=0)
  {
   this->set_data();
   this->load_data();
   this->draw_rectangle();
  }

 }

  void Rectangle::destroy_texture()
  {
   this->delete_texture();
  }

  bool Rectangle::is_texture_exist() const
  {
   return texture!=0;
  }

  Render::Render()
  {

  }

  Render::~Render()
  {

  }

  unsigned int Render::get_maximum_texture_size() const
  {
   int maximum_size;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maximum_size);
   return maximum_size;
  }

  void Render::set_perfomance_setting()
  {
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH);
   glDisable(GL_DITHER);
   glDisable(GL_LOGIC_OP);
   glDisable(GL_FOG);
   glDisable(GL_STENCIL_TEST);
   glDisable(GL_SCISSOR_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_NORMALIZE);
   glDisable(GL_AUTO_NORMAL);
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_GEN_Q);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_1D);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_BLEND);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_EDGE_FLAG_ARRAY);
   glDisableClientState(GL_INDEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
  }

  void Render::set_render_hints()
  {
   glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  }

  void Render::set_common_setting()
  {
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glAlphaFunc(GL_GREATER,0.1f);
   glClearColor(0.0,0.0,0.0,0.0);
  }

 void Render::set_matrix_setting()
 {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
 }

 void Render::set_perspective(const unsigned int width,const unsigned int height)
 {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0,static_cast<double>(width),static_cast<double>(height),0.0,0.0,1.0);
  glViewport(0,0,width,height);
 }

  void Render::create_render(const unsigned int width,const unsigned int height)
  {
   this->set_perfomance_setting();
   this->set_render_hints();
   this->set_common_setting();
   this->set_perspective(width,height);
   this->set_matrix_setting();
   MAXIMUM_TEXTURE_SIZE=this->get_maximum_texture_size();
  }

  void Render::clear_stage()
  {
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }

  void Render::start_render(const unsigned int width,const unsigned int height)
  {
   this->create_render(width,height);
   this->clear_stage();
  }

 }

 namespace Misc
 {

  Multimedia::Multimedia()
   {
    loader=NULL;
    player=NULL;
    controler=NULL;
    video=NULL;
   }

   Multimedia::~Multimedia()
   {
    if (player!=NULL)
   {
    player->Stop();
    player->Release();
    player=NULL;
   }
   if (video!=NULL)
   {
    video->Release();
    video=NULL;
   }
   if (controler!=NULL)
   {
    controler->Release();
    controler=NULL;
   }
   if (loader!=NULL)
   {
    loader->Release();
    loader=NULL;
   }
   CoUninitialize();
  }

  void Multimedia::com_setup()
  {
   if (CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)!=S_OK)
   {
    if (CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)!=S_FALSE)
    {
     EUGENEGDK::Halt("Can't initialize COM");
    }

   }

  }

  void Multimedia::set_screen_mode()
  {
   if (video!=NULL)
   {
    video->put_FullScreenMode(OATRUE);
   }

  }

  void Multimedia::load_content(const wchar_t *target)
  {
   if (loader!=NULL)
   {
    loader->RenderFile(target,NULL);
   }

  }

  void Multimedia::open(const wchar_t *target)
  {
   this->load_content(target);
   this->set_screen_mode();
  }

  bool Multimedia::is_play()
  {
   long long current,total;
   current=0;
   total=0;
   if (controler!=NULL)
   {
    if (controler->GetPositions(&current,&total)!=S_OK)
    {
     current=0;
     total=0;
    }

   }
   return current<total;
  }

  void Multimedia::rewind()
  {
   long long position;
   position=0;
   if (controler!=NULL)
   {
    controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
   }

  }

  void Multimedia::play_content()
  {
   if (player!=NULL)
   {
    player->Run();
   }

  }

  void Multimedia::create_loader()
  {
   if (loader==NULL)
   {
    if (CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,reinterpret_cast<void**>(&loader))!=S_OK)
    {
     loader=NULL;
     EUGENEGDK::Halt("Can't create a multimedia loader");
    }

   }

  }

  void Multimedia::create_player()
  {
   if (player==NULL)
   {
    if (loader->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&player))!=S_OK)
    {
     player=NULL;
     EUGENEGDK::Halt("Can't create a multimedia player");
    }

   }

  }

  void Multimedia::create_controler()
  {
   if (controler==NULL)
   {
    if (loader->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&controler))!=S_OK)
    {
     controler=NULL;
     EUGENEGDK::Halt("Can't create a player controler");
    }

   }

  }

  void Multimedia::create_video_player()
  {
   if (video==NULL)
   {
    if (loader->QueryInterface(IID_IVideoWindow,reinterpret_cast<void**>(&video))!=S_OK)
    {
     video=NULL;
     EUGENEGDK::Halt("Can't create a video player");
    }

   }

  }

  void Multimedia::initialize()
  {
   this->com_setup();
   this->create_loader();
   this->create_player();
   this->create_controler();
   this->create_video_player();
  }

  bool Multimedia::check_playing()
  {
   OAFilterState state;
   bool playing;
   playing=false;
   if (player!=NULL)
   {
    if (player->GetState(INFINITE,&state)!=E_FAIL)
    {
     if (state==State_Running)
     {
      playing=this->is_play();
     }

    }

   }
   return playing;
  }

  void Multimedia::stop()
  {
   if (player!=NULL)
   {
    player->Stop();
   }

  }

  void Multimedia::play()
  {
   this->stop();
   this->rewind();
   this->play_content();
  }

  void Multimedia::play_loop()
  {
   if (this->check_playing()==false)
   {
    this->play();
   }

  }

  void Multimedia::load(const char *target)
  {
   Core::Unicode_Convertor convertor;
   this->stop();
   this->open(convertor.convert(target));
  }

  void Multimedia::initialize(const char *target)
  {
   this->initialize();
   this->load(target);
  }

  Memory::Memory()
  {
   memory.dwLength=sizeof(MEMORYSTATUSEX);
   memory.dwMemoryLoad=0;
   memory.ullAvailExtendedVirtual=0;
   memory.ullAvailPageFile=0;
   memory.ullAvailPhys=0;
   memory.ullAvailVirtual=0;
   memory.ullTotalPageFile=0;
   memory.ullTotalPhys=0;
   memory.ullTotalVirtual=0;
  }

  Memory::~Memory()
  {

  }

  void Memory::get_status()
  {
   if (GlobalMemoryStatusEx(&memory)==FALSE)
   {
    memory.dwLength=sizeof(MEMORYSTATUSEX);
    memory.dwMemoryLoad=0;
    memory.ullAvailExtendedVirtual=0;
    memory.ullAvailPageFile=0;
    memory.ullAvailPhys=0;
    memory.ullAvailVirtual=0;
    memory.ullTotalPageFile=0;
    memory.ullTotalPhys=0;
    memory.ullTotalVirtual=0;
   }

  }

  unsigned long long int Memory::get_total_physical()
  {
   this->get_status();
   return memory.ullTotalPhys;
  }

  unsigned long long int Memory::get_free_physical()
  {
   this->get_status();
   return memory.ullAvailPhys;
  }

  unsigned long long int Memory::get_total_virtual()
  {
   this->get_status();
   return memory.ullTotalVirtual;
  }

  unsigned long long int Memory::get_free_virtual()
  {
   this->get_status();
   return memory.ullAvailVirtual;
  }

  unsigned long int Memory::get_usage()
  {
   this->get_status();
   return memory.dwMemoryLoad;
  }

 }

 namespace Input
 {

  Keyboard::Keyboard()
  {
   preversion.set_length(KEYBOARD);
  }

  Keyboard::~Keyboard()
  {
   preversion.destroy_buffer();
  }

  bool Keyboard::check_state(const unsigned char code,const unsigned char state)
  {
   bool accept;
   accept=false;
   if (preversion.get_buffer()!=NULL)
   {
    accept=(Keys[code]==state) && (preversion[code]!=state);
    preversion[code]=Keys[code];
   }
   return accept;
  }

  void Keyboard::initialize()
  {
   if (preversion.get_buffer()==NULL)
   {
    preversion.create_buffer("Can't allocate memory for keyboard state buffer");
    preversion.fill_buffer(KEY_RELEASE);
   }

  }

  bool Keyboard::check_hold(const unsigned char code)
  {
   if (preversion.get_buffer()!=NULL)
   {
    preversion[code]=Keys[code];
   }
   return Keys[code]==KEY_PRESS;
  }

  bool Keyboard::check_press(const unsigned char code)
  {
   return this->check_state(code,KEY_PRESS);
  }

  bool Keyboard::check_release(const unsigned char code)
  {
   return this->check_state(code,KEY_RELEASE);
  }

  bool Keyboard::is_ready() const
  {
   return preversion.get_length()>0;
  }

  Mouse::Mouse()
  {
   preversion[EUGENEGDK::MOUSE_LEFT]=KEY_RELEASE;
   preversion[EUGENEGDK::MOUSE_RIGHT]=KEY_RELEASE;
   preversion[EUGENEGDK::MOUSE_MIDDLE]=KEY_RELEASE;
   position.x=0;
   position.y=0;
  }

  Mouse::~Mouse()
  {

  }

  void Mouse::get_position()
  {
   if (GetCursorPos(&position)==FALSE)
   {
    position.x=0;
    position.y=0;
   }

  }

  bool Mouse::check_state(const EUGENEGDK::MOUSE_BUTTON button,const unsigned char state)
  {
   bool accept;
   accept=(Buttons[button]==state) && (preversion[button]!=state);
   preversion[button]=Buttons[button];
   return accept;
  }

  void Mouse::show()
  {
   while(ShowCursor(TRUE)<1)
   {
    ;
   }

  }

  void Mouse::hide()
  {
   while(ShowCursor(FALSE)>-1)
   {
    ;
   }

  }

  void Mouse::set_position(const unsigned int x,const unsigned int y)
  {
   if (SetCursorPos(x,y)==FALSE)
   {
    position.x=0;
    position.y=0;
   }

  }

  unsigned int Mouse::get_x()
  {
   this->get_position();
   return position.x;
  }

  unsigned int Mouse::get_y()
  {
   this->get_position();
   return position.y;
  }

  bool Mouse::check_hold(const EUGENEGDK::MOUSE_BUTTON button)
  {
   preversion[button]=Buttons[button];
   return Buttons[button]==KEY_PRESS;
  }

  bool Mouse::check_press(const EUGENEGDK::MOUSE_BUTTON button)
  {
   return this->check_state(button,KEY_PRESS);
  }

  bool Mouse::check_release(const EUGENEGDK::MOUSE_BUTTON button)
  {
   return this->check_state(button,KEY_RELEASE);
  }

  Gamepad::Gamepad()
  {
   active=0;
   memset(&configuration,0,sizeof(JOYCAPS));
   memset(&current,0,sizeof(JOYINFOEX));
   current.dwSize=sizeof(JOYINFOEX);
   current.dwFlags=JOY_RETURNALL;
   current.dwPOV=JOY_POVCENTERED;
   preversion=current;
  }

  Gamepad::~Gamepad()
  {

  }

  bool Gamepad::read_configuration()
  {
   return joyGetDevCaps(static_cast<size_t>(active),&configuration,sizeof(JOYCAPS))==JOYERR_NOERROR;
  }

  bool Gamepad::read_state()
  {
   return joyGetPosEx(active,&current)==JOYERR_NOERROR;
  }

  void Gamepad::clear_state()
  {
   memset(&configuration,0,sizeof(JOYCAPS));
   memset(&current,0,sizeof(JOYINFOEX));
   current.dwSize=sizeof(JOYINFOEX);
   current.dwFlags=JOY_RETURNALL;
   current.dwPOV=JOY_POVCENTERED;
   preversion=current;
  }

  bool Gamepad::check_current_state(const unsigned long int button) const
  {
   return (current.dwButtons&button)!=0;
  }

  bool Gamepad::check_preversion_state(const unsigned long int button) const
  {
   return (preversion.dwButtons&button)!=0;
  }

  unsigned int Gamepad::get_amount()
  {
   return joyGetNumDevs();
  }

  unsigned int Gamepad::get_button_amount()
  {
   unsigned int button_amount;
   button_amount=0;
   if (this->read_configuration()==true)
   {
    button_amount=configuration.wNumButtons;
   }
   return button_amount;
  }

  void Gamepad::update()
  {
   preversion=current;
   if (this->read_state()==false)
   {
    this->clear_state();
   }

  }

  unsigned long int Gamepad::get_sticks_amount()
  {
   unsigned long int sticks_amount;
   sticks_amount=0;
   if (this->read_configuration()==true)
   {
    if (configuration.wNumAxes>1)
    {
     sticks_amount=configuration.wNumAxes/2;
    }

   }
   return sticks_amount;
  }

  void Gamepad::set_active(const unsigned int gamepad)
  {
   if (gamepad<this->get_amount())
   {
    this->clear_state();
    active=gamepad;
   }

  }

  EUGENEGDK::GAMEPAD_DPAD Gamepad::get_dpad() const
  {
   EUGENEGDK::GAMEPAD_DPAD dpad;
   dpad=EUGENEGDK::GAMEPAD_NONE;
   switch (current.dwPOV)
   {
    case JOY_POVFORWARD:
    dpad=EUGENEGDK::GAMEPAD_UP;
    break;
    case JOY_POVBACKWARD:
    dpad=EUGENEGDK::GAMEPAD_DOWN;
    break;
    case JOY_POVLEFT:
    dpad=EUGENEGDK::GAMEPAD_LEFT;
    break;
    case JOY_POVRIGHT:
    dpad=GAMEPAD_RIGHT;
    break;
    case JOYSTICK_UPLEFT:
    dpad=EUGENEGDK::GAMEPAD_UPLEFT;
    break;
    case JOYSTICK_UPRIGHT:
    dpad=EUGENEGDK::GAMEPAD_UPRIGHT;
    break;
    case JOYSTICK_DOWNLEFT:
    dpad=EUGENEGDK::GAMEPAD_DOWNLEFT;
    break;
    case JOYSTICK_DOWNRIGHT:
    dpad=EUGENEGDK::GAMEPAD_DOWNRIGHT;
    break;
    default:
    ;
    break;
   }
   return dpad;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_stick_x(const EUGENEGDK::GAMEPAD_STICKS stick)
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   unsigned long int control;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   if (stick==EUGENEGDK::GAMEPAD_LEFT_STICK)
   {
    if (this->get_sticks_amount()>0)
    {
     control=(configuration.wXmax-configuration.wXmin)/2;
     if (current.dwXpos<control)
     {
      directional=GAMEPAD_NEGATIVE_DIRECTION;
     }
     if (current.dwXpos>control)
     {
      directional=GAMEPAD_POSITIVE_DIRECTION;
     }

    }

   }
   if (stick==EUGENEGDK::GAMEPAD_RIGHT_STICK)
   {
    if (this->get_sticks_amount()>1)
    {
     control=(configuration.wZmax-configuration.wZmin)/2;
     if (current.dwZpos<control)
     {
      directional=GAMEPAD_NEGATIVE_DIRECTION;
     }
     if (current.dwZpos>control)
     {
      directional=GAMEPAD_POSITIVE_DIRECTION;
     }

    }

   }
   return directional;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_stick_y(const EUGENEGDK::GAMEPAD_STICKS stick)
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   unsigned long int control;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   if (stick==EUGENEGDK::GAMEPAD_LEFT_STICK)
   {
    if (this->get_sticks_amount()>0)
    {
     control=(configuration.wYmax-configuration.wYmin)/2;
     if (current.dwYpos<control)
     {
      directional=GAMEPAD_NEGATIVE_DIRECTION;
     }
     if (current.dwYpos>control)
     {
      directional=GAMEPAD_POSITIVE_DIRECTION;
     }

    }

   }
   if (stick==EUGENEGDK::GAMEPAD_RIGHT_STICK)
   {
    if (this->get_sticks_amount()>1)
    {
     control=(configuration.wRmax-configuration.wRmin)/2;
     if (current.dwRpos<control)
     {
      directional=GAMEPAD_NEGATIVE_DIRECTION;
     }
     if (current.dwRpos>control)
     {
      directional=GAMEPAD_POSITIVE_DIRECTION;
     }

    }

   }
   return directional;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_left_stick_x()
  {
   return this->get_stick_x(EUGENEGDK::GAMEPAD_LEFT_STICK);
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_left_stick_y()
  {
   return this->get_stick_y(EUGENEGDK::GAMEPAD_LEFT_STICK);
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_right_stick_x()
  {
   return this->get_stick_x(EUGENEGDK::GAMEPAD_RIGHT_STICK);
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_right_stick_y()
  {
   return this->get_stick_y(EUGENEGDK::GAMEPAD_RIGHT_STICK);
  }

  bool Gamepad::check_hold(const EUGENEGDK::GAMEPAD_BUTTONS button) const
  {
   return this->check_current_state(button);
  }

  bool Gamepad::check_press(const EUGENEGDK::GAMEPAD_BUTTONS button) const
  {
   return (this->check_current_state(button)==true) && (this->check_preversion_state(button)==false);
  }

  bool Gamepad::check_release(const EUGENEGDK::GAMEPAD_BUTTONS button) const
  {
   return (this->check_current_state(button)==false) && (this->check_preversion_state(button)==true);
  }

 }

 namespace File
 {

  Binary_File::Binary_File()
  {
   target=NULL;
  }

  Binary_File::~Binary_File()
  {
   if (target!=NULL)
   {
    fclose(target);
    target=NULL;
   }

  }

  void Binary_File::close()
  {
   if (target!=NULL)
   {
    fclose(target);
    target=NULL;
   }

  }

  void Binary_File::set_position(const long int offset)
  {
   if (target!=NULL)
   {
    fseek(target,offset,SEEK_SET);
   }

  }

  long int Binary_File::get_position()
  {
   long int position;
   position=0;
   if (target!=NULL)
   {
    position=ftell(target);
   }
   return position;
  }

  long int Binary_File::get_length()
  {
   long int length;
   length=0;
   if (target!=NULL)
   {
    fseek(target,0,SEEK_END);
    length=ftell(target);
    rewind(target);
   }
   return length;
  }

  bool Binary_File::check_error()
  {
   int error;
   error=0;
   if (target!=NULL)
   {
    error=ferror(target);
   }
   return error!=0;
  }

  bool Binary_File::is_open() const
  {
   return target!=NULL;
  }

  Input_File::Input_File()
  {

  }

  Input_File::~Input_File()
  {
   if (target!=NULL)
   {
    fclose(target);
    target=NULL;
   }

  }

  void Input_File::open(const char *name)
  {
   this->close();
   target=fopen(name,"rb");
  }

  void Input_File::read(void *buffer,const size_t length)
  {
   if (target!=NULL)
   {
    if (buffer!=NULL)
    {
     fread(buffer,sizeof(char),length,target);
    }

   }

  }

  Output_File::Output_File()
  {

  }

  Output_File::~Output_File()
  {
   if (target!=NULL)
   {
    fclose(target);
    target=NULL;
   }

  }

  void Output_File::open(const char *name)
  {
   this->close();
   target=fopen(name,"wb");
  }

  void Output_File::create_temp()
  {
   this->close();
   target=tmpfile();
  }

  void Output_File::write(const void *buffer,const size_t length)
  {
   if (target!=NULL)
   {
    if (buffer!=NULL)
    {
     fwrite(buffer,sizeof(char),length,target);
    }

   }

  }

  void Output_File::flush()
  {
   if (target!=NULL)
   {
    fflush(target);
   }

  }

 }

 namespace Graphics
 {

  Screen::Screen()
  {
   ready=false;
  }

  Screen::~Screen()
  {

  }

  void Screen::check_video_mode()
  {
   Internal::Display::get_video_mode();
   Internal::Display::correct_depth();
   Internal::Display::set_video_mode();
  }

  void Screen::set_resolution(const unsigned long int width,const unsigned long int height)
  {
   Internal::Display::get_video_mode();
   Internal::Display::correct_depth();
   Internal::Display::set_setting(width,height);
   Internal::Display::set_video_mode();
  }

  void Screen::screen_setup()
  {
   Internal::Engine::prepare_engine();
   Internal::WINGL::set_render(Internal::Engine::get_context(),Internal::Display::get_depth());
   Core::Render::start_render(Internal::Display::get_display_width(),Internal::Display::get_display_height());
   Internal::Synchronization::create_timer();
   Internal::Synchronization::set_timer(17);
  }

  void Screen::clear_screen()
  {
   if (ready==true)
   {
    Core::Render::clear_stage();
   }

  }

  void Screen::initialize()
  {
   if (ready==false)
   {
    this->check_video_mode();
    this->screen_setup();
    ready=true;
   }

  }

  void Screen::initialize(const unsigned int width,const unsigned int height)
  {
   if (ready==false)
   {
    this->set_resolution(width,height);
    this->screen_setup();
    ready=true;
   }

  }

  bool Screen::update()
  {
   if (ready==true)
   {
    Internal::WINGL::Swap();
    Core::FPS::update_counter();
    Core::Render::clear_stage();
   }
   return Internal::Engine::process_message();
  }

  bool Screen::sync()
  {
   Internal::Synchronization::wait_timer();
   return this->update();
  }

  bool Screen::is_ready() const
  {
   return ready;
  }

  bool Screen::is_accelerated() const
  {
   return Internal::WINGL::is_software_render()==false;
  }

  unsigned long int Screen::get_color() const
  {
   return Internal::Display::get_depth();
  }

  unsigned int Screen::get_fps() const
  {
   return Core::FPS::get_fps_amount();
  }

  unsigned int Screen::get_width() const
  {
   return Internal::Display::get_display_width();
  }

  unsigned int Screen::get_height() const
  {
   return Internal::Display::get_display_height();
  }

  Screen* Screen::get_handle()
  {
   return this;
  }

  Image::Image()
  {
   data.set_length(0);
   width=0;
   height=0;
  }

  Image::~Image()
  {
   data.destroy_buffer();
   width=0;
   height=0;
  }

  void Image::uncompress_tga_data(const unsigned char *target)
  {
   size_t index,position,amount;
   index=0;
   position=0;
   while (index<data.get_length())
   {
    if (target[position]<128)
    {
     amount=target[position]+1;
     amount*=sizeof(unsigned int);
     memcpy(data.get_buffer()+index,target+(position+1),amount);
     index+=amount;
     position+=1+amount;
    }
    else
    {
     for (amount=target[position]-127;amount>0;--amount)
     {
      memcpy(data.get_buffer()+index,target+(position+1),sizeof(unsigned int));
      index+=sizeof(unsigned int);
     }
     position+=1+sizeof(unsigned int);
    }

   }

  }

  void Image::load_tga(File::Input_File &target)
  {
   Core::Buffer<unsigned char> compressed_buffer;
   size_t compressed_length,uncompressed_length;
   TGA_head head;
   TGA_map color_map;
   TGA_image image;
   compressed_length=static_cast<size_t>(target.get_length()-18);
   target.read(&head,3);
   target.read(&color_map,5);
   target.read(&image,10);
   width=image.width;
   height=image.height;
   uncompressed_length=static_cast<size_t>(width)*static_cast<size_t>(height)*sizeof(unsigned int);
   if (image.color==IMAGE_COLOR)
   {
    data.set_length(uncompressed_length);
    data.create_buffer("Can't allocate memory for uncompressed image buffer");
    switch (head.type)
    {
     case 2:
     target.read(data.get_buffer(),data.get_length());
     break;
     case 10:
     compressed_buffer.set_length(compressed_length);
     compressed_buffer.create_buffer("Can't allocate memory for compressed image buffer");
     target.read(compressed_buffer.get_buffer(),compressed_buffer.get_length());
     this->uncompress_tga_data(compressed_buffer.get_buffer());
     compressed_buffer.destroy_buffer();
     break;
     default:
     width=0;
     height=0;
     data.destroy_buffer();
     break;
    }

   }
   else
   {
    width=0;
    height=0;
   }

  }

  unsigned int Image::get_width() const
  {
   return width;
  }

  unsigned int Image::get_height() const
  {
   return height;
  }

  size_t Image::get_length() const
  {
   return data.get_length();
  }

  unsigned char *Image::get_data()
  {
   return data.get_buffer();
  }

  Image* Image::get_handle()
  {
   return this;
  }

  void Image::destroy_image()
  {
   data.destroy_buffer();
   width=0;
   height=0;
  }

  void Image::load_tga(const char *name)
  {
   File::Input_File target;
   this->destroy_image();
   target.open(name);
   if (target.is_open()==true)
   {
    this->load_tga(target);
    target.close();
   }

  }

  Picture::Picture()
  {
   image.set_length(0);
   image_width=0;
   image_height=0;
  }

  Picture::~Picture()
  {
   image.destroy_buffer();
   image_width=0;
   image_height=0;
  }

  void Picture::set_image_size(const unsigned int width,const unsigned int height)
  {
   image_width=width;
   image_height=height;
  }

  void Picture::create_storage()
  {
   size_t length;
   length=static_cast<size_t>(image_width)*static_cast<size_t>(image_height);
   image.set_length(length);
   image.create_buffer("Can't allocate memory for image storage");
  }

  void Picture::copy_image(const unsigned int *target)
  {
   size_t index;
   for (index=0;index<image.get_length();++index)
   {
    image[index]=target[index];
   }

  }

  void Picture::load_image(Image *buffer)
  {
   this->destroy_image();
   if (buffer!=NULL)
   {
    if (buffer->get_length()>0)
    {
     this->set_image_size(buffer->get_width(),buffer->get_height());
     this->create_storage();
     memcpy(image.get_buffer(),buffer->get_data(),buffer->get_length());
    }

   }

  }

  unsigned int *Picture::get_image()
  {
   return image.get_buffer();
  }

  void Picture::destroy_image()
  {
   image.destroy_buffer();
   this->set_image_size(0,0);
  }

  bool Picture::is_storage_empty() const
  {
   return image.get_length()==0;
  }

  unsigned int Picture::get_image_width() const
  {
   return image_width;
  }

  unsigned int Picture::get_image_height() const
  {
   return image_height;
  }

  Animation::Animation()
  {
   frame=1;
   frames=1;
  }

  Animation::~Animation()
  {

  }

  void Animation::correct_frame()
  {
   if (frame>frames)
   {
    frame=1;
   }

  }

  void Animation::reset_animation_setting()
  {
   frame=1;
   frames=1;
  }

  void Animation::increase_frame()
  {
   ++frame;
   this->correct_frame();
  }

  void Animation::set_frame(const unsigned int target)
  {
   if (target>0)
   {
    frame=target;
   }
   this->correct_frame();
  }

  void Animation::set_frames(const unsigned int amount)
  {
   if (amount>0)
   {
    frames=amount;
   }

  }

  unsigned int Animation::get_frames() const
  {
   return frames;
  }

  unsigned int Animation::get_frame() const
  {
   return frame;
  }

  Billboard::Billboard()
  {
   billboard.set_size(0,0);
   transparent=true;
   current_x=0;
   current_y=0;
   sprite_width=0;
   sprite_height=0;
  }

  Billboard::~Billboard()
  {
   billboard.destroy_texture();
  }

  void Billboard::check_transparent()
  {
   if (transparent==true)
   {
    billboard.enable_transparent();
   }
   else
   {
    billboard.disable_transparent();
   }

  }

  void Billboard::draw_sprite_image()
  {
   billboard.set_size(sprite_width,sprite_height);
   billboard.set_position(current_x,current_y);
   billboard.draw();
  }

  void Billboard::reset_billboard_setting()
  {
   transparent=true;
   current_x=0;
   current_y=0;
   sprite_width=0;
   sprite_height=0;
  }

  void Billboard::prepare(const unsigned int width,const unsigned int height,const unsigned int *picture)
  {
   billboard.set_total_size(width,height);
   billboard.prepare(picture);
  }

  void Billboard::set_transparent(const bool enabled)
  {
   transparent=enabled;
  }

  bool Billboard::get_transparent() const
  {
   return transparent;
  }

  void Billboard::set_width(const unsigned int width)
  {
   if (billboard.is_texture_exist()==true)
   {
    if (width>0)
    {
     sprite_width=width;
    }

   }

  }

  void Billboard::set_height(const unsigned int height)
  {
   if (billboard.is_texture_exist()==true)
   {
    if (height>0)
    {
     sprite_height=height;
    }

   }

  }

  void Billboard::set_size(const unsigned int width,const unsigned int height)
  {
   this->set_width(width);
   this->set_height(height);
  }

  void Billboard::set_position(const unsigned int x,const unsigned int y)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x=x;
    current_y=y;
   }

  }

  void Billboard::set_x(const unsigned int x)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x=x;
   }

  }

  void Billboard::set_y(const unsigned int y)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y=y;
   }

  }

  void Billboard::increase_x()
  {
   if (billboard.is_texture_exist()==true)
   {
    ++current_x;
   }

  }

  void Billboard::decrease_x()
  {
   if (billboard.is_texture_exist()==true)
   {
    --current_x;
   }

  }

  void Billboard::increase_y()
  {
   if (billboard.is_texture_exist()==true)
   {
    ++current_y;
   }

  }

  void Billboard::decrease_y()
  {
   if (billboard.is_texture_exist()==true)
   {
    --current_y;
   }

  }

  void Billboard::increase_x(const unsigned int increment)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x+=increment;
   }

  }

  void Billboard::decrease_x(const unsigned int decrement)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x-=decrement;
   }

  }

  void Billboard::increase_y(const unsigned int increment)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y+=increment;
   }

  }

  void Billboard::decrease_y(const unsigned int decrement)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y-=decrement;
   }

  }

  unsigned int Billboard::get_x() const
  {
   return current_x;
  }

  unsigned int Billboard::get_y() const
  {
   return current_y;
  }

  unsigned int Billboard::get_width() const
  {
   return sprite_width;
  }

  unsigned int Billboard::get_height() const
  {
   return sprite_height;
  }

  EUGENEGDK::BOX Billboard::get_box() const
  {
   EUGENEGDK::BOX collision;
   collision.x=current_x;
   collision.y=current_y;
   collision.width=sprite_width;
   collision.height=sprite_height;
   return collision;
  }

  void Billboard::draw_sprite()
  {
   this->check_transparent();
   this->draw_sprite_image();
  }

  void Billboard::draw_sprite(const unsigned int x,const unsigned int y)
  {
   this->set_position(x,y);
   this->draw_sprite();
  }

  void Billboard::draw_sprite(const bool transparency)
  {
   this->set_transparent(transparency);
   this->draw_sprite();
  }

  void Billboard::draw_sprite(const bool transparency,const unsigned int x,const unsigned int y)
  {
   this->set_transparent(transparency);
   this->draw_sprite(x,y);
  }

  Sprite::Sprite()
  {
   current_kind=EUGENEGDK::SINGLE_SPRITE;
  }

  Sprite::~Sprite()
  {

  }

  void Sprite::reset_sprite_setting()
  {
   current_kind=EUGENEGDK::SINGLE_SPRITE;
  }

  void Sprite::set_sprite_setting()
  {
   switch (current_kind)
   {
    case EUGENEGDK::HORIZONTAL_STRIP:
    this->set_size(this->get_image_width()/this->get_frames(),this->get_image_height());
    break;
    case EUGENEGDK::VERTICAL_STRIP:
    this->set_size(this->get_image_width(),this->get_image_height()/this->get_frames());
    break;
    default:
    this->set_size(this->get_image_width(),this->get_image_height());
    break;
   }

  }

  void Sprite::configure_sprite()
  {
   if (this->is_storage_empty()==false)
   {
    this->set_sprite_setting();
   }

  }

  void Sprite::set_sprite_frame()
  {
   switch(current_kind)
   {
    case EUGENEGDK::HORIZONTAL_STRIP:
    billboard.set_horizontal_offset(static_cast<double>(this->get_frame()),static_cast<double>(this->get_frames()));
    break;
    case EUGENEGDK::VERTICAL_STRIP:
    billboard.set_vertical_offset(static_cast<double>(this->get_frame()),static_cast<double>(this->get_frames()));
    break;
    default:
    billboard.set_horizontal_offset(1.0,1.0);
    break;
   }

  }

  void Sprite::set_kind(const EUGENEGDK::SPRITE_TYPE kind)
  {
   current_kind=kind;
   this->configure_sprite();
   this->set_sprite_frame();
  }

  Sprite* Sprite::get_handle()
  {
   return this;
  }

  EUGENEGDK::SPRITE_TYPE Sprite::get_kind() const
  {
   return current_kind;
  }

  void Sprite::set_setting(const EUGENEGDK::SPRITE_TYPE kind,const unsigned int frames)
  {
   this->reset_animation_setting();
   if (kind!=EUGENEGDK::SINGLE_SPRITE)
   {
    this->set_frames(frames);
   }
   this->set_kind(kind);
  }

  void Sprite::load_sprite(Image *buffer,const EUGENEGDK::SPRITE_TYPE kind,const unsigned int frames)
  {
   this->load_image(buffer);
   if (this->is_storage_empty()==false)
   {
    this->reset_animation_setting();
    this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
    this->set_setting(kind,frames);
   }

  }

  void Sprite::load_sprite(Image *buffer)
  {
   this->load_sprite(buffer,EUGENEGDK::SINGLE_SPRITE,1);
  }

  void Sprite::load_sprite(Image &buffer,const EUGENEGDK::SPRITE_TYPE kind,const unsigned int frames)
  {
   this->load_sprite(buffer.get_handle(),kind,frames);
  }

  void Sprite::load_sprite(Image &buffer)
  {
   this->load_sprite(buffer.get_handle());
  }

  void Sprite::set_target(const unsigned int target)
  {
   this->set_frame(target);
   this->set_sprite_frame();
  }

  void Sprite::step()
  {
   this->increase_frame();
   this->set_sprite_frame();
  }

  void Sprite::clone(Sprite *target)
  {
   if (target->is_storage_empty()==false)
   {
    this->destroy_sprite();
    this->set_image_size(target->get_image_width(),target->get_image_height());
    this->create_storage();
    this->set_setting(target->get_kind(),target->get_frames());
    this->set_transparent(target->get_transparent());
    this->copy_image(target->get_image());
    this->set_size(target->get_width(),target->get_height());
    this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
   }

  }

  void Sprite::clone(Sprite &target)
  {
   this->clone(target.get_handle());
  }

  void Sprite::destroy_sprite()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
   this->reset_animation_setting();
   this->reset_sprite_setting();
  }

  Sheet::Sheet()
  {
   rows=0;
   columns=0;
  }

  Sheet::~Sheet()
  {

  }

  void Sheet::reset_sheet_setting()
  {
   rows=0;
   columns=0;
  }

  void Sheet::prepare_sheet()
  {
   this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
   this->set_size(this->get_image_width()/rows,this->get_image_height()/columns);
  }

  unsigned int Sheet::get_row() const
  {
   unsigned int row;
   row=this->get_frame()%rows;
   if (row==0)
   {
    row=rows;
   }
   return row;
  }

  unsigned int Sheet::get_column() const
  {
   unsigned int column;
   column=1;
   if (this->get_frame()>rows)
   {
    column+=(this->get_frame()-1)/rows;
   }
   return column;
  }

  unsigned int Sheet::get_rows() const
  {
   return rows;
  }

  unsigned int Sheet::get_columns() const
  {
   return columns;
  }

  void Sheet::destroy_sheet()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
   this->reset_animation_setting();
   this->reset_sheet_setting();
  }

  void Sheet::select(const unsigned int row,const unsigned int column)
  {
   if (row>0)
   {
    if (column>0)
    {
     billboard.set_tile_offset(static_cast<double>(row),static_cast<double>(rows),static_cast<double>(column),static_cast<double>(columns));
    }

   }

  }

  void Sheet::select(const unsigned int target)
  {
   this->set_frame(target);
   this->select(this->get_row(),this->get_column());
  }

  void Sheet::step()
  {
   this->increase_frame();
   this->select(this->get_row(),this->get_column());
  }

  void Sheet::load_sheet(Image *sheet,const unsigned int row_amount,const unsigned int column_amount)
  {
   if (row_amount>0)
   {
    if (column_amount>0)
    {
     this->load_image(sheet);
     if (this->is_storage_empty()==false)
     {
      rows=row_amount;
      columns=column_amount;
      this->reset_animation_setting();
      this->set_frames(rows*columns);
      this->select(1);
      this->prepare_sheet();
     }

    }

   }

  }

  void Sheet::load_sheet(Image &sheet,const unsigned int row_amount,const unsigned int column_amount)
  {
   this->load_sheet(sheet.get_handle(),row_amount,column_amount);
  }

  Background::Background()
  {
   stage.set_position(0,0);
  }

  Background::~Background()
  {
   stage.destroy_sprite();
  }

  void Background::prepare(const Screen *screen)
  {
   if (screen!=NULL)
   {
    stage.set_size(screen->get_width(),screen->get_height());
   }

  }

  void Background::prepare(const unsigned int width,const unsigned int height)
  {
   stage.set_size(width,height);
  }

  void Background::prepare(Screen &screen)
  {
   this->prepare(screen.get_handle());
  }

  void Background::set_setting(const EUGENEGDK::BACKGROUND_TYPE kind,const unsigned int frames)
  {
   switch (kind)
   {
    case EUGENEGDK::HORIZONTAL_BACKGROUND:
    stage.set_setting(EUGENEGDK::HORIZONTAL_STRIP,frames);
    break;
    case EUGENEGDK::VERTICAL_BACKGROUND:
    stage.set_setting(EUGENEGDK::VERTICAL_STRIP,frames);
    break;
    default:
    stage.set_setting(EUGENEGDK::SINGLE_SPRITE,1);
    break;
   }

  }

  void Background::load_background(Image *background,const EUGENEGDK::BACKGROUND_TYPE kind,const unsigned int frames)
  {
   switch (kind)
   {
    case EUGENEGDK::HORIZONTAL_BACKGROUND:
    stage.load_sprite(background,EUGENEGDK::HORIZONTAL_STRIP,frames);
    break;
    case EUGENEGDK::VERTICAL_BACKGROUND:
    stage.load_sprite(background,EUGENEGDK::VERTICAL_STRIP,frames);
    break;
    default:
    stage.load_sprite(background);
    break;
   }

  }

  void Background::load_background(Image *background)
  {
   this->load_background(background,EUGENEGDK::NORMAL_BACKGROUND,1);
  }

  void Background::load_background(Image &background,const EUGENEGDK::BACKGROUND_TYPE kind,const unsigned int frames)
  {
   this->load_background(background.get_handle(),kind,frames);
  }

  void Background::load_background(Image &background)
  {
   this->load_background(background.get_handle());
  }

  void Background::set_target(const unsigned int target)
  {
   stage.set_target(target);
  }

  void Background::step()
  {
   stage.step();
  }

  void Background::draw_background()
  {
   stage.draw_sprite(false);
  }

  void Background::destroy_image()
  {
   stage.destroy_image();
  }

  void Background::destroy_background()
  {
   stage.destroy_sprite();
  }

  unsigned int Background::get_frame() const
  {
   return stage.get_frame();
  }

  unsigned int Background::get_frames() const
  {
   return stage.get_frames();
  }

  unsigned int Background::get_width() const
  {
   return stage.get_width();
  }

  unsigned int Background::get_height() const
  {
   return stage.get_height();
  }

  EUGENEGDK::BACKGROUND_TYPE Background::get_kind() const
  {
   BACKGROUND_TYPE kind;
   kind=EUGENEGDK::NORMAL_BACKGROUND;
   if (stage.get_kind()==EUGENEGDK::HORIZONTAL_STRIP)
   {
    kind=EUGENEGDK::HORIZONTAL_BACKGROUND;
   }
   if (stage.get_kind()==EUGENEGDK::VERTICAL_STRIP)
   {
    kind=EUGENEGDK::VERTICAL_BACKGROUND;
   }
   return kind;
  }

  Text::Text()
  {
   text.set_size(0,0);
   current_x=0;
   current_y=0;
  }

  Text::~Text()
  {
   text.destroy_sheet();
  }

  void Text::increase_position()
  {
   text.increase_x(text.get_width());
  }

  void Text::restore_position()
  {
   text.set_position(current_x,current_y);
  }

  unsigned int Text::get_font_width() const
  {
   return text.get_width();
  }

  unsigned int Text::get_font_height() const
  {
   return text.get_height();
  }

  void Text::set_position(const unsigned int x,const unsigned int y)
  {
   current_x=x;
   current_y=y;
   text.set_position(current_x,current_y);
  }

  void Text::set_size(const unsigned int width,const unsigned int height)
  {
   text.set_size(width,height);
  }

  void Text::load_font(Image *font)
  {
   text.load_sheet(font,16,16);
  }

  void Text::load_font(Image &font)
  {
   this->load_font(font.get_handle());
  }

  void Text::draw_character(const char target)
  {
   text.select(static_cast<unsigned char>(target)+1);
   text.draw_sprite(true);
  }

  void Text::draw_text(const char *target)
  {
   size_t index,length;
   length=strlen(target);
   this->restore_position();
   for (index=0;index<length;++index)
   {
    this->draw_character(target[index]);
    this->increase_position();
   }

  }

  void Text::draw_character(const unsigned int x,const unsigned int y,const char target)
  {
   this->set_position(x,y);
   this->draw_character(target);
  }

  void Text::draw_text(const unsigned int x,const unsigned int y,const char *target)
  {
   this->set_position(x,y);
   this->draw_text(target);
  }

  void Text::destroy_image()
  {
   text.destroy_image();
  }

  void Text::destroy_font()
  {
   text.destroy_sheet();
  }

 }

 namespace Common
 {

  Timer::Timer()
  {
   start=time(NULL);
   interval=0.0;
  }

  Timer::~Timer()
  {

  }

  void Timer::set_timer(const double seconds)
  {
   interval=seconds;
   start=time(NULL);
  }

  bool Timer::check_timer()
  {
   bool check;
   check=difftime(time(NULL),start)>=interval;
   if (check==true)
   {
    start=time(NULL);
   }
   return check;
  }

  Collision::Collision()
  {
   first.x=0;
   first.y=0;
   first.width=0;
   first.height=0;
   second=first;
  }

  Collision::~Collision()
  {

  }

  bool Collision::check_horizontal_collision() const
  {
   bool collision;
   collision=false;
   if ((first.x+first.width)>=second.x)
   {
    collision=first.x<=(second.x+second.width);
   }
   return collision;
  }

  bool Collision::check_vertical_collision() const
  {
   bool collision;
   collision=false;
   if ((first.y+first.height)>=second.y)
   {
    collision=first.y<=(second.y+second.height);
   }
   return collision;
  }

  void Collision::set_target(const EUGENEGDK::BOX &first_target,const EUGENEGDK::BOX &second_target)
  {
   first=first_target;
   second=second_target;
  }

  bool Collision::check_collision() const
  {
   return this->check_horizontal_collision() && this->check_vertical_collision();
  }

  bool Collision::check_collision(const EUGENEGDK::BOX &first_target,const EUGENEGDK::BOX &second_target)
  {
   this->set_target(first_target,second_target);
   return this->check_collision();
  }

  EUGENEGDK::BOX Collision::generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height) const
  {
   EUGENEGDK::BOX collision;
   collision.x=x;
   collision.y=y;
   collision.width=width;
   collision.height=height;
   return collision;
  }

 }

 namespace Filesystem
 {

  bool delete_file(const char *name)
  {
   return remove(name)==0;
  }

  bool file_exist(const char *name)
  {
   FILE *target;
   bool exist;
   exist=false;
   target=fopen(name,"rb");
   if (target!=NULL)
   {
    exist=true;
    fclose(target);
   }
   return exist;
  }

 }

 namespace Tools
 {

  void quit()
  {
   exit(EXIT_SUCCESS);
  }

  bool enable_logging(const char *name)
  {
   return freopen(name,"wt",stdout)!=NULL;
  }

  void randomize()
  {
   srand(clock()/CLOCKS_PER_SEC);
  }

  unsigned int get_random(const unsigned int number)
  {
   return rand()%(number+1);
  }

  unsigned int get_texture_size()
  {
   return MAXIMUM_TEXTURE_SIZE;
  }

 }

}