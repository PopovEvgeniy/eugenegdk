/*
Eugene game development kit was made by Popov Evgeniy Alekseyevich
Some code taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Eugene game development kit license

Copyright (C) 2021 - 2025 Popov Evgeniy Alekseyevich

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
  unsigned char descriptor:8;
 } TGA_image;

 unsigned int MAXIMUM_TEXTURE_SIZE=0;
 const size_t KEYBOARD=256;
 const size_t MOUSE=3;
 const unsigned char KEY_RELEASE=0;
 const unsigned char KEY_PRESS=1;

 unsigned char Keys[KEYBOARD]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};
 unsigned char Buttons[MOUSE]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};
 bool run=false;
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
    case WM_CREATE:
    run=true;
    break;
    case WM_CLOSE:
    run=false;
    break;
    case WM_PAINT:
    ValidateRect(window,NULL);
    break;
    case WM_DESTROY:
    PostQuitMessage(0);
    break;
    case WM_ACTIVATE:
    if (LOWORD(wParam)!=WA_INACTIVE) SetFocus(window);
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
   event=NULL;
   timer=0;
  }

  Synchronization::~Synchronization()
  {
   if (timer!=0)
   {
    timeKillEvent(timer);
    timer=0;
   }
   if (event!=NULL)
   {
    CloseHandle(event);
    event=NULL;
   }

  }

  void Synchronization::create_event()
  {
   event=CreateEvent(NULL,TRUE,FALSE,NULL);
   if (event==NULL)
   {
    EUGENEGDK::Halt("Can't create synchronization event");
   }

  }

  void Synchronization::timer_setup(const unsigned int delay)
  {
   timer=timeSetEvent(delay,0,reinterpret_cast<LPTIMECALLBACK>(event),0,TIME_PERIODIC|TIME_CALLBACK_EVENT_SET);
   if (timer==0)
   {
    EUGENEGDK::Halt("Can't set timer setting");
   }

  }

  void Synchronization::create_timer(const unsigned int delay)
  {
   this->create_event();
   this->timer_setup(delay);
  }

  void Synchronization::wait_timer()
  {
   if (event!=NULL)
   {
    WaitForSingleObjectEx(event,INFINITE,FALSE);
    ResetEvent(event);
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

  void Display::set_setting(const unsigned int width,const unsigned int height)
  {
   display.dmPelsWidth=width;
   display.dmPelsHeight=height;
  }

  unsigned int Display::get_depth() const
  {
   return display.dmBitsPerPel;
  }

  unsigned int Display::get_display_width() const
  {
   return display.dmPelsWidth;
  }

  unsigned int Display::get_display_height() const
  {
   return display.dmPelsHeight;
  }

  Engine::Engine()
  {
   memset(&window_class,0,sizeof(WNDCLASSEX));
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
    DestroyWindow(window);
    window=NULL;
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
   UpdateWindow(window);
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
   MSG Message;
   while (PeekMessage(&Message,window,0,0,PM_NOREMOVE)==TRUE)
   {
    if (GetMessage(&Message,window,0,0)==TRUE)
    {
     TranslateMessage(&Message);
     DispatchMessage(&Message);
    }

   }
   return run;
  }

  void Engine::Swap()
  {
   SwapBuffers(context);
  }

  WINGL::WINGL()
  {
   render=NULL;
   wglSwapIntervalEXT=NULL;
   setting.bReserved=0;
   setting.cAccumAlphaBits=0;
   setting.cAccumBits=0;
   setting.cAccumBlueBits=0;
   setting.cAccumGreenBits=0;
   setting.cAccumRedBits=0;
   setting.cAlphaBits=0;
   setting.cAlphaShift=0;
   setting.cAuxBuffers=0;
   setting.cBlueBits=0;
   setting.cBlueShift=0;
   setting.cGreenBits=0;
   setting.cGreenShift=0;
   setting.cRedBits=0;
   setting.cRedShift=0;
   setting.cStencilBits=0;
   setting.dwDamageMask=0;
   setting.dwLayerMask=0;
   setting.dwVisibleMask=0;
   setting.cColorBits=24;
   setting.cDepthBits=16;
   setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
   setting.nVersion=1;
   setting.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_STEREO_DONTCARE;
   setting.iPixelType=PFD_TYPE_RGBA;
   setting.iLayerType=PFD_MAIN_PLANE;
  }

  WINGL::~WINGL()
  {
   if (render!=NULL)
   {
    wglMakeCurrent(wglGetCurrentDC(),NULL);
    wglDeleteContext(render);
    render=NULL;
   }

  }

  void WINGL::set_pixel_format(HDC device)
  {
   int format;
   format=ChoosePixelFormat(device,&setting);
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

  void WINGL::create_render_context(HDC device)
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

  void WINGL::set_render(HDC device)
  {
   this->set_pixel_format(device);
   this->create_render_context(device);
   this->disable_vsync();
  }

  bool WINGL::is_software_render() const
  {
   return ((setting.dwFlags&PFD_GENERIC_FORMAT)!=0) && ((setting.dwFlags&PFD_GENERIC_ACCELERATED)==0);
  }

 }

 namespace Core
 {

  float get_start_offset(const float current,const float total)
  {
   return (1.0f/total)*(current-1.0f);
  }

  float get_end_offset(const float current,const float total)
  {
   return (1.0f/total)*current;
  }

  unsigned int get_pixel_component(const unsigned int pixel,const Core::PIXEL_COMPONENT component)
  {
   return (pixel >> component) & 0xFF;
  }

  unsigned int make_pixel(const unsigned int red,const unsigned int green,const unsigned int blue,const unsigned int alpha)
  {
   return (alpha << 24)|(blue << 16)|(green << 8)|red;
  }

  size_t get_offset(const unsigned int x,const unsigned int y,const unsigned int width)
  {
   return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(width);
  }

  EUGENEGDK::GAMEPAD_DIRECTION get_horizontal_direction(const unsigned int current,const unsigned int maximum,const unsigned int minimum)
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   unsigned int center,dead;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   center=(maximum-minimum)/2;
   dead=maximum/10;
   if (current>(center+dead))
   {
    directional=EUGENEGDK::GAMEPAD_POSITIVE_DIRECTION;
   }
   if (current<(center-dead))
   {
    directional=EUGENEGDK::GAMEPAD_NEGATIVE_DIRECTION;
   }
   return directional;
  }

 EUGENEGDK::GAMEPAD_DIRECTION get_inverted_direction(const EUGENEGDK::GAMEPAD_DIRECTION target)
 {
  EUGENEGDK::GAMEPAD_DIRECTION directional;
  switch (target)
  {
   case EUGENEGDK::GAMEPAD_POSITIVE_DIRECTION:
   directional=EUGENEGDK::GAMEPAD_NEGATIVE_DIRECTION;
   break;
   case EUGENEGDK::GAMEPAD_NEGATIVE_DIRECTION:
   directional=EUGENEGDK::GAMEPAD_POSITIVE_DIRECTION;
   break;
   default:
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   break;
  }
  return directional;
 }

 EUGENEGDK::GAMEPAD_DIRECTION get_vertical_direction(const unsigned int current,const unsigned int maximum,const unsigned int minimum)
 {
  return Core::get_inverted_direction(Core::get_horizontal_direction(current,maximum,minimum));
 }

  Unicode_Converter::Unicode_Converter()
  {
   target=NULL;
  }

  Unicode_Converter::~Unicode_Converter()
  {
   Resource::destroy_array(target);
   target=NULL;
  }

  wchar_t *Unicode_Converter::convert(const char *source)
  {
   size_t length;
   length=strlen(source);
   Resource::create(&target,length+1);
   memset(target,0,(length+1)*sizeof(wchar_t));
   MultiByteToWideChar(CP_ACP,0,source,-1,target,static_cast<int>(length));
   return target;
  }

  Resizer::Resizer()
  {
   image=NULL;
   source_width=0;
   source_height=0;
   x_ratio=0;
   y_ratio=0;
   target_width=1;
   target_height=1;
   normalization=UCHAR_MAX*UCHAR_MAX;
  }

  Resizer::~Resizer()
  {
   Resource::destroy_array(image);
   image=NULL;
  }

  unsigned int Resizer::get_x_difference(const unsigned int x) const
  {
   return (x*x_ratio+1)%UCHAR_MAX;
  }

  unsigned int Resizer::get_y_difference(const unsigned int y) const
  {
   return (y*y_ratio+1)%UCHAR_MAX;
  }

  unsigned int Resizer::get_source_x(const unsigned int x) const
  {
   return (x*x_ratio+1)/UCHAR_MAX;
  }

  unsigned int Resizer::get_source_y(const unsigned int y) const
  {
   return (y*y_ratio+1)/UCHAR_MAX;
  }

  unsigned int Resizer::get_next_x(const unsigned int x) const
  {
   unsigned int next_x;
   next_x=x+1;
   if (next_x==source_width)
   {
    --next_x;
   }
   return next_x;
  }

  unsigned int Resizer::get_next_y(const unsigned int y) const
  {
   unsigned int next_y;
   next_y=y+1;
   if (next_y==source_height)
   {
    --next_y;
   }
   return next_y;
  }

  void Resizer::scale_image(const unsigned int *target)
  {
   size_t index;
   unsigned int x,y,source_x,source_y,next_x,next_y,first,second,third,last,red,green,blue,alpha,x_difference,y_difference,x_weigh,y_weigh;
   index=0;
   for (y=0;y<target_height;++y)
   {
    source_y=this->get_source_y(y);
    next_y=this->get_next_y(source_y);
    y_difference=this->get_y_difference(y);
    y_weigh=UCHAR_MAX-y_difference;
    for (x=0;x<target_width;++x)
    {
     source_x=this->get_source_x(x);
     next_x=this->get_next_x(source_x);
     x_difference=this->get_x_difference(x);
     x_weigh=UCHAR_MAX-x_difference;
     first=target[Core::get_offset(source_x,source_y,source_width)];
     second=target[Core::get_offset(next_x,source_y,source_width)];
     third=target[Core::get_offset(source_x,next_y,source_width)];
     last=target[Core::get_offset(next_x,next_y,source_width)];
     red=(get_pixel_component(first,Core::RED_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::RED_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::RED_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::RED_COMPONENT)*x_difference*y_difference)/normalization;
     green=(get_pixel_component(first,Core::GREEN_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::GREEN_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::GREEN_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::GREEN_COMPONENT)*x_difference*y_difference)/normalization;
     blue=(get_pixel_component(first,Core::BLUE_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::BLUE_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::BLUE_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::BLUE_COMPONENT)*x_difference*y_difference)/normalization;
     alpha=(get_pixel_component(first,Core::ALPHA_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::ALPHA_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::ALPHA_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::ALPHA_COMPONENT)*x_difference*y_difference)/normalization;
     image[index]=Core::make_pixel(red,green,blue,alpha);
     ++index;
    }

   }

  }

  void Resizer::set_setting(const unsigned int width,const unsigned int height)
  {
   source_width=width;
   source_height=height;
  }

  void Resizer::correct_size(const unsigned int limit)
  {
   if (target_width>limit)
   {
    target_width=limit;
   }
   if (target_height>limit)
   {
    target_height=limit;
   }

  }

  void Resizer::calculate_ratio()
  {
   x_ratio=((source_width-1)*UCHAR_MAX)/(target_width-1);
   y_ratio=((source_height-1)*UCHAR_MAX)/(target_height-1);
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

  void Resizer::create_texture()
  {
   size_t length;
   length=static_cast<size_t>(target_width)*static_cast<size_t>(target_height);
   Resource::create(&image,length);
  }

  bool Resizer::is_dont_need_resize() const
  {
   return (source_width==target_width) && (source_height==target_height);
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
   return image;
  }

  void Resizer::make_texture(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit)
  {
   this->set_setting(width,height);
   this->calculate_size();
   this->correct_size(limit);
   if (this->is_dont_need_resize()==false)
   {
    this->calculate_ratio();
    this->create_texture();
    this->scale_image(target);
   }

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
   point[0].u=0.0f;
   point[0].v=1.0f;
   point[1].u=1.0f;
   point[1].v=1.0f;
   point[2].u=1.0f;
   point[2].v=0.0f;
   point[3].u=0.0f;
   point[3].v=0.0f;
  }

  Shape::~Shape()
  {

  }

  void Shape::set_data(const Core::MIRROR_KIND kind)
  {
   switch (kind)
   {
    case Core::HORIZONTAL_MIRROR:
    vertex[0].x=current_x+target_width;
    vertex[0].y=current_y+target_height;
    vertex[1].x=current_x;
    vertex[1].y=current_y+target_height;
    vertex[2].x=current_x;
    vertex[2].y=current_y;
    vertex[3].x=current_x+target_width;
    vertex[3].y=current_y;
    break;
    case Core::VERTICAL_MIRROR:
    vertex[0].x=current_x;
    vertex[0].y=current_y;
    vertex[1].x=current_x+target_width;
    vertex[1].y=current_y;
    vertex[2].x=current_x+target_width;
    vertex[2].y=current_y+target_height;
    vertex[3].x=current_x;
    vertex[3].y=current_y+target_height;
    break;
    case Core::MIRROR_BOTH:
    vertex[0].x=current_x+target_width;
    vertex[0].y=current_y;
    vertex[1].x=current_x;
    vertex[1].y=current_y;
    vertex[2].x=current_x;
    vertex[2].y=current_y+target_height;
    vertex[3].x=current_x+target_width;
    vertex[3].y=current_y+target_height;
    break;
    default:
    vertex[0].x=current_x;
    vertex[0].y=current_y+target_height;
    vertex[1].x=current_x+target_width;
    vertex[1].y=current_y+target_height;
    vertex[2].x=current_x+target_width;
    vertex[2].y=current_y;
    vertex[3].x=current_x;
    vertex[3].y=current_y;
    break;
   }

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

  void Shape::set_tile_offset(const float row,const float rows,const float column,const float columns)
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

  void Shape::set_horizontal_offset(const float current,const float total)
  {
   this->set_tile_offset(current,total,1.0f,1.0f);
  }

  void Shape::set_vertical_offset(const float current,const float total)
  {
   this->set_tile_offset(1.0f,1.0f,current,total);
  }

  void Shape::set_texture_coordinates(const EUGENEGDK::VERTEX_INDEX index,const float u,const float v)
  {
   point[index].u=u;
   point[index].v=v;
  }

  Rectangle::Rectangle()
  {
   texture=0;
  }

  Rectangle::~Rectangle()
  {
   if (texture!=0)
   {
    glDeleteTextures(1,&texture);
    texture=0;
   }

  }

  void Rectangle::load_texture(const unsigned int width,const unsigned int height,const void *buffer)
  {
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
  }

  void Rectangle::create_texture(const unsigned int *buffer)
  {
   Resizer resizer;
   resizer.make_texture(buffer,this->get_total_width(),this->get_total_height(),MAXIMUM_TEXTURE_SIZE);
   if (resizer.is_dont_need_resize()==true)
   {
    this->load_texture(this->get_total_width(),this->get_total_height(),buffer);
   }
   else
   {
    this->load_texture(resizer.get_width(),resizer.get_height(),resizer.get_buffer());
   }

  }

  void Rectangle::check_texture()
  {
   if (glGetError()!=GL_NO_ERROR)
   {
    texture=0;
   }

  }

  void Rectangle::draw_rectangle()
  {
   glVertexPointer(2,GL_INT,0,vertex);
   glTexCoordPointer(2,GL_FLOAT,0,point);
   glBindTexture(GL_TEXTURE_2D,texture);
   glDrawArrays(GL_TRIANGLE_FAN,0,4);
  }

  void Rectangle::enable_transparent()
  {
   if (glIsEnabled(GL_ALPHA_TEST)==GL_FALSE)
   {
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
   }

  }

  void Rectangle::disable_transparent()
  {
   if (glIsEnabled(GL_ALPHA_TEST)==GL_TRUE)
   {
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

  }

  void Rectangle::destroy_texture()
  {
   if (texture!=0)
   {
    glDeleteTextures(1,&texture);
    texture=0;
   }

  }

  void Rectangle::prepare(const unsigned int *buffer)
  {
   if (buffer!=NULL)
   {
    this->destroy_texture();
    this->create_texture(buffer);
    this->check_texture();
   }

  }

  void Rectangle::draw(const Core::MIRROR_KIND kind)
  {
   if (texture!=0)
   {
    this->set_data(kind);
    this->draw_rectangle();
   }

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

  void Render::set_image_setting()
  {
   glPixelStorei(GL_UNPACK_ALIGNMENT,4);
   glPixelStorei(GL_UNPACK_SWAP_BYTES,0);
   glPixelStorei(GL_UNPACK_LSB_FIRST,0);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
   glPixelStorei(GL_PACK_ALIGNMENT,4);
   glPixelStorei(GL_PACK_SWAP_BYTES,0);
   glPixelStorei(GL_PACK_LSB_FIRST,0);
   glPixelStorei(GL_PACK_ROW_LENGTH,0);
   glPixelStorei(GL_PACK_SKIP_PIXELS,0);
   glPixelStorei(GL_PACK_SKIP_ROWS,0);
  }

  void Render::set_perfomance_setting()
  {
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH);
   glDisable(GL_POLYGON_OFFSET_POINT);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_POLYGON_STIPPLE);
   glDisable(GL_DITHER);
   glDisable(GL_LOGIC_OP);
   glDisable(GL_INDEX_LOGIC_OP);
   glDisable(GL_COLOR_LOGIC_OP);
   glDisable(GL_FOG);
   glDisable(GL_STENCIL_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_NORMALIZE);
   glDisable(GL_AUTO_NORMAL);
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_GEN_Q);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_1D);
   glDisable(GL_CULL_FACE);
   glDisable(GL_MAP1_COLOR_4);
   glDisable(GL_MAP1_INDEX);
   glDisable(GL_MAP1_NORMAL);
   glDisable(GL_MAP1_TEXTURE_COORD_1);
   glDisable(GL_MAP1_TEXTURE_COORD_2);
   glDisable(GL_MAP1_TEXTURE_COORD_3);
   glDisable(GL_MAP1_TEXTURE_COORD_4);
   glDisable(GL_MAP1_VERTEX_3);
   glDisable(GL_MAP1_VERTEX_4);
   glDisable(GL_MAP2_COLOR_4);
   glDisable(GL_MAP2_INDEX);
   glDisable(GL_MAP2_NORMAL);
   glDisable(GL_MAP2_TEXTURE_COORD_1);
   glDisable(GL_MAP2_TEXTURE_COORD_2);
   glDisable(GL_MAP2_TEXTURE_COORD_3);
   glDisable(GL_MAP2_TEXTURE_COORD_4);
   glDisable(GL_MAP2_VERTEX_3);
   glDisable(GL_MAP2_VERTEX_4);
   glEnable(GL_SCISSOR_TEST);
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
   glHint(GL_FOG_HINT,GL_FASTEST);
   glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  }

  void Render::set_common_setting()
  {
   glDrawBuffer(GL_BACK);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glShadeModel(GL_FLAT);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glAlphaFunc(GL_GREATER,0.6f);
   glClearColor(0.0,0.0,0.0,0.0);
  }

  void Render::disable_depth_buffer()
  {
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthMask(GL_FALSE);
   glDisable(GL_DEPTH_TEST);
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
  glScissor(0,0,width,height);
  glViewport(0,0,width,height);
 }

  void Render::create_render(const unsigned int width,const unsigned int height)
  {
   this->set_image_setting();
   this->set_perspective(width,height);
   this->set_perfomance_setting();
   this->set_render_hints();
   this->set_common_setting();
   this->set_matrix_setting();
   this->disable_depth_buffer();
   MAXIMUM_TEXTURE_SIZE=this->get_maximum_texture_size();
  }

  void Render::clear_stage()
  {
   glClear(GL_COLOR_BUFFER_BIT);
  }

  void Render::start_render(const unsigned int width,const unsigned int height)
  {
   this->create_render(width,height);
   this->clear_stage();
  }

 }

 namespace Misc
 {

   Audio::Audio()
   {
    loader=NULL;
    player=NULL;
    controler=NULL;
    video=NULL;
   }

   Audio::~Audio()
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

  void Audio::com_setup()
  {
   if (CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)!=S_OK)
   {
    if (CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)!=S_FALSE)
    {
     EUGENEGDK::Halt("Can't initialize COM");
    }

   }

  }

  void Audio::disable_video()
  {
   if (video!=NULL)
   {
    video->put_FullScreenMode(OAFALSE);
    video->put_AutoShow(OAFALSE);
   }

  }

  void Audio::load_content(const wchar_t *target)
  {
   if (loader!=NULL)
   {
    loader->RenderFile(target,NULL);
   }

  }

  bool Audio::is_play()
  {
   long long int current,total;
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

  void Audio::rewind()
  {
   long long int position;
   position=0;
   if (controler!=NULL)
   {
    controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
   }

  }

  void Audio::play_content()
  {
   if (player!=NULL)
   {
    player->Run();
   }

  }

  void Audio::create_loader()
  {
   if (loader==NULL)
   {
    if (CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,reinterpret_cast<void**>(&loader))!=S_OK)
    {
     loader=NULL;
    }

   }

  }

  void Audio::create_player()
  {
   if (loader!=NULL)
   {
    if (player==NULL)
    {
     if (loader->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&player))!=S_OK)
     {
      player=NULL;
     }

    }

   }

  }

  void Audio::create_controler()
  {
   if (loader!=NULL)
   {
    if (controler==NULL)
    {
     if (loader->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&controler))!=S_OK)
     {
      controler=NULL;
     }

    }

   }

  }

  void Audio::get_video_instance()
  {
   if (loader!=NULL)
   {
    if (video==NULL)
    {
     if (loader->QueryInterface(IID_IVideoWindow,reinterpret_cast<void**>(&video))!=S_OK)
     {
      video=NULL;
     }

    }

   }

  }

  void Audio::initialize()
  {
   this->com_setup();
   this->create_loader();
   this->create_player();
   this->create_controler();
   this->get_video_instance();
   this->disable_video();
  }

  bool Audio::check_playing()
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

  void Audio::stop()
  {
   if (player!=NULL)
   {
    player->Stop();
   }

  }

  void Audio::play()
  {
   this->stop();
   this->rewind();
   this->play_content();
  }

  void Audio::play_loop()
  {
   if (this->check_playing()==false)
   {
    this->play();
   }

  }

  void Audio::play(const bool loop)
  {
   if (loop==true)
   {
    this->play_loop();
   }
   else
   {
    this->play();
   }

  }

  void Audio::load(const char *target)
  {
   Core::Unicode_Converter converter;
   this->stop();
   this->load_content(converter.convert(target));
  }

  void Audio::initialize(const char *target)
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

  unsigned long long int Memory::get_physical_usage()
  {
   this->get_status();
   return memory.ullTotalPhys-memory.ullAvailPhys;
  }

  unsigned long long int Memory::get_virtual_usge()
  {
   this->get_status();
   return memory.ullTotalVirtual-memory.ullAvailVirtual;
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
   preversion=NULL;
  }

  Keyboard::~Keyboard()
  {
   Resource::destroy_array(preversion);
   preversion=NULL;
  }

  void Keyboard::prepare()
  {
   size_t index;
   for (index=0;index<KEYBOARD;++index)
   {
    preversion[index]=KEY_RELEASE;
   }

  }

  bool Keyboard::check_state(const unsigned char code,const unsigned char state)
  {
   bool accept;
   accept=false;
   if (preversion!=NULL)
   {
    accept=(Keys[code]==state) && (preversion[code]!=state);
    preversion[code]=Keys[code];
   }
   return accept;
  }

  void Keyboard::initialize()
  {
   if (preversion==NULL)
   {
    Resource::create(&preversion,KEYBOARD);
    this->prepare();
   }

  }

  bool Keyboard::check_hold(const unsigned char code)
  {
   if (preversion!=NULL)
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
   return preversion!=NULL;
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
   memset(&configuration,0,sizeof(JOYCAPS));
   active=0;
   current.dwButtonNumber=0;
   current.dwButtons=0;
   current.dwReserved1=0;
   current.dwReserved2=0;
   current.dwRpos=0;
   current.dwUpos=0;
   current.dwVpos=0;
   current.dwXpos=0;
   current.dwYpos=0;
   current.dwZpos=0;
   current.dwSize=sizeof(JOYINFOEX);
   current.dwFlags=JOY_RETURNALL;
   current.dwPOV=JOY_POVCENTERED;
   preversion=current;
  }

  Gamepad::~Gamepad()
  {

  }

  void Gamepad::clear_state()
  {
   current.dwButtonNumber=0;
   current.dwButtons=0;
   current.dwReserved1=0;
   current.dwReserved2=0;
   current.dwRpos=0;
   current.dwUpos=0;
   current.dwVpos=0;
   current.dwXpos=0;
   current.dwYpos=0;
   current.dwZpos=0;
   current.dwSize=sizeof(JOYINFOEX);
   current.dwFlags=JOY_RETURNALL;
   current.dwPOV=JOY_POVCENTERED;
   preversion=current;
  }

  bool Gamepad::check_current_state(const EUGENEGDK::GAMEPAD_BUTTONS button) const
  {
   return (current.dwButtons&button)!=0;
  }

  bool Gamepad::check_preversion_state(const EUGENEGDK::GAMEPAD_BUTTONS button) const
  {
   return (preversion.dwButtons&button)!=0;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_right_stick_horizontal_directional() const
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   if (configuration.wNumAxes==4)
   {
    directional=Core::get_horizontal_direction(current.dwRpos,configuration.wRmax,configuration.wRmin); // Old gamepad
   }
   else
   {
    if (configuration.wMid==1118)
    {
     directional=Core::get_horizontal_direction(current.dwUpos,configuration.wUmax,configuration.wUmin); // Xbox gamepad;
    }
    else
    {
     directional=Core::get_horizontal_direction(current.dwZpos,configuration.wZmax,configuration.wZmin); // Other modern gamepad
    }

   }
   return directional;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_right_stick_vertical_directional() const
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   if (configuration.wNumAxes==4)
   {
    directional=Core::get_vertical_direction(current.dwUpos,configuration.wUmax,configuration.wUmin); // Old gamepad
   }
   else
   {
    directional=Core::get_vertical_direction(current.dwRpos,configuration.wRmax,configuration.wRmin); // Modern gamepad
   }
   return directional;
  }

  void Gamepad::update()
  {
   preversion=current;
   if (joyGetPosEx(active,&current)!=JOYERR_NOERROR)
   {
    this->clear_state();
   }
   if (joyGetDevCaps(static_cast<size_t>(active),&configuration,sizeof(JOYCAPS))!=JOYERR_NOERROR)
   {
    memset(&configuration,0,sizeof(JOYCAPS));
   }

  }

  unsigned int Gamepad::get_amount()
  {
   return joyGetNumDevs();
  }

  unsigned int Gamepad::get_button_amount() const
  {
   return configuration.wNumButtons;
  }

  unsigned int Gamepad::get_stick_amount() const
  {
   return (configuration.wNumAxes==0) ? 0:(configuration.wNumAxes/2);
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
    dpad=EUGENEGDK::GAMEPAD_RIGHT;
    break;
    case 31500:
    dpad=EUGENEGDK::GAMEPAD_UPLEFT;
    break;
    case 4500:
    dpad=EUGENEGDK::GAMEPAD_UPRIGHT;
    break;
    case 22500:
    dpad=EUGENEGDK::GAMEPAD_DOWNLEFT;
    break;
    case 13500:
    dpad=EUGENEGDK::GAMEPAD_DOWNRIGHT;
    break;
    default:
    ;
    break;
   }
   return dpad;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_stick_x(const EUGENEGDK::GAMEPAD_STICKS stick) const
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   if (stick==EUGENEGDK::GAMEPAD_LEFT_STICK)
   {
    if (this->get_stick_amount()>0)
    {
     directional=Core::get_horizontal_direction(current.dwXpos,configuration.wXmax,configuration.wXmin);
    }

   }
   else
   {
    if (this->get_stick_amount()>1)
    {
     directional=this->get_right_stick_horizontal_directional();
    }

   }
   return directional;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_stick_y(const EUGENEGDK::GAMEPAD_STICKS stick) const
  {
   EUGENEGDK::GAMEPAD_DIRECTION directional;
   directional=EUGENEGDK::GAMEPAD_NEUTRAL_DIRECTION;
   if (stick==EUGENEGDK::GAMEPAD_LEFT_STICK)
   {
    if (this->get_stick_amount()>0)
    {
     directional=Core::get_vertical_direction(current.dwYpos,configuration.wYmax,configuration.wYmin);
    }

   }
   else
   {
    if (this->get_stick_amount()>1)
    {
     directional=this->get_right_stick_vertical_directional();
    }

   }
   return directional;
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_left_stick_x() const
  {
   return this->get_stick_x(EUGENEGDK::GAMEPAD_LEFT_STICK);
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_left_stick_y() const
  {
   return this->get_stick_y(EUGENEGDK::GAMEPAD_LEFT_STICK);
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_right_stick_x() const
  {
   return this->get_stick_x(EUGENEGDK::GAMEPAD_RIGHT_STICK);
  }

  EUGENEGDK::GAMEPAD_DIRECTION Gamepad::get_right_stick_y() const
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

  long int Binary_File::get_position()
  {
   return (target==NULL) ? 0:ftell(target);
  }

  bool Binary_File::check_error()
  {
   return (target==NULL) ? true:(ferror(target)!=0);
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

  }

  Input_File* Input_File::get_handle()
  {
   return this;
  }

  void Input_File::open(const char *name)
  {
   this->close();
   target=fopen(name,"rb");
  }

  void Input_File::read(void *buffer,const size_t length)
  {
   if (this->target!=NULL)
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

  }

  Output_File* Output_File::get_handle()
  {
   return this;
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
   if (this->target!=NULL)
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

  }

  Screen::~Screen()
  {

  }

  void Screen::check_video_mode()
  {
   this->get_video_mode();
   if (this->get_depth()<16)
   {
    this->correct_depth();
    this->set_video_mode();
   }

  }

  void Screen::set_resolution(const unsigned int width,const unsigned int height)
  {
   this->get_video_mode();
   this->correct_depth();
   this->set_setting(width,height);
   this->set_video_mode();
  }

  void Screen::screen_setup()
  {
   this->prepare_engine();
   this->set_render(this->get_context());
   this->start_render(this->get_display_width(),this->get_display_height());
   this->create_timer(17);
  }

  void Screen::clear_screen()
  {
   if (this->get_context()!=NULL)
   {
    this->clear_stage();
   }

  }

  void Screen::initialize()
  {
   if (this->get_context()==NULL)
   {
    this->check_video_mode();
    this->screen_setup();
   }

  }

  void Screen::initialize(const unsigned int width,const unsigned int height)
  {
   if (this->get_context()==NULL)
   {
    this->set_resolution(width,height);
    this->screen_setup();
   }

  }

  bool Screen::update()
  {
   if (this->get_context()!=NULL)
   {
    this->Swap();
    this->update_counter();
    this->clear_stage();
   }
   return this->process_message();
  }

  bool Screen::sync()
  {
   this->wait_timer();
   return this->update();
  }

  bool Screen::sync(const bool limit)
  {
   if (limit==true)
   {
    this->wait_timer();
   }
   return this->update();
  }

  bool Screen::is_ready()
  {
   return this->get_context()!=NULL;
  }

  bool Screen::is_accelerated() const
  {
   return this->is_software_render()==false;
  }

  bool Screen::is_software() const
  {
   return this->is_software_render();
  }

  unsigned int Screen::get_color() const
  {
   return this->get_depth();
  }

  unsigned int Screen::get_fps() const
  {
   return this->get_fps_amount();
  }

  unsigned int Screen::get_width() const
  {
   return this->get_display_width();
  }

  unsigned int Screen::get_height() const
  {
   return this->get_display_height();
  }

  bool Screen::check_x(const unsigned int x) const
  {
   return x<this->get_width();
  }

  bool Screen::check_y(const unsigned int y) const
  {
   return y<this->get_height();
  }

  Screen* Screen::get_handle()
  {
   return this;
  }

  Image::Image()
  {
   data=NULL;
   width=0;
   height=0;
   length=0;
  }

  Image::~Image()
  {
   Resource::destroy_array(data);
   data=NULL;
  }

  size_t Image::get_source_position(const unsigned int x,const unsigned int y,const Core::MIRROR_KIND mirror) const
  {
   size_t position;
   switch (mirror)
   {
    case Core::HORIZONTAL_MIRROR:
    position=Core::get_offset(width-x-1,y,width);
    break;
    case Core::VERTICAL_MIRROR:
    position=Core::get_offset(x,height-y-1,width);
    break;
    case Core::MIRROR_BOTH:
    position=Core::get_offset(width-x-1,height-y-1,width);
    break;
    default:
    position=Core::get_offset(x,y,width);
    break;
   }
   return position*3;
  }

  void Image::set_size(const unsigned int image_width,const unsigned int image_height)
  {
   width=image_width;
   height=image_height;
   length=static_cast<size_t>(width)*static_cast<size_t>(height)*3;
  }

  void Image::mirror_image(const Core::MIRROR_KIND mirror)
  {
   unsigned char *mirrored;
   unsigned int x,y;
   size_t index,position;
   index=0;
   position=0;
   mirrored=NULL;
   Resource::create(&mirrored,length);
   for (y=0;y<height;++y)
   {
    for (x=0;x<width;++x)
    {
     position=this->get_source_position(x,y,mirror);
     mirrored[index]=data[position];
     mirrored[index+1]=data[position+1];
     mirrored[index+2]=data[position+2];
     index+=3;
    }

   }
   Resource::destroy_array(data);
   data=mirrored;
  }

  void Image::uncompress_tga_data(const unsigned char *source)
  {
   size_t index,position,amount;
   index=0;
   position=0;
   while (index<length)
   {
    if (source[position]<128)
    {
     for (amount=source[position]+1;amount>0;--amount)
     {
      data[index]=source[position+1];
      data[index+1]=source[position+2];
      data[index+2]=source[position+3];
      index+=3;
      position+=3;
     }
     ++position;
    }
    else
    {
     for (amount=source[position]-127;amount>0;--amount)
     {
      data[index]=source[position+1];
      data[index+1]=source[position+2];
      data[index+2]=source[position+3];
      index+=3;
     }
     position+=sizeof(unsigned int);
    }

   }

  }

  void Image::mirror_tga(const unsigned char descriptor)
  {
   switch (descriptor)
   {
    case 0x00: // Bottom-left
    this->mirror_image(Core::VERTICAL_MIRROR);
    break;
    case 0x30: // Top-right
    this->mirror_image(Core::HORIZONTAL_MIRROR);
    break;
    case 0x10: // Bottom-right
    this->mirror_image(Core::MIRROR_BOTH);
    break;
    default: // Top-left
    ;
    break;
   }

  }

  void Image::load_tga(File::Input_File &target)
  {
   unsigned char *buffer;
   size_t compressed_length;
   TGA_head head;
   TGA_map color_map;
   TGA_image image;
   buffer=NULL;
   compressed_length=static_cast<size_t>(target.get_length()-18);
   target.read(&head,3);
   target.read(&color_map,5);
   target.read(&image,10);
   if (image.color==24)
   {
    this->set_size(image.width,image.height);
    Resource::create(&data,length);
    switch (head.type)
    {
     case 2:
     target.read(data,length);
     break;
     case 10:
     Resource::create(&buffer,compressed_length);
     target.read(buffer,compressed_length);
     this->uncompress_tga_data(buffer);
     Resource::destroy_array(buffer);
     buffer=NULL;
     break;
     default:
     this->destroy_image();
     break;
    }
    this->mirror_tga(image.descriptor);
   }

  }

  void Image::destroy_image()
  {
   Resource::destroy_array(data);
   data=NULL;
   this->set_size(0,0);
  }

  size_t Image::get_length() const
  {
   return length;
  }

  unsigned int Image::get_width() const
  {
   return width;
  }

  unsigned int Image::get_height() const
  {
   return height;
  }

  unsigned char *Image::get_data()
  {
   return data;
  }

  unsigned char *Image::load(const char *name)
  {
   File::Input_File target;
   if (name!=NULL)
   {
    this->destroy_image();
    target.open(name);
   }
   if (target.is_open()==true)
   {
    this->load_tga(target);
    target.close();
   }
   return data;
  }

  Image* Image::get_handle()
  {
   return this;
  }

  Picture::Picture()
  {
   image=NULL;
   image_width=0;
   image_height=0;
   pixels=0;
  }

  Picture::~Picture()
  {
   Resource::destroy_array(image);
   image=NULL;
  }

  void Picture::set_image_size(const unsigned int width,const unsigned int height)
  {
   image_width=width;
   image_height=height;
   pixels=static_cast<size_t>(width)*static_cast<size_t>(height);
  }

  void Picture::create_storage()
  {
   Resource::create(&image,pixels);
  }

  void Picture::copy_image(const unsigned int *target)
  {
   size_t index;
   for (index=0;index<pixels;++index)
   {
    image[index]=target[index];
   }

  }

  void Picture::convert_image(const unsigned char *target)
  {
   size_t index,position;
   position=0;
   for (index=0;index<pixels;++index)
   {
    image[index]=Core::make_pixel(target[position+2],target[position+1],target[position],0);
    if (image[0]!=image[index])
    {
     image[index]=Core::make_pixel(target[position+2],target[position+1],target[position],UCHAR_MAX);
    }
    position+=3;
   }

  }

  void Picture::load_image(Image *buffer)
  {
   if (buffer!=NULL)
   {
    if (buffer->get_length()>0)
    {
     this->destroy_image();
     this->set_image_size(buffer->get_width(),buffer->get_height());
     this->create_storage();
     this->convert_image(buffer->get_data());
    }

   }

  }

  void Picture::destroy_image()
  {
   Resource::destroy_array(image);
   image=NULL;
   this->set_image_size(0,0);
  }

  bool Picture::is_storage_empty() const
  {
   return pixels==0;
  }

  unsigned int Picture::get_image_width() const
  {
   return image_width;
  }

  unsigned int Picture::get_image_height() const
  {
   return image_height;
  }

  size_t Picture::get_pixel_amount() const
  {
   return pixels;
  }

  size_t Picture::get_image_length() const
  {
   return pixels*sizeof(unsigned int);
  }

  unsigned int *Picture::get_image()
  {
   return image;
  }

  Animation::Animation()
  {
   frame=1;
   frames=1;
  }

  Animation::~Animation()
  {

  }

  void Animation::reset_animation_setting()
  {
   frame=1;
   frames=1;
  }

  void Animation::increase_frame()
  {
   ++frame;
   if (frame>frames)
   {
    frame=1;
   }

  }

  void Animation::set_frame(const unsigned int target)
  {
   if (this->check_frame(target)==true)
   {
    frame=target;
   }

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

  bool Animation::is_last_frame() const
  {
   return frame==frames;
  }

  bool Animation::check_frame(const unsigned int target) const
  {
   return (target>0) && (target<=frames);
  }

  Billboard::Billboard()
  {
   billboard.set_size(0,0);
   transparent=true;
   current_x=0;
   current_y=0;
   start_x=0;
   start_y=0;
   sprite_width=0;
   sprite_height=0;
   mirror=Core::MIRROR_NONE;
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
   billboard.draw(mirror);
  }

  void Billboard::reset_billboard_setting()
  {
   transparent=true;
   current_x=0;
   current_y=0;
   start_x=0;
   start_y=0;
   sprite_width=0;
   sprite_height=0;
   mirror=Core::MIRROR_NONE;
  }

  void Billboard::prepare(const unsigned int width,const unsigned int height,const unsigned int *picture)
  {
   billboard.set_total_size(width,height);
   billboard.prepare(picture);
  }

  void Billboard::set_texture_coordinates(const EUGENEGDK::VERTEX_INDEX index,const float u,const float v)
  {
   billboard.set_texture_coordinates(index,u,v);
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

  void Billboard::set_start(const unsigned int x,const unsigned int y)
  {
   if (billboard.is_texture_exist()==true)
   {
    start_x=x;
    start_y=y;
   }

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

  unsigned int Billboard::increase_x()
  {
   if (billboard.is_texture_exist()==true)
   {
    ++current_x;
   }
   return current_x;
  }

  unsigned int Billboard::decrease_x()
  {
   if (billboard.is_texture_exist()==true)
   {
    --current_x;
   }
   return current_x;
  }

  unsigned int Billboard::increase_y()
  {
   if (billboard.is_texture_exist()==true)
   {
    ++current_y;
   }
   return current_y;
  }

  unsigned int Billboard::decrease_y()
  {
   if (billboard.is_texture_exist()==true)
   {
    --current_y;
   }
   return current_y;
  }

  unsigned int Billboard::increase_x(const unsigned int increment)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x+=increment;
   }
   return current_x;
  }

  unsigned int Billboard::decrease_x(const unsigned int decrement)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x-=decrement;
   }
   return current_x;
  }

  unsigned int Billboard::increase_y(const unsigned int increment)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y+=increment;
   }
   return current_y;
  }

  unsigned int Billboard::decrease_y(const unsigned int decrement)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y-=decrement;
   }
   return current_y;
  }

  unsigned int Billboard::get_start_x() const
  {
   return start_x;
  }

  unsigned int Billboard::get_start_y() const
  {
   return start_y;
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

  bool Billboard::is_load() const
  {
   return billboard.is_texture_exist();
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

  void Billboard::disable_mirror()
  {
   mirror=Core::MIRROR_NONE;
  }

  void Billboard::horizontal_mirror()
  {
   mirror=Core::HORIZONTAL_MIRROR;
  }

  void Billboard::vertical_mirror()
  {
   mirror=Core::VERTICAL_MIRROR;
  }

  void Billboard::complex_mirror()
  {
   mirror=Core::MIRROR_BOTH;
  }

  void Billboard::go_start()
  {
   this->set_position(start_x,start_y);
  }

  void Billboard::go_start_x()
  {
   this->set_x(start_x);
  }

  void Billboard::go_start_y()
  {
   this->set_y(start_y);
  }

  void Billboard::draw()
  {
   this->check_transparent();
   this->draw_sprite_image();
  }

  void Billboard::draw(const unsigned int x,const unsigned int y)
  {
   this->set_position(x,y);
   this->draw();
  }

  void Billboard::draw(const bool transparency)
  {
   this->set_transparent(transparency);
   this->draw();
  }

  void Billboard::draw(const bool transparency,const unsigned int x,const unsigned int y)
  {
   this->set_transparent(transparency);
   this->draw(x,y);
  }

  Sprite::Sprite()
  {
   current_kind=EUGENEGDK::HORIZONTAL_ANIMATED;
  }

  Sprite::~Sprite()
  {

  }

  void Sprite::reset_sprite_setting()
  {
   current_kind=EUGENEGDK::HORIZONTAL_ANIMATED;
  }

  void Sprite::set_sprite_setting()
  {
   if (current_kind==EUGENEGDK::HORIZONTAL_ANIMATED)
   {
    this->set_size(this->get_image_width()/this->get_frames(),this->get_image_height());
   }
   else
   {
    this->set_size(this->get_image_width(),this->get_image_height()/this->get_frames());
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
   if (current_kind==EUGENEGDK::HORIZONTAL_ANIMATED)
   {
    billboard.set_horizontal_offset(static_cast<float>(this->get_frame()),static_cast<float>(this->get_frames()));
   }
   else
   {
    billboard.set_vertical_offset(static_cast<float>(this->get_frame()),static_cast<float>(this->get_frames()));
   }

  }

  void Sprite::set_kind(const EUGENEGDK::IMAGE_KIND kind)
  {
   current_kind=kind;
   this->configure_sprite();
   this->set_sprite_frame();
  }

  Sprite* Sprite::get_handle()
  {
   return this;
  }

  EUGENEGDK::IMAGE_KIND Sprite::get_kind() const
  {
   return current_kind;
  }

  void Sprite::set_settings(const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   this->reset_animation_setting();
   this->set_frames(frames);
   this->set_kind(kind);
  }

  void Sprite::load(Image *buffer,const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   this->load_image(buffer);
   if (this->is_storage_empty()==false)
   {
    this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
    this->set_settings(kind,frames);
   }

  }

  void Sprite::load(Image &buffer,const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   this->load(buffer.get_handle(),kind,frames);
  }

  void Sprite::load(const char *name,const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   Image picture;
   picture.load(name);
   this->load(picture,kind,frames);
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

  void Sprite::destroy()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
   this->reset_animation_setting();
   this->reset_sprite_setting();
  }

  void Sprite::clone(Sprite *target)
  {
   if (target!=NULL)
   {
    if (target->get_image_length()>0)
    {
     this->destroy();
     this->set_image_size(target->get_image_width(),target->get_image_height());
     this->create_storage();
     this->set_settings(target->get_kind(),target->get_frames());
     this->set_transparent(target->get_transparent());
     this->copy_image(target->get_image());
     this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
     this->set_size(target->get_width(),target->get_height());
    }

   }

  }

  void Sprite::clone(Sprite &target)
  {
   this->clone(target.get_handle());
  }

  Cartoon::Cartoon()
  {

  }

  Cartoon::~Cartoon()
  {

  }

  Cartoon* Cartoon::get_handle()
  {
   return this;
  }

  void Cartoon::load(Image *buffer)
  {
   this->load_image(buffer);
   if (this->is_storage_empty()==false)
   {
    this->reset_billboard_setting();
    this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
    this->set_size(this->get_image_width(),this->get_image_height());
   }

  }

  void Cartoon::load(Image &buffer)
  {
   this->load(buffer.get_handle());
  }

  void Cartoon::load(const char *name)
  {
   Image picture;
   picture.load(name);
   this->load(picture);
  }

  void Cartoon::destroy()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
  }

  void Cartoon::clone(Cartoon *target)
  {
   if (target!=NULL)
   {
    if (target->get_image_length()>0)
    {
     this->destroy();
     this->set_image_size(target->get_image_width(),target->get_image_height());
     this->create_storage();
     this->set_transparent(target->get_transparent());
     this->copy_image(target->get_image());
     this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
     this->set_size(target->get_width(),target->get_height());
    }

   }

  }

  void Cartoon::clone(Cartoon &target)
  {
   this->clone(target.get_handle());
  }

  Sheet::Sheet()
  {
   rows=1;
   columns=1;
  }

  Sheet::~Sheet()
  {

  }

  Sheet* Sheet::get_handle()
  {
   return this;
  }

  bool Sheet::check_row(const unsigned int target) const
  {
   return (target>0) && (target<=rows);
  }

  bool Sheet::check_column(const unsigned int target) const
  {
   return (target>0) && (target<=columns);
  }

  void Sheet::reset_sheet_setting()
  {
   rows=1;
   columns=1;
  }

  void Sheet::prepare_sheet()
  {
   this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
   this->set_size(this->get_image_width()/rows,this->get_image_height()/columns);
  }

  unsigned int Sheet::get_row(const unsigned int target) const
  {
   unsigned int row;
   row=1;
   if (this->check_frame(target)==true)
   {
    row+=(target-1)%rows;
   }
   return row;
  }

  unsigned int Sheet::get_column(const unsigned int target) const
  {
   unsigned int column;
   column=1;
   if (this->check_frame(target)==true)
   {
    column+=(target-1)/rows;
   }
   return column;
  }

  unsigned int Sheet::calculate(const unsigned int row,const unsigned int column) const
  {
   unsigned int target;
   target=1;
   if (this->check_row(row)==true)
   {
    if (this->check_column(column)==true)
    {
     target=row+column*rows;
    }

   }
   return target;
  }

  unsigned int Sheet::get_rows() const
  {
   return rows;
  }

  unsigned int Sheet::get_columns() const
  {
   return columns;
  }

  void Sheet::destroy()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
   this->reset_animation_setting();
   this->reset_sheet_setting();
  }

  void Sheet::clone(Sheet *target)
  {
   if (target!=NULL)
   {
    if (target->get_image_length()>0)
    {
     this->destroy();
     this->set_image_size(target->get_image_width(),target->get_image_height());
     this->create_storage();
     this->copy_image(target->get_image());
     rows=target->get_rows();
     columns=target->get_columns();
     this->set_frames(rows*columns);
     this->prepare_sheet();
     this->set_transparent(target->get_transparent());
     this->set_target(target->get_frame());
     this->set_size(target->get_width(),target->get_height());
    }

   }

  }

  void Sheet::clone(Sheet &target)
  {
   this->clone(target.get_handle());
  }

  void Sheet::select(const unsigned int row,const unsigned int column)
  {
   if (this->check_row(row)==true)
   {
    if (this->check_column(column)==true)
    {
     billboard.set_tile_offset(static_cast<float>(row),static_cast<float>(rows),static_cast<float>(column),static_cast<float>(columns));
    }

   }

  }

  void Sheet::set_target(const unsigned int target)
  {
   this->set_frame(target);
   if (this->check_frame(target)==true)
   {
    this->select(this->get_row(target),this->get_column(target));
   }

  }

  void Sheet::step()
  {
   this->increase_frame();
   this->select(this->get_row(this->get_frame()),this->get_column(this->get_frame()));
  }

  void Sheet::load(Image *sheet,const unsigned int row_amount,const unsigned int column_amount)
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
      this->set_target(1);
      this->prepare_sheet();
     }

    }

   }

  }

  void Sheet::load(Image &sheet,const unsigned int row_amount,const unsigned int column_amount)
  {
   this->load(sheet.get_handle(),row_amount,column_amount);
  }

  void Sheet::load(const char *name,const unsigned int row_amount,const unsigned int column_amount)
  {
   Image picture;
   picture.load(name);
   this->load(picture,row_amount,column_amount);
  }

  Background::Background()
  {
   stage.set_position(0,0);
  }

  Background::~Background()
  {
   stage.destroy();
  }

  Background* Background::get_handle()
  {
   return this;
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

  void Background::set_settings(const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.set_settings(kind,frames);
  }

  void Background::load(Image *background,const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.load(background,kind,frames);
  }

  void Background::load(Image &background,const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.load(background,kind,frames);
  }

  void Background::load(const char *name,const EUGENEGDK::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.load(name,kind,frames);
  }

  void Background::disable_mirror()
  {
   stage.disable_mirror();
  }

  void Background::horizontal_mirror()
  {
   stage.horizontal_mirror();
  }

  void Background::vertical_mirror()
  {
   stage.vertical_mirror();
  }

  void Background::complex_mirror()
  {
   stage.complex_mirror();
  }

  void Background::set_target(const unsigned int target)
  {
   stage.set_target(target);
  }

  void Background::step()
  {
   stage.step();
  }

  void Background::draw()
  {
   stage.draw(false);
  }

  void Background::draw(const unsigned int target)
  {
   stage.set_target(target);
   stage.draw(false);
  }

  void Background::destroy_image()
  {
   stage.destroy_image();
  }

  void Background::destroy()
  {
   stage.destroy();
  }

  bool Background::is_load() const
  {
   return stage.is_load();
  }

  bool Background::is_last_frame() const
  {
   return stage.is_last_frame();
  }

  bool Background::check_frame(const unsigned int target) const
  {
   return stage.check_frame(target);
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

  EUGENEGDK::IMAGE_KIND Background::get_kind() const
  {
   return stage.get_kind();
  }

  Scene::Scene()
  {
   stage.set_position(0,0);
  }

  Scene::~Scene()
  {
   stage.destroy();
  }

  Scene* Scene::get_handle()
  {
   return this;
  }

  void Scene::prepare(const Screen *screen)
  {
   if (screen!=NULL)
   {
    stage.set_size(screen->get_width(),screen->get_height());
   }

  }

  void Scene::prepare(const unsigned int width,const unsigned int height)
  {
   stage.set_size(width,height);
  }

  void Scene::prepare(Screen &screen)
  {
   this->prepare(screen.get_handle());
  }

  void Scene::load(Image *background)
  {
   stage.load(background);
  }

  void Scene::load(Image &background)
  {
   stage.load(background);
  }

  void Scene::load(const char *name)
  {
   stage.load(name);
  }

  void Scene::disable_mirror()
  {
   stage.disable_mirror();
  }

  void Scene::horizontal_mirror()
  {
   stage.horizontal_mirror();
  }

  void Scene::vertical_mirror()
  {
   stage.vertical_mirror();
  }

  void Scene::complex_mirror()
  {
   stage.complex_mirror();
  }

  void Scene::draw()
  {
   stage.draw(false);
  }

  void Scene::destroy_image()
  {
   stage.destroy_image();
  }

  void Scene::destroy()
  {
   stage.destroy();
  }

  bool Scene::is_load() const
  {
   return stage.is_load();
  }

  unsigned int Scene::get_width() const
  {
   return stage.get_width();
  }

  unsigned int Scene::get_height() const
  {
   return stage.get_height();
  }

  Parallax::Parallax()
  {
   stage.set_size(0,0);
   u_offset=0.0f;
   v_offset=0.0f;
   u_speed=0.0f;
   v_speed=0.0f;
  }

  Parallax::~Parallax()
  {
   stage.destroy();
  }

  void Parallax::calculate_offset()
  {
   u_offset+=u_speed;
   v_offset+=v_speed;
  }

  void Parallax::reset_u_offset()
  {
   if (u_offset>1.0f)
   {
    u_offset=0.0f;
   }

  }

  void Parallax::reset_v_offset()
  {
   if (v_offset>1.0f)
   {
    v_offset=0.0f;
   }

  }

  void Parallax::set_texture_coordinates()
  {
   stage.set_texture_coordinates(EUGENEGDK::FIRST_VERTEX,u_offset,1.0f+v_offset);
   stage.set_texture_coordinates(EUGENEGDK::SECOND_VERTEX,1.0f+u_offset,1.0f+v_offset);
   stage.set_texture_coordinates(EUGENEGDK::THIRD_VERTEX,1.0f+u_offset,v_offset);
   stage.set_texture_coordinates(EUGENEGDK::LAST_VERTEX,u_offset,v_offset);
  }

  Parallax* Parallax::get_handle()
  {
   return this;
  }

  void Parallax::prepare(const Screen *screen)
  {
   if (screen!=NULL)
   {
    stage.set_size(screen->get_width(),screen->get_height());
   }

  }

  void Parallax::prepare(const unsigned int width,const unsigned int height)
  {
   stage.set_size(width,height);
  }

  void Parallax::prepare(Screen &screen)
  {
   this->prepare(screen.get_handle());
  }

  void Parallax::load(Image *background)
  {
   stage.load(background);
  }

  void Parallax::load(Image &background)
  {
   stage.load(background);
  }

  void Parallax::load(const char *name)
  {
   stage.load(name);
  }

  void Parallax::disable_mirror()
  {
   stage.disable_mirror();
  }

  void Parallax::horizontal_mirror()
  {
   stage.horizontal_mirror();
  }

  void Parallax::vertical_mirror()
  {
   stage.vertical_mirror();
  }

  void Parallax::complex_mirror()
  {
   stage.complex_mirror();
  }

  void Parallax::destroy_image()
  {
   stage.destroy_image();
  }

  void Parallax::set_speed(const float horizontal,const float vertical)
  {
   if (horizontal>0.0f)
   {
    u_speed=horizontal;
   }
   if (vertical>0.0f)
   {
    v_speed=vertical;
   }

  }

  float Parallax::get_horizontal_speed() const
  {
   return u_speed;
  }

  float Parallax::get_vertical_speed() const
  {
   return v_speed;
  }

  void Parallax::draw()
  {
   this->calculate_offset();
   this->reset_u_offset();
   this->reset_v_offset();
   this->set_texture_coordinates();
   stage.draw(false);
  }

  void Parallax::destroy()
  {
   stage.destroy();
  }

  bool Parallax::is_load() const
  {
   return stage.is_load();
  }

  unsigned int Parallax::get_width() const
  {
   return stage.get_width();
  }

  unsigned int Parallax::get_height() const
  {
   return stage.get_height();
  }

  Text::Text()
  {
   text.set_size(0,0);
   orientation=EUGENEGDK::HORIZONTAL_TEXT;
  }

  Text::~Text()
  {
   text.destroy();
  }

  Text* Text::get_handle()
  {
   return this;
  }

  void Text::increase_position()
  {
   if (orientation==EUGENEGDK::HORIZONTAL_TEXT)
   {
    text.increase_x(text.get_width());
   }
   else
   {
    text.increase_y(text.get_height());
   }

  }

  void Text::restore_position()
  {
   text.go_start();
  }

  EUGENEGDK::TEXT_KIND Text::get_orientation() const
  {
   return orientation;
  }

  void Text::set_orientation(const EUGENEGDK::TEXT_KIND target)
  {
   orientation=target;
  }

  bool Text::is_font_load() const
  {
   return text.is_load();
  }

  unsigned int Text::get_font_width() const
  {
   return text.get_width();
  }

  unsigned int Text::get_font_height() const
  {
   return text.get_height();
  }

  unsigned int Text::get_x() const
  {
   return text.get_start_x();
  }

  unsigned int Text::get_y() const
  {
   return text.get_start_y();
  }

  void Text::set_position(const unsigned int x,const unsigned int y)
  {
   text.set_start(x,y);
   text.set_position(x,y);
  }

  void Text::set_size(const unsigned int width,const unsigned int height)
  {
   text.set_size(width,height);
  }

  void Text::set_width(const unsigned int width)
  {
   text.set_width(width);
  }

  void Text::set_height(const unsigned int height)
  {
   text.set_height(height);
  }

  void Text::set_settings(const unsigned int width,const unsigned int height,const EUGENEGDK::TEXT_KIND kind)
  {
   this->set_size(width,height);
   this->set_orientation(kind);
  }

  void Text::load_font(Image *font)
  {
   text.load(font,16,16);
  }

  void Text::load_font(Image &font)
  {
   text.load(font,16,16);
  }

  void Text::load_font(const char *name)
  {
   text.load(name,16,16);
  }

  void Text::print(const char target)
  {
   text.set_target(static_cast<unsigned char>(target)+1);
   text.draw(true);
  }

  void Text::print(const unsigned int x,const unsigned int y,const char target)
  {
   this->set_position(x,y);
   this->print(target);
  }

  size_t Text::print(const char *target)
  {
   size_t index,length;
   length=strlen(target);
   this->restore_position();
   for (index=0;index<length;++index)
   {
    this->print(target[index]);
    this->increase_position();
   }
   return length;
  }

  size_t Text::print(const unsigned int x,const unsigned int y,const char *target)
  {
   this->set_position(x,y);
   return this->print(target);
  }

  void Text::disable_mirror()
  {
   text.disable_mirror();
  }

  void Text::horizontal_mirror()
  {
   text.horizontal_mirror();
  }

  void Text::vertical_mirror()
  {
   text.vertical_mirror();
  }

  void Text::complex_mirror()
  {
   text.complex_mirror();
  }

  void Text::destroy_image()
  {
   text.destroy_image();
  }

  void Text::destroy_font()
  {
   text.destroy();
  }

 }

 namespace Transformation
 {

  Coordinates::Coordinates()
  {
   half_viewport_width=1;
   half_viewport_height=1;
  }

  Coordinates::~Coordinates()
  {

  }

  Coordinates* Coordinates::get_handle()
  {
   return this;
  }

  void Coordinates::initialize(const int viewport_width,const int viewport_height)
  {
   if (viewport_width>1)
   {
    half_viewport_width=viewport_width/2;
   }
   if (viewport_height>1)
   {
    half_viewport_height=viewport_height/2;
   }

  }

  int Coordinates::get_viewport_width() const
  {
   return 2*half_viewport_width;
  }

  int Coordinates::get_viewport_height() const
  {
   return 2*half_viewport_height;
  }

  int Coordinates::get_lowest_cartesian_x() const
  {
   return -1*half_viewport_width;
  }

  int Coordinates::get_highest_cartesian_x() const
  {
   return half_viewport_width;
  }

  int Coordinates::get_lowest_cartesian_y() const
  {
   return -1*half_viewport_height;
  }

  int Coordinates::get_highest_cartesian_y() const
  {
   return half_viewport_height;
  }

  bool Coordinates::check_cartesian_x(const int x) const
  {
   return (x>=this->get_lowest_cartesian_x()) && (x<=this->get_highest_cartesian_x());
  }

  bool Coordinates::check_cartesian_y(const int y) const
  {
   return (y>=this->get_lowest_cartesian_y()) && (y<=this->get_highest_cartesian_y());
  }

  int Coordinates::get_cartesian_x(const int x) const
  {
   return (x<0) ? half_viewport_width:(x-half_viewport_width);
  }

  int Coordinates::get_cartesian_y(const int y) const
  {
   return (y<0) ? half_viewport_height:(half_viewport_height-y);
  }

  int Coordinates::get_screen_x(const int x) const
  {
   return x+half_viewport_width;
  }

  int Coordinates::get_screen_y(const int y) const
  {
   return half_viewport_height-y;
  }

  int Isometric::get_isometric_x(const int x,const int y)
  {
   return x-y;
  }

  int Isometric::get_isometric_y(const int x,const int y)
  {
   return (x+y)/2;
  }

  int Isometric::get_cartesian_x(const int x,const int y)
  {
   return (2*y+x)/2;
  }

  int Isometric::get_cartesian_y(const int x,const int y)
  {
   return (2*y-x)/2;
  }

  Level::Level()
  {
   start_x=0;
   start_y=0;
   halft_tile_width=0;
   halft_tile_height=0;
  }

  Level::~Level()
  {

  }

  Level* Level::get_handle()
  {
   return this;
  }

  void Level::initialize(const int tile_width,const int tile_height)
  {
   if (tile_width>0)
   {
    halft_tile_width=tile_width/2;
   }
   if (tile_height>0)
   {
    halft_tile_height=tile_height/2;
   }

  }

  void Level::set_offset(const int x_offset,const int y_offset)
  {
   start_x=x_offset;
   start_y=y_offset;
  }

  int Level::get_x_offset() const
  {
   return start_x;
  }

  int Level::get_y_offset() const
  {
   return start_y;
  }

  int Level::get_x(const int row,const int column) const
  {
   return start_x+(row-column)*halft_tile_width;
  }

  int Level::get_y(const int row,const int column) const
  {
   return start_y+(row+column)*halft_tile_height;
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

  Timer* Timer::get_handle()
  {
   return this;
  }

  void Timer::set_timer(const double seconds)
  {
   interval=seconds;
   start=time(NULL);
  }

  double Timer::get_interval() const
  {
   return interval;
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

  Collision* Collision::get_handle()
  {
   return this;
  }

  bool Collision::check_horizontal_collision() const
  {
   return ((first.x+first.width)>=second.x) && (first.x<=(second.x+second.width));
  }

  bool Collision::check_vertical_collision() const
  {
   return ((first.y+first.height)>=second.y) && (first.y<=(second.y+second.height));
  }

  void Collision::set_target(const EUGENEGDK::BOX first_target,const EUGENEGDK::BOX second_target)
  {
   first=first_target;
   second=second_target;
  }

  bool Collision::check_collision() const
  {
   return this->check_horizontal_collision() && this->check_vertical_collision();
  }

  bool Collision::check_collision(const EUGENEGDK::BOX first_target,const EUGENEGDK::BOX second_target)
  {
   this->set_target(first_target,second_target);
   return this->check_collision();
  }

  Tilemap::Tilemap()
  {
   cell_width=1;
   cell_height=1;
  }

  Tilemap::~Tilemap()
  {

  }

  Tilemap* Tilemap::get_handle()
  {
   return this;
  }

  void Tilemap::initialize(const unsigned int tile_width,const unsigned int tile_height)
  {
   if (tile_width>0)
   {
    cell_width=tile_width;
   }
   if (tile_height>0)
   {
    cell_height=tile_height;
   }

  }

  unsigned int Tilemap::get_tile_width() const
  {
   return cell_width;
  }

  unsigned int Tilemap::get_tile_height() const
  {
   return cell_height;
  }

  unsigned int Tilemap::get_x(const unsigned int row) const
  {
   return row*cell_width;
  }

  unsigned int Tilemap::get_y(const unsigned int column) const
  {
   return column*cell_height;
  }

  unsigned int Tilemap::get_row(const unsigned int x) const
  {
   return x/cell_width;
  }

  unsigned int Tilemap::get_column(const unsigned int y) const
  {
   return y/cell_height;
  }

  unsigned int Tilemap::get_row_amount(const unsigned int viewport_width) const
  {
   unsigned int amount;
   amount=0;
   if (viewport_width>0)
   {
    amount=viewport_width/cell_width;
    if ((viewport_width%cell_width)!=0)
    {
     ++amount;
    }

   }
   return amount;
  }

  unsigned int Tilemap::get_column_amount(const unsigned int viewport_height) const
  {
   unsigned int amount;
   amount=0;
   if (viewport_height>0)
   {
    amount=viewport_height/cell_height;
    if ((viewport_height%cell_height)!=0)
    {
     ++amount;
    }

   }
   return amount;
  }

  unsigned int Tilemap::get_tile_amount(const unsigned int viewport_width,const unsigned int viewport_height) const
  {
   return this->get_row_amount(viewport_width)*this->get_column_amount(viewport_height);
  }

  bool Tilemap::check_row(const unsigned int row,const unsigned int viewport_width) const
  {
   return row<this->get_row_amount(viewport_width);
  }

  bool Tilemap::check_column(const unsigned int column,const unsigned int viewport_height) const
  {
   return column<this->get_column_amount(viewport_height);
  }

  EUGENEGDK::BOX Tilemap::get_box(const unsigned int row,const unsigned int column) const
  {
   EUGENEGDK::BOX collision;
   collision.x=this->get_x(row);
   collision.y=this->get_y(column);
   collision.width=cell_width;
   collision.height=cell_height;
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

  EUGENEGDK::BOX generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height)
  {
   EUGENEGDK::BOX collision;
   collision.x=x;
   collision.y=y;
   collision.width=width;
   collision.height=height;
   return collision;
  }

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