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

#include "eugenegdk.h"

const size_t KEYBOARD=256;
const unsigned char KEY_RELEASE=0;
const unsigned char KEY_PRESS=1;
const unsigned long int JOYSTICK_UPLEFT=31500;
const unsigned long int JOYSTICK_UPRIGHT=4500;
const unsigned long int JOYSTICK_DOWNLEFT=22500;
const unsigned long int JOYSTICK_DOWNRIGHT=13500;

unsigned char Keys[KEYBOARD];
unsigned char Buttons[MOUSE];

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
  case WM_CREATE:
  memset(Keys,KEY_RELEASE,KEYBOARD);
  memset(Buttons,KEY_RELEASE,MOUSE);
  break;
  case WM_LBUTTONDOWN:
  Buttons[MOUSE_LEFT]=KEY_PRESS;
  break;
  case WM_LBUTTONUP:
  Buttons[MOUSE_LEFT]=KEY_RELEASE;
  break;
  case WM_RBUTTONDOWN:
  Buttons[MOUSE_RIGHT]=KEY_PRESS;
  break;
  case WM_RBUTTONUP:
  Buttons[MOUSE_RIGHT]=KEY_RELEASE;
  break;
  case WM_MBUTTONDOWN:
  Buttons[MOUSE_MIDDLE]=KEY_PRESS;
  break;
  case WM_MBUTTONUP:
  Buttons[MOUSE_MIDDLE]=KEY_RELEASE;
  break;
  case WM_KEYDOWN:
  Keys[GETSCANCODE(lParam)]=KEY_PRESS;
  break;
  case WM_KEYUP:
  Keys[GETSCANCODE(lParam)]=KEY_RELEASE;
  break;
 }
 return DefWindowProc(window,Message,wParam,lParam);
}

namespace EUGENEGDK
{

void Halt(const char *message)
{
 puts(message);
 exit(EXIT_FAILURE);
}

 COM_Base::COM_Base()
{
 HRESULT status;
 status=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
 if (status!=S_OK)
 {
  if (status!=S_FALSE)
  {
   Halt("Can't initialize COM");
  }

 }

}

COM_Base::~COM_Base()
{
 CoUninitialize();
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
 }

}

void Synchronization::create_timer()
{
 timer=CreateWaitableTimer(NULL,FALSE,NULL);
 if (timer==NULL)
 {
  Halt("Can't create synchronization timer");
 }

}

void Synchronization::set_timer(const unsigned long int interval)
{
 LARGE_INTEGER start;
 start.QuadPart=0;
 if (SetWaitableTimer(timer,&start,interval,NULL,NULL,FALSE)==FALSE)
 {
  Halt("Can't set timer");
 }

}

void Synchronization::wait_timer()
{
 WaitForSingleObjectEx(timer,INFINITE,TRUE);
}

Engine::Engine()
{
 window_class.lpszClassName=TEXT("SWGF");
 window_class.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
 window_class.cbSize=sizeof(WNDCLASSEX);
 window_class.lpfnWndProc=Process_Message;
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
 }
 if (window!=NULL)
 {
  CloseWindow(window);
 }
 if (window_class.hbrBackground!=NULL)
 {
  DeleteObject(window_class.hbrBackground);
 }
 UnregisterClass(window_class.lpszClassName,window_class.hInstance);
}

void Engine::get_instance()
{
 if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,NULL,&window_class.hInstance)==FALSE)
 {
  Halt("Can't get the application instance");
 }

}

void Engine::set_backgrond_color()
{
 window_class.hbrBackground=CreateSolidBrush(RGB(0,0,0));
 if (window_class.hbrBackground==NULL)
 {
  Halt("Can't set background color");
 }

}

void Engine::load_icon()
{
 window_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
 if (window_class.hIcon==NULL)
 {
  Halt("Can't load the standart program icon");
 }

}

void Engine::load_cursor()
{
 window_class.hCursor=LoadCursor(NULL,IDC_ARROW);
 if (window_class.hCursor==NULL)
 {
  Halt("Can't load the standart cursor");
 }

}

void Engine::register_window_class()
{
 if (!RegisterClassEx(&window_class))
 {
  Halt("Can't register window class");
 }

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
}

void Engine::take_context()
{
 context=GetWindowDC(window);
 if (context==NULL)
 {
  Halt("Can't take window context");
 }

}

void Engine::create_window()
{
 window=CreateWindowEx(WS_EX_APPWINDOW,window_class.lpszClassName,NULL,WS_VISIBLE|WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL,NULL,window_class.hInstance,NULL);
 if (window==NULL)
 {
  Halt("Can't create window");
 }
 SetFocus(window);
}

void Engine::capture_mouse()
{
 RECT border;
 if (GetClientRect(window,&border)==FALSE)
 {
  Halt("Can't capture window");
 }
 if (ClipCursor(&border)==FALSE)
 {
  Halt("Can't capture cursor");
 }

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
   TranslateMessage(&Message);
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

unsigned int Engine::get_width()
{
 return GetSystemMetrics(SM_CXSCREEN);
}

unsigned int Engine::get_height()
{
 return GetSystemMetrics(SM_CYSCREEN);
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
 if (difftime(time(NULL),start)>=1)
 {
  fps=current;
  current=0;
  start=time(NULL);
 }

}

unsigned long int FPS::get_fps() const
{
 return fps;
}

Unicode_Convertor::Unicode_Convertor()
{
 target=NULL;
}

Unicode_Convertor::~Unicode_Convertor()
{
 if (target!=NULL) delete[] target;
}

void Unicode_Convertor::get_memory(const size_t length)
{
 try
 {
  target=new wchar_t[length+1];
 }
 catch (...)
 {
  Halt("Can't allocate memory");
 }

}

void Unicode_Convertor::clear_buffer(const size_t length)
{
 size_t index,stop;
 stop=length+1;
 for (index=0;index<stop;++index)
 {
  target[index]=0;
 }

}

void Unicode_Convertor::create_buffer(const size_t length)
{
 this->get_memory(length);
 this->clear_buffer(length);
}

void Unicode_Convertor::convert_string(const char *source)
{
 size_t index,length;
 length=strlen(source);
 for (index=0;index<length;++index)
 {
  target[index]=btowc(source[index]);
 }

}

wchar_t *Unicode_Convertor::convert(const char *source)
{
 this->create_buffer(strlen(source));
 this->convert_string(source);
 return target;
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
  Halt("Can't change video mode");
 }

}

void Display::get_video_mode()
{
 if (EnumDisplaySettingsEx(NULL,ENUM_CURRENT_SETTINGS,&display,EDS_RAWMODE)==FALSE)
 {
  Halt("Can't get display setting");
 }

}

void Display::check_video_mode()
{
 this->get_video_mode();
 if (display.dmBitsPerPel<16)
 {
  display.dmBitsPerPel=16;
  this->set_video_mode();
 }

}

unsigned long int Display::get_color() const
{
 return display.dmBitsPerPel;
}

WINGL::WINGL()
{
 render=NULL;
 wglSwapIntervalEXT=NULL;
 memset(&setting,0,sizeof(PIXELFORMATDESCRIPTOR));
 setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
 setting.nVersion=1;
}

WINGL::~WINGL()
{
 if (render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }

}

bool WINGL::check_base_setting() const
{
 bool result;
 result=false;
 if (setting.cColorBits==this->get_color())
 {
  if ((setting.dwFlags&PFD_DRAW_TO_WINDOW)&&(setting.dwFlags&PFD_SUPPORT_OPENGL)) result=true;
 }
 return result;
}

bool WINGL::check_advanced_setting() const
{
 bool result;
 result=false;
 if (setting.dwFlags&PFD_DOUBLEBUFFER)
 {
  if ((setting.iPixelType==PFD_TYPE_RGBA)&&(setting.iLayerType==PFD_MAIN_PLANE)) result=true;
 }
 return result;
}

bool WINGL::check_common_setting() const
{
 bool result;
 result=false;
 if (this->check_base_setting()==true)
 {
  result=this->check_advanced_setting();
 }
 return result;
}

bool WINGL::check_acceleration() const
{
 bool result;
 result=false;
 if (!(setting.dwFlags&PFD_GENERIC_FORMAT)&&!(setting.dwFlags&PFD_GENERIC_ACCELERATED))
 {
  result=true;
 }
 else
 {
  if ((setting.dwFlags&PFD_GENERIC_FORMAT)&&(setting.dwFlags&PFD_GENERIC_ACCELERATED)) result=true;
 }
 return result;
}

int WINGL::get_pixel_format()
{
 int index,result;
 result=0;
 for (index=DescribePixelFormat(this->get_context(),1,setting.nSize,&setting);index>0;--index)
 {
  DescribePixelFormat(this->get_context(),index,setting.nSize,&setting);
  if (this->check_common_setting()==true)
  {
   if (this->check_acceleration()==true)
   {
    result=index;
    break;
   }

  }

 }
 return result;
}

void WINGL::set_pixel_format(const int format)
{
 if (format==0)
 {
  Halt("Invalid pixel format");
 }
 if (SetPixelFormat(this->get_context(),format,&setting)==FALSE)
 {
  Halt("Can't set pixel format");
 }

}

void WINGL::create_render_context()
{
 render=wglCreateContext(this->get_context());
 if (render==NULL)
 {
  Halt("Can't create render context");
 }
 wglMakeCurrent(this->get_context(),render);
}

void WINGL::set_render()
{
 this->set_pixel_format(this->get_pixel_format());
 this->create_render_context();
}

void WINGL::disable_vsync()
{
 wglSwapIntervalEXT=reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
 if (wglSwapIntervalEXT!=NULL)
 {
  wglSwapIntervalEXT(0);
 }

}

void WINGL::Swap()
{
 SwapBuffers(this->get_context());
}

Render::Render()
{

}

Render::~Render()
{

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
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_CULL_FACE);
 glEnable(GL_TEXTURE_2D);
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 glDisableClientState(GL_EDGE_FLAG_ARRAY);
 glDisableClientState(GL_INDEX_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
}

void Render::set_common_setting()
{
 glDepthFunc(GL_ALWAYS);
 glDepthMask(GL_TRUE);
 glFrontFace(GL_CCW);
 glCullFace(GL_BACK);
}

void Render::set_perspective()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0,this->get_width(),this->get_height(),0,0,1);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glViewport(0,0,this->get_width(),this->get_height());
}

void Render::create_render()
{
 this->set_render();
 this->set_perfomance_setting();
 this->set_common_setting();
 this->set_perspective();
 this->disable_vsync();
}

void Render::clear_stage()
{
 glClearColor(0,0,0,0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Render::start_render()
{
 this->create_window();
 this->take_context();
 this->capture_mouse();
 this->create_render();
 this->clear_stage();
}

void Render::refresh()
{
 this->Swap();
}

Resizer::Resizer()
{
 image=NULL;
 size_limit=0;
 source_width=0;
 source_height=0;
 target_width=1;
 target_height=1;
}

Resizer::~Resizer()
{
 if (image!=NULL)
 {
  delete[] image;
  image=NULL;
 }

}

size_t Resizer::get_source_offset(const unsigned int x,const unsigned int y) const
{
 return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(source_width);
}

size_t Resizer::get_target_offset(const unsigned int x,const unsigned int y) const
{
 return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(target_width);
}

void Resizer::resize_image(const unsigned int *target)
{
 float x_ratio,y_ratio;
 unsigned int x,y,steps;
 size_t index;
 x=0;
 y=0;
 x_ratio=static_cast<float>(source_width)/static_cast<float>(target_width);
 y_ratio=static_cast<float>(source_height)/static_cast<float>(target_height);
 for (steps=target_width*target_height;steps>0;--steps)
 {
  index=this->get_source_offset(x_ratio*static_cast<float>(x),y_ratio*static_cast<float>(y));
  image[this->get_target_offset(x,y)]=target[index];
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

void Resizer::create_buffer()
{
 size_t length;
 length=static_cast<size_t>(target_width)*static_cast<size_t>(target_height);
 try
 {
  image=new unsigned int[length];
 }
 catch(...)
 {
  Halt("Can't allocate memory for image buffer");
 }
 length*=sizeof(unsigned int);
}

void Resizer::create_buffer(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit)
{
 this->set_setting(width,height,limit);
 this->calculate_size();
 this->correct_size();
 this->create_buffer();
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
 return image;
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
 point[0].u=0;
 point[0].v=1;
 point[1].u=1;
 point[1].v=1;
 point[2].u=1;
 point[2].v=0;
 point[3].u=0;
 point[3].v=0;
 horizontal_mirror=DISABLE_MIRRORING;
 vertical_mirror=DISABLE_MIRRORING;
}

Shape::~Shape()
{

}

float Shape::get_start_offset(const float current,const float total)
{
 return (1.0/total)*(current-1);
}

float Shape::get_end_offset(const float current,const float total)
{
 return (1.0/total)*current;
}

void Shape::reset_data()
{
 vertex[0].x=0;
 vertex[0].y=target_height;
 vertex[1].x=target_width;
 vertex[1].y=target_height;
 vertex[2].x=target_width;
 vertex[2].y=0;
 vertex[3].x=0;
 vertex[3].y=0;
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

unsigned int Shape::get_x() const
{
 return current_x;
}

unsigned int Shape::get_y() const
{
 return current_y;
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

void Shape::set_horizontal_offset(const float current,const float total)
{
 point[0].u=this->get_start_offset(current,total);
 point[0].v=1;
 point[1].u=this->get_end_offset(current,total);
 point[1].v=1;
 point[2].u=this->get_end_offset(current,total);
 point[2].v=0;
 point[3].u=this->get_start_offset(current,total);
 point[3].v=0;
}

void Shape::set_vertical_offset(const float current,const float total)
{
 point[0].u=0;
 point[0].v=this->get_end_offset(current,total);
 point[1].u=1;
 point[1].v=this->get_end_offset(current,total);
 point[2].u=1;
 point[2].v=this->get_start_offset(current,total);
 point[3].u=0;
 point[3].v=this->get_start_offset(current,total);
}

void Shape::set_tile_offset(const float row,const float rows,const float column,const float columns)
{
 point[0].u=this->get_start_offset(row,rows);
 point[0].v=this->get_end_offset(column,columns);
 point[1].u=this->get_end_offset(row,rows);
 point[1].v=this->get_end_offset(column,columns);
 point[2].u=this->get_end_offset(row,rows);
 point[2].v=this->get_start_offset(column,columns);
 point[3].u=this->get_start_offset(row,rows);
 point[3].v=this->get_start_offset(column,columns);
}

void Shape::set_mirror_status(const MIRROR_STATUS horizontal,const MIRROR_STATUS vertical)
{
 horizontal_mirror=horizontal;
 vertical_mirror=vertical;
}

MIRROR_STATUS Shape::get_horizontal_mirror() const
{
 return horizontal_mirror;
}

MIRROR_STATUS Shape::get_vertical_mirror() const
{
 return vertical_mirror;
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
 }

}

unsigned int Rectangle::get_maximum_size() const
{
 int maximum_size;
 glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maximum_size);
 return maximum_size;
}

void Rectangle::create_texture(const unsigned int *buffer)
{
 Resizer resizer;
 resizer.create_buffer(buffer,this->get_total_width(),this->get_total_height(),this->get_maximum_size());
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
  Halt("Can't create the target texture");
 }

}

void Rectangle::load_data()
{
 glVertexPointer(2,GL_INT,0,vertex);
 glTexCoordPointer(2,GL_FLOAT,0,point);
}

void Rectangle::draw_rectangle()
{
 glBindTexture(GL_TEXTURE_2D,texture);
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

void Rectangle::reset_model_setting()
{
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void Rectangle::set_model_setting()
{
 glMatrixMode(GL_MODELVIEW);
 glTranslatef(this->get_x(),this->get_y(),0);
 glMatrixMode(GL_TEXTURE);
 glScalef(this->get_horizontal_mirror(),this->get_vertical_mirror(),0);
}

void Rectangle::enable_transparent()
{
 glEnable(GL_ALPHA_TEST);
 glEnable(GL_BLEND);
 glAlphaFunc(GL_GREATER,0);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void Rectangle::disable_transparent()
{
 glDisable(GL_ALPHA_TEST);
 glDisable(GL_BLEND);
}

void Rectangle::prepare(const unsigned int *buffer)
{
 this->delete_texture();
 this->create_texture(buffer);
 this->check_texture();
}

void Rectangle::draw()
{
 this->reset_data();
 this->load_data();
 this->reset_model_setting();
 this->set_model_setting();
 this->draw_rectangle();
}

Primitive::Primitive()
{

}

Primitive::~Primitive()
{

}

void Primitive::prepare()
{
 glDisable(GL_ALPHA_TEST);
 glDisable(GL_BLEND);
 glPointSize(1);
 glLineStipple(1,0xFF);
 glColor3ub(0,0,0);
}

void Primitive::set_color(const unsigned char red,const unsigned char green,const unsigned char blue)
{
 glColor3ub(red,green,blue);
}

void Primitive::draw_pixel(const unsigned int x,const unsigned int y)
{
 glBegin(GL_POINTS);
 glVertex2i(x,y);
 glEnd();
}

void Primitive::draw_line(const unsigned int x,const unsigned int y,const unsigned int x2,const unsigned int y2)
{
 glBegin(GL_LINES);
 glVertex2i(x,y);
 glVertex2i(x2,y2);
 glEnd();
}

void Primitive::draw_rectangle(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height)
{
 glPolygonMode(GL_FRONT,GL_LINE);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(x,y,0);
 glBegin(GL_QUADS);
 glVertex2i(0,height);
 glVertex2i(width,height);
 glVertex2i(width,0);
 glVertex2i(0,0);
 glEnd();
 glLoadIdentity();
 glPolygonMode(GL_FRONT,GL_FILL);
}

void Primitive::draw_filled_rectangle(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height)
{
 glPolygonMode(GL_FRONT,GL_FILL);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(x,y,0);
 glBegin(GL_QUADS);
 glVertex2i(0,height);
 glVertex2i(width,height);
 glVertex2i(width,0);
 glVertex2i(0,0);
 glEnd();
 glLoadIdentity();
}

Screen::Screen()
{

}

Screen::~Screen()
{

}

void Screen::clear_screen()
{
 this->clear_stage();
}

void Screen::initialize()
{
 this->check_video_mode();
 this->prepare_engine();
 this->start_render();
 this->create_timer();
 this->set_timer(17);
}

bool Screen::update()
{
 this->refresh();
 this->update_counter();
 this->clear_stage();
 return this->process_message();
}

bool Screen::sync()
{
 bool run;
 run=this->update();
 this->wait_timer();
 return run;
}

Screen* Screen::get_handle()
{
 return this;
}

Keyboard::Keyboard()
{
 preversion=NULL;
}

Keyboard::~Keyboard()
{
 if (preversion!=NULL)
 {
  delete[] preversion;
  preversion=NULL;
 }

}

void Keyboard::create_buffer()
{
 try
 {
  preversion=new unsigned char[KEYBOARD];
 }
 catch (...)
 {
  Halt("Can't allocate memory for keyboard state buffer");
 }

}

void Keyboard::clear_buffer()
{
 size_t index;
 for (index=0;index<KEYBOARD;++index)
 {
  preversion[index]=KEY_RELEASE;
 }

}

bool Keyboard::check_state(const unsigned char code,const unsigned char state)
{
 bool result;
 result=false;
 if (Keys[code]==state)
 {
  if (preversion[code]!=state) result=true;
 }
 preversion[code]=Keys[code];
 return result;
}

void Keyboard::initialize()
{
 this->create_buffer();
 this->clear_buffer();
}

bool Keyboard::check_hold(const unsigned char code)
{
 bool result;
 result=false;
 if (Keys[code]==KEY_PRESS) result=true;
 preversion[code]=Keys[code];
 return result;
}

bool Keyboard::check_press(const unsigned char code)
{
 return this->check_state(code,KEY_PRESS);
}

bool Keyboard::check_release(const unsigned char code)
{
 return this->check_state(code,KEY_RELEASE);
}

Mouse::Mouse()
{
 preversion[MOUSE_LEFT]=KEY_RELEASE;
 preversion[MOUSE_RIGHT]=KEY_RELEASE;
 preversion[MOUSE_MIDDLE]=KEY_RELEASE;
 position.x=0;
 position.y=0;
}

Mouse::~Mouse()
{
 while(ShowCursor(TRUE)<1) ;
}

void Mouse::get_position()
{
 if (GetCursorPos(&position)==FALSE)
 {
  Halt("Can't get the mouse cursor position");
 }

}

bool Mouse::check_state(const MOUSE_BUTTON button,const unsigned char state)
{
 bool result;
 result=false;
 if (Buttons[button]==state)
 {
  if (preversion[button]!=state) result=true;
 }
 preversion[button]=Buttons[button];
 return result;
}

void Mouse::show()
{
 while(ShowCursor(TRUE)<1) ;
}

void Mouse::hide()
{
 while(ShowCursor(FALSE)>-2) ;
}

void Mouse::set_position(const unsigned long int x,const unsigned long int y)
{
 if (SetCursorPos(x,y)==FALSE)
 {
  Halt("Can't set the mouse cursor position");
 }

}

unsigned long int Mouse::get_x()
{
 this->get_position();
 return position.x;
}

unsigned long int Mouse::get_y()
{
 this->get_position();
 return position.y;
}

bool Mouse::check_hold(const MOUSE_BUTTON button)
{
 bool result;
 result=false;
 if (Buttons[button]==KEY_PRESS)
 {
  result=true;
 }
 preversion[button]=Buttons[button];
 return result;
}

bool Mouse::check_press(const MOUSE_BUTTON button)
{
 return this->check_state(button,KEY_PRESS);
}

bool Mouse::check_release(const MOUSE_BUTTON button)
{
 return this->check_state(button,KEY_RELEASE);
}

Gamepad::Gamepad()
{
 active=0;
 max_amount=16;
 memset(&configuration,0,sizeof(JOYCAPS));
 memset(&current,0,sizeof(JOYINFOEX));
 memset(&preversion,0,sizeof(JOYINFOEX));
 current.dwSize=sizeof(JOYINFOEX);
 preversion.dwSize=sizeof(JOYINFOEX);
 current.dwFlags=JOY_RETURNALL;
 preversion.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion.dwPOV=JOY_POVCENTERED;
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
 memset(&preversion,0,sizeof(JOYINFOEX));
 current.dwSize=sizeof(JOYINFOEX);
 preversion.dwSize=sizeof(JOYINFOEX);
 current.dwFlags=JOY_RETURNALL;
 preversion.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion.dwPOV=JOY_POVCENTERED;
}

bool Gamepad::check_button(const GAMEPAD_BUTTONS button,const JOYINFOEX &target)
{
 bool result;
 result=false;
 if (target.dwButtons&button) result=true;
 return result;
}

unsigned int Gamepad::get_amount()
{
 return joyGetNumDevs();
}

unsigned int Gamepad::get_button_amount()
{
 unsigned int result;
 result=0;
 if (this->read_configuration()==true) result=configuration.wNumButtons;
 return result;
}

unsigned int Gamepad::get_last_index()
{
 unsigned int last_index;
 last_index=this->get_amount();
 if (last_index>0) --last_index;
 return last_index;
}

bool Gamepad::check_connection()
{
 return this->read_state();
}

void Gamepad::update()
{
 preversion=current;
 if (this->read_state()==false) this->clear_state();
}

unsigned long int Gamepad::get_sticks_amount()
{
 unsigned long int result;
 result=0;
 if (this->read_configuration()==true)
 {
  switch (configuration.wNumAxes)
  {
   case 2:
   result=1;
   break;
   case 4:
   result=2;
   break;
  }

 }
 return result;
}

void Gamepad::set_active(const unsigned int gamepad)
{
 if (active<max_amount)
 {
  this->clear_state();
  active=gamepad;
 }

}

unsigned int Gamepad::get_max_amount() const
{
 return max_amount;
}

unsigned int Gamepad::get_active() const
{
 return active;
}

GAMEPAD_DPAD Gamepad::get_dpad() const
{
 GAMEPAD_DPAD result;
 result=GAMEPAD_NONE;
 switch (current.dwPOV)
 {
  case JOY_POVFORWARD:
  result=GAMEPAD_UP;
  break;
  case JOY_POVBACKWARD:
  result=GAMEPAD_DOWN;
  break;
  case JOY_POVLEFT:
  result=GAMEPAD_LEFT;
  break;
  case JOY_POVRIGHT:
  result=GAMEPAD_RIGHT;
  break;
  case JOYSTICK_UPLEFT:
  result=GAMEPAD_UPLEFT;
  break;
  case JOYSTICK_UPRIGHT:
  result=GAMEPAD_UPRIGHT;
  break;
  case JOYSTICK_DOWNLEFT:
  result=GAMEPAD_DOWNLEFT;
  break;
  case JOYSTICK_DOWNRIGHT:
  result=GAMEPAD_DOWNRIGHT;
  break;
 }
 return result;
}

GAMEPAD_DIRECTION Gamepad::get_stick_x(const GAMEPAD_STICKS stick)
{
 GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=GAMEPAD_NEUTRAL_DIRECTION;
 if (stick==GAMEPAD_LEFT_STICK)
 {
  if (this->get_sticks_amount()>0)
  {
   control=(configuration.wXmax-configuration.wXmin)/2;
   if (current.dwXpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwXpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 if (stick==GAMEPAD_RIGHT_STICK)
 {
  if (this->get_sticks_amount()>1)
  {
   control=(configuration.wZmax-configuration.wZmin)/2;
   if (current.dwZpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwZpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 return result;
}

GAMEPAD_DIRECTION Gamepad::get_stick_y(const GAMEPAD_STICKS stick)
{
 GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=GAMEPAD_NEUTRAL_DIRECTION;
 if (stick==GAMEPAD_LEFT_STICK)
 {
  if (this->get_sticks_amount()>0)
  {
   control=(configuration.wYmax-configuration.wYmin)/2;
   if (current.dwYpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwYpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 if (stick==GAMEPAD_RIGHT_STICK)
 {
  if (this->get_sticks_amount()>1)
  {
   control=(configuration.wRmax-configuration.wRmin)/2;
   if (current.dwRpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwRpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 return result;
}

bool Gamepad::check_hold(const GAMEPAD_BUTTONS button)
{
 return this->check_button(button,current);
}

bool Gamepad::check_press(const GAMEPAD_BUTTONS button)
{
 bool result;
 result=false;
 if (this->check_button(button,current)==true)
 {
  if (this->check_button(button,preversion)==false) result=true;
 }
 return result;
}

bool Gamepad::check_release(const GAMEPAD_BUTTONS button)
{
 bool result;
 result=false;
 if (this->check_button(button,current)==false)
 {
  if (this->check_button(button,preversion)==true) result=true;
 }
 return result;
}

Multimedia::Multimedia()
{
 loader=NULL;
 player=NULL;
 controler=NULL;
 video=NULL;
}

Multimedia::~Multimedia()
{
 if (player!=NULL) player->StopWhenReady();
 if (video!=NULL) video->Release();
 if (controler!=NULL) controler->Release();
 if (player!=NULL) player->Release();
 if (loader!=NULL) loader->Release();
}

void Multimedia::open(const wchar_t *target)
{
 player->StopWhenReady();
 if (loader->RenderFile(target,NULL)!=S_OK)
 {
  Halt("Can't load a multimedia file");
 }
 video->put_FullScreenMode(OATRUE);
}

bool Multimedia::is_play()
{
 bool result;
 long long current,total;
 result=false;
 if (controler->GetPositions(&current,&total)==S_OK)
 {
  if (current<total) result=true;
 }
 return result;
}

void Multimedia::rewind()
{
 long long position;
 position=0;
 if (controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning)!=S_OK)
 {
  Halt("Can't set start position");
 }

}

void Multimedia::create_loader()
{
 if (CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,reinterpret_cast<void**>(&loader))!=S_OK)
 {
  Halt("Can't create a multimedia loader");
 }

}

void Multimedia::create_player()
{
 if (loader->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&player))!=S_OK)
 {
  Halt("Can't create a multimedia player");
 }

}

void Multimedia::create_controler()
{
 if (loader->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&controler))!=S_OK)
 {
  Halt("Can't create a player controler");
 }

}

void Multimedia::create_video_player()
{
 if (loader->QueryInterface(IID_IVideoWindow,reinterpret_cast<void**>(&video))!=S_OK)
 {
  Halt("Can't create a video player");
 }

}

void Multimedia::initialize()
{
 this->create_loader();
 this->create_player();
 this->create_controler();
 this->create_video_player();
}

void Multimedia::load(const char *target)
{
 Unicode_Convertor convertor;
 this->open(convertor.convert(target));
}

bool Multimedia::check_playing()
{
 OAFilterState state;
 bool result;
 result=false;
 if (player->GetState(INFINITE,&state)!=E_FAIL)
 {
  if (state==State_Running) result=this->is_play();
 }
 return result;
}

void Multimedia::stop()
{
 player->StopWhenReady();
}

void Multimedia::play()
{
 this->stop();
 this->rewind();
 player->Run();
}

System::System()
{
 srand(UINT_MAX);
}

System::~System()
{

}

unsigned int System::get_random(const unsigned int number)
{
 return rand()%number;
}

void System::quit()
{
 exit(EXIT_SUCCESS);
}

void System::run(const char *command)
{
 system(command);
}

char* System::read_environment(const char *variable)
{
 return getenv(variable);
}

void System::enable_logging(const char *name)
{
 if (freopen(name,"wt",stdout)==NULL)
 {
  Halt("Can't create log file");
 }

}

Filesystem::Filesystem()
{
 status=false;
}

Filesystem::~Filesystem()
{

}

void Filesystem::file_exist(const char *name)
{
 FILE *target;
 status=false;
 target=fopen(name,"rb");
 if (target!=NULL)
 {
  status=true;
  fclose(target);
 }

}

void Filesystem::delete_file(const char *name)
{
 status=(remove(name)==0);
}

bool Filesystem::get_status() const
{
 return status;
}

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

void Binary_File::open_file(const char *name,const char *mode)
{
 target=fopen(name,mode);
 if (target==NULL)
 {
  Halt("Can't open the binary file");
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
 fseek(target,offset,SEEK_SET);
}

long int Binary_File::get_position()
{
 return ftell(target);
}

long int Binary_File::get_length()
{
 long int result;
 fseek(target,0,SEEK_END);
 result=ftell(target);
 rewind(target);
 return result;
}

bool Binary_File::check_error()
{
 return ferror(target)!=0;
}

Input_File::Input_File()
{

}

Input_File::~Input_File()
{

}

void Input_File::open(const char *name)
{
 this->close();
 this->open_file(name,"rb");
}

void Input_File::read(void *buffer,const size_t length)
{
 fread(buffer,sizeof(char),length,target);
}

Output_File::Output_File()
{

}

Output_File::~Output_File()
{

}

void Output_File::open(const char *name)
{
 this->close();
 this->open_file(name,"wb");
}

void Output_File::create_temp()
{
 this->close();
 target=tmpfile();
 if (target==NULL)
 {
  Halt("Can't create a temporary file");
 }

}

void Output_File::write(void *buffer,const size_t length)
{
 fwrite(buffer,sizeof(char),length,target);
}

void Output_File::flush()
{
 fflush(target);
}

Image::Image()
{
 width=0;
 height=0;
 data=NULL;
}

Image::~Image()
{
 if (data!=NULL)
 {
  delete[] data;
  data=NULL;
 }

}

unsigned char *Image::create_buffer(const size_t length)
{
 unsigned char *result;
 result=NULL;
 try
 {
  result=new unsigned char[length];
 }
 catch (...)
 {
  Halt("Can't allocate memory for image buffer");
 }
 return result;
}

void Image::clear_buffer()
{
 if (data!=NULL)
 {
  delete[] data;
  data=NULL;
 }

}

void Image::load_tga(const char *name)
{
 Input_File target;
 size_t index,position,amount,compressed_length,uncompressed_length;
 unsigned char *compressed;
 unsigned char *uncompressed;
 TGA_head head;
 TGA_map color_map;
 TGA_image image;
 this->clear_buffer();
 target.open(name);
 compressed_length=static_cast<size_t>(target.get_length()-18);
 target.read(&head,3);
 target.read(&color_map,5);
 target.read(&image,10);
 if (image.color!=32)
 {
  Halt("Invalid image format");
 }
 if (head.type!=2)
 {
  if (head.type!=10)
  {
   Halt("Invalid image format");
  }

 }
 index=0;
 position=0;
 width=image.width;
 height=image.height;
 uncompressed_length=this->get_length();
 uncompressed=this->create_buffer(uncompressed_length);
 if (head.type==2)
 {
  target.read(uncompressed,uncompressed_length);
 }
 if (head.type==10)
 {
  compressed=this->create_buffer(compressed_length);
  target.read(compressed,compressed_length);
  while(index<uncompressed_length)
  {
   if (compressed[position]<128)
   {
    amount=compressed[position]+1;
    amount*=sizeof(unsigned int);
    memmove(uncompressed+index,compressed+(position+1),amount);
    index+=amount;
    position+=1+amount;
   }
   else
   {
    for (amount=compressed[position]-127;amount>0;--amount)
    {
     memmove(uncompressed+index,compressed+(position+1),sizeof(unsigned int));
     index+=sizeof(unsigned int);
    }
    position+=1+sizeof(unsigned int);
   }

  }
  delete[] compressed;
 }
 target.close();
 data=uncompressed;
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
 return static_cast<size_t>(width)*static_cast<size_t>(height)*sizeof(unsigned int);
}

unsigned char *Image::get_data()
{
 return data;
}

void Image::destroy_image()
{
 width=0;
 height=0;
 this->clear_buffer();
}

Picture::Picture()
{
 image_width=0;
 image_height=0;
 length=0;
 image=NULL;
}

Picture::~Picture()
{
 if (image!=NULL)
 {
  delete[] image;
  image=NULL;
 }

}

void Picture::set_image_size(const unsigned int width,const unsigned int height)
{
 image_width=width;
 image_height=height;
}

void Picture::clear_buffer()
{
 if (image!=NULL)
 {
  delete[] image;
  image_width=0;
  image_height=0;
  length=0;
  image=NULL;
 }

}

unsigned int *Picture::create_buffer()
{
 unsigned int *result;
 result=NULL;
 length=static_cast<size_t>(image_width)*static_cast<size_t>(image_height);
 try
 {
  result=new unsigned int[length];
 }
 catch(...)
 {
  Halt("Can't allocate memory for image buffer");
 }
 length*=sizeof(unsigned int);
 return result;
}

void Picture::set_buffer(unsigned int *buffer)
{
 image=buffer;
}

unsigned int *Picture::get_buffer()
{
 return image;
}

size_t Picture::get_length() const
{
 return length;
}

unsigned int *Picture::get_image()
{
 return image;
}

void Picture::load_image(Image &buffer)
{
 this->clear_buffer();
 this->set_image_size(buffer.get_width(),buffer.get_height());
 image=this->create_buffer();
 memmove(image,buffer.get_data(),buffer.get_length());
}

unsigned int Picture::get_image_width() const
{
 return image_width;
}

unsigned int Picture::get_image_height() const
{
 return image_height;
}

Frame::Frame()
{
 frame=1;
 frames=1;
}

Frame::~Frame()
{

}

void Frame::set_frame(const unsigned int target)
{
 if (target>0)
 {
  if (target<=frames) frame=target;
 }

}

void Frame::increase_frame()
{
 ++frame;
 if (frame>frames)
 {
  frame=1;
 }

}

void Frame::set_frames(const unsigned int amount)
{
 if (amount>1) frames=amount;
}

unsigned int Frame::get_frames() const
{
 return frames;
}

unsigned int Frame::get_frame() const
{
 return frame;
}

Background::Background()
{
 target.set_size(0,0);
 current_kind=NORMAL_BACKGROUND;
}

Background::~Background()
{

}

void Background::configure_background()
{
 switch(current_kind)
 {
  case NORMAL_BACKGROUND:
  target.set_horizontal_offset(1,1);
  break;
  case HORIZONTAL_BACKGROUND:
  target.set_horizontal_offset(this->get_frame(),this->get_frames());
  break;
  case VERTICAL_BACKGROUND:
  target.set_vertical_offset(this->get_frame(),this->get_frames());
  break;
 }

}

void Background::prepare(const unsigned int screen_width,const unsigned int screen_height)
{
 target.set_size(screen_width,screen_height);
 target.set_total_size(this->get_image_width(),this->get_image_height());
 target.prepare(this->get_buffer());
 target.set_position(0,0);
}

void Background::prepare(Screen *screen)
{
 this->prepare(screen->get_width(),screen->get_height());
}

void Background::set_kind(const BACKGROUND_TYPE kind)
{
 current_kind=kind;
 this->configure_background();
}

void Background::set_setting(const BACKGROUND_TYPE kind,const unsigned int frames)
{
 if (kind!=NORMAL_BACKGROUND) this->set_frames(frames);
 this->set_kind(kind);
}

void Background::set_target(const unsigned int target)
{
 this->set_frame(target);
 this->set_kind(current_kind);
}

void Background::step()
{
 this->increase_frame();
 this->set_kind(current_kind);
}

void Background::horizontal_mirror()
{
 if (target.get_horizontal_mirror()==DISABLE_MIRRORING)
 {
  target.set_mirror_status(ENABLE_MIRRORING,target.get_vertical_mirror());
 }
 else
 {
  target.set_mirror_status(DISABLE_MIRRORING,target.get_vertical_mirror());
 }

}

void Background::vertical_mirror()
{
 if (target.get_vertical_mirror()==DISABLE_MIRRORING)
 {
  target.set_mirror_status(target.get_horizontal_mirror(),ENABLE_MIRRORING);
 }
 else
 {
  target.set_mirror_status(target.get_horizontal_mirror(),DISABLE_MIRRORING);
 }

}

void Background::draw_background()
{
 target.disable_transparent();
 target.draw();
}

Sprite::Sprite()
{
 transparent=true;
 current_x=0;
 current_y=0;
 sprite_width=0;
 sprite_height=0;
 current_kind=SINGLE_SPRITE;
}

Sprite::~Sprite()
{

}

void Sprite::check_transparent()
{
 if (transparent==true)
 {
  target.enable_transparent();
 }
 else
 {
  target.disable_transparent();
 }

}

void Sprite::draw_sprite_image()
{
 target.set_size(sprite_width,sprite_height);
 target.set_position(current_x,current_y);
 target.draw();
}

void Sprite::configure_sprite()
{
 switch(current_kind)
 {
  case SINGLE_SPRITE:
  sprite_width=this->get_image_width();
  sprite_height=this->get_image_height();
  break;
  case HORIZONTAL_STRIP:
  sprite_width=this->get_image_width()/this->get_frames();
  sprite_height=this->get_image_height();
  break;
  case VERTICAL_STRIP:
  sprite_width=this->get_image_width();
  sprite_height=this->get_image_height()/this->get_frames();
  break;
 }

}

void Sprite::set_sprite_frame()
{
 switch(current_kind)
 {
  case SINGLE_SPRITE:
  target.set_horizontal_offset(1,1);
  break;
  case HORIZONTAL_STRIP:
  target.set_horizontal_offset(this->get_frame(),this->get_frames());
  break;
  case VERTICAL_STRIP:
  target.set_vertical_offset(this->get_frame(),this->get_frames());
  break;
 }

}

void Sprite::prepare()
{
 target.set_total_size(this->get_image_width(),this->get_image_height());
 target.prepare(this->get_buffer());
}

void Sprite::load_sprite(Image &buffer,const SPRITE_TYPE kind,const unsigned int frames)
{
 this->load_image(buffer);
 if (kind!=SINGLE_SPRITE) this->set_frames(frames);
 this->set_kind(kind);
}

void Sprite::set_transparent(const bool enabled)
{
 transparent=enabled;
}

bool Sprite::get_transparent() const
{
 return transparent;
}

void Sprite::set_x(const unsigned int x)
{
 current_x=x;
}

void Sprite::set_y(const unsigned int y)
{
 current_y=y;
}

void Sprite::increase_x()
{
 ++current_x;
}

void Sprite::decrease_x()
{
 --current_x;
}

void Sprite::increase_y()
{
 ++current_y;
}

void Sprite::decrease_y()
{
 --current_y;
}

void Sprite::increase_x(const unsigned int increment)
{
 current_x+=increment;
}

void Sprite::decrease_x(const unsigned int decrement)
{
 current_x-=decrement;
}

void Sprite::increase_y(const unsigned int increment)
{
 current_y+=increment;
}

void Sprite::decrease_y(const unsigned int decrement)
{
 current_y-=decrement;
}

unsigned int Sprite::get_x() const
{
 return current_x;
}

unsigned int Sprite::get_y() const
{
 return current_y;
}

unsigned int Sprite::get_width() const
{
 return sprite_width;
}

unsigned int Sprite::get_height() const
{
 return sprite_height;
}

Sprite* Sprite::get_handle()
{
 return this;
}

Collision_Box Sprite::get_box() const
{
 Collision_Box target;
 target.x=current_x;
 target.y=current_y;
 target.width=sprite_width;
 target.height=sprite_height;
 return target;
}

void Sprite::set_kind(const SPRITE_TYPE kind)
{
 current_kind=kind;
 this->configure_sprite();
 this->set_sprite_frame();
}

SPRITE_TYPE Sprite::get_kind() const
{
 return current_kind;
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

void Sprite::set_position(const unsigned int x,const unsigned int y)
{
 current_x=x;
 current_y=y;
}

void Sprite::set_width(const unsigned int width)
{
 if (width>0)
 {
  sprite_width=width;
 }

}

void Sprite::set_height(const unsigned int height)
{
 if (height>0)
 {
  sprite_height=height;
 }

}

void Sprite::set_size(const unsigned int width,const unsigned int height)
{
 this->set_width(width);
 this->set_height(height);
}

void Sprite::clone(Sprite &target)
{
 this->clear_buffer();
 this->set_image_size(target.get_image_width(),target.get_image_height());
 this->set_frames(target.get_frames());
 this->set_kind(target.get_kind());
 this->set_transparent(target.get_transparent());
 this->set_buffer(this->create_buffer());
 memmove(this->get_image(),target.get_image(),target.get_length());
 this->set_size(target.get_width(),target.get_height());
}

void Sprite::horizontal_mirror()
{
 if (target.get_horizontal_mirror()==DISABLE_MIRRORING)
 {
  target.set_mirror_status(ENABLE_MIRRORING,target.get_vertical_mirror());
 }
 else
 {
  target.set_mirror_status(DISABLE_MIRRORING,target.get_vertical_mirror());
 }

}

void Sprite::vertical_mirror()
{
 if (target.get_vertical_mirror()==DISABLE_MIRRORING)
 {
  target.set_mirror_status(target.get_horizontal_mirror(),ENABLE_MIRRORING);
 }
 else
 {
  target.set_mirror_status(target.get_horizontal_mirror(),DISABLE_MIRRORING);
 }

}

void Sprite::draw_sprite()
{
 this->check_transparent();
 this->draw_sprite_image();
}

void Sprite::draw_sprite(const unsigned int x,const unsigned int y)
{
 this->set_position(x,y);
 this->draw_sprite();
}

void Sprite::draw_sprite(const bool transparency)
{
 this->set_transparent(transparency);
 this->draw_sprite();
}

void Sprite::draw_sprite(const bool transparency,const unsigned int x,const unsigned int y)
{
 this->set_transparent(transparency);
 this->draw_sprite(x,y);
}

Tileset::Tileset()
{
 target.set_size(0,0);
 rows=0;
 columns=0;
 tile_width=0;
 tile_height=0;
}

Tileset::~Tileset()
{

}

void Tileset::prepare()
{
 target.set_total_size(this->get_image_width(),this->get_image_height());
 target.prepare(this->get_buffer());
}

unsigned int Tileset::get_tile_width() const
{
 return tile_width;
}

unsigned int Tileset::get_tile_height() const
{
 return tile_height;
}

unsigned int Tileset::get_rows() const
{
 return rows;
}

unsigned int Tileset::get_columns() const
{
 return columns;
}

void Tileset::set_size(const unsigned int width,const unsigned int height)
{
 if (width>0)
 {
  tile_width=width;
 }
 if (height>0)
 {
  tile_height=height;
 }

}

void Tileset::select_tile(const unsigned int row,const unsigned int column)
{
 if ((row<rows)&&(column<columns))
 {
  target.set_tile_offset(row,rows,column,columns);
 }

}

void Tileset::draw_tile(const unsigned int x,const unsigned int y)
{
 target.set_size(tile_width,tile_height);
 target.set_position(x,y);
 target.disable_transparent();
 target.draw();
}

void Tileset::draw_tile(const unsigned int row,const unsigned int column,const unsigned int x,const unsigned int y)
{
 this->select_tile(row,column);
 this->draw_tile(x,y);
}

void Tileset::load_tileset(Image &buffer,const unsigned int row_amount,const unsigned int column_amount)
{
 if ((row_amount>0)&&(column_amount>0))
 {
  this->load_image(buffer);
  rows=row_amount;
  columns=column_amount;
  tile_width=this->get_image_width()/rows;
  tile_height=this->get_image_height()/columns;
 }

}

Text::Text()
{
 current_x=0;
 current_y=0;
 font=NULL;
}

Text::~Text()
{

}

void Text::increase_position()
{
 font->increase_x(font->get_width());
}

void Text::restore_position()
{
 font->set_position(current_x,current_y);
}

void Text::set_position(const unsigned int x,const unsigned int y)
{
 font->set_position(x,y);
 current_x=x;
 current_y=y;
}

void Text::load_font(Sprite *target)
{
 font=target;
 font->set_frames(256);
 font->set_kind(HORIZONTAL_STRIP);
}

void Text::draw_character(const char target)
{
 font->set_target(static_cast<unsigned char>(target)+1);
 font->draw_sprite();
}

void Text::draw_text(const char *text)
{
 size_t index,length;
 length=strlen(text);
 this->restore_position();
 for (index=0;index<length;++index)
 {
  this->draw_character(text[index]);
  this->increase_position();
 }
 this->restore_position();
}

void Text::draw_character(const unsigned int x,const unsigned int y,const char target)
{
 this->set_position(x,y);
 this->draw_character(target);
}

void Text::draw_text(const unsigned int x,const unsigned int y,const char *text)
{
 this->set_position(x,y);
 this->draw_text(text);
}

Timer::Timer()
{
 interval=0;
 start=time(NULL);
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
 bool result;
 result=false;
 if (difftime(time(NULL),start)>=interval)
 {
  result=true;
  start=time(NULL);
 }
 return result;
}

Collision::Collision()
{
 first.x=0;
 first.y=0;
 first.width=0;
 first.height=0;
 second.x=0;
 second.y=0;
 second.width=0;
 second.height=0;
}

Collision::~Collision()
{

}

void Collision::set_target(const Collision_Box &first_target,const Collision_Box &second_target)
{
 first=first_target;
 second=second_target;
}

bool Collision::check_horizontal_collision() const
{
 bool result;
 result=false;
 if ((first.x+first.width)>=second.x)
 {
  if (first.x<=(second.x+second.width)) result=true;
 }
 return result;
}

bool Collision::check_vertical_collision() const
{
 bool result;
 result=false;
 if ((first.y+first.height)>=second.y)
 {
  if (first.y<=(second.y+second.height)) result=true;
 }
 return result;
}

bool Collision::check_collision() const
{
 return this->check_horizontal_collision() || this->check_vertical_collision();
}

bool Collision::check_horizontal_collision(const Collision_Box &first_target,const Collision_Box &second_target)
{
 this->set_target(first_target,second_target);
 return this->check_horizontal_collision();
}

bool Collision::check_vertical_collision(const Collision_Box &first_target,const Collision_Box &second_target)
{
 this->set_target(first_target,second_target);
 return this->check_vertical_collision();
}

bool Collision::check_collision(const Collision_Box &first_target,const Collision_Box &second_target)
{
 this->set_target(first_target,second_target);
 return this->check_collision();
}

Collision_Box Collision::generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height) const
{
 Collision_Box result;
 result.x=x;
 result.y=y;
 result.width=width;
 result.height=height;
 return result;
}

}