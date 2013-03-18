
#ifndef AVTK_OPENGL_H
#define AVTK_OPENGL_H

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

class AvtkOpenGL : public Fl_Gl_Window
{
  public:
    AvtkOpenGL(int _x,int _y,int _w,int _h,const char *l=0):
        Fl_Gl_Window(_x,_y,_w,_h,l)
    {
      overlay_sides = 3;
      offset = 0;
      damage(FL_DAMAGE_ALL);
      
      x = _x;
      y = _y;
      width = _w;
      height = _h;
      
    }
    
    int x, y, width, height;
    
    int overlay_sides;
    float offset;
    
    static void static_update(void* inst)
    {
      AvtkOpenGL* instance = (AvtkOpenGL*)inst;
      instance->offset += 0.07;
      
      Fl::repeat_timeout( 1 / 60.f, &AvtkOpenGL::static_update, inst);
      
      instance->damage(FL_DAMAGE_ALL);
    }
    
    void draw()
    {
      // the valid() property may be used to avoid reinitializing your
      // GL transformation for each redraw:
      if (!valid())
      {
        valid(1);
        glLoadIdentity();
        glViewport(0,0,w(),h());
      }
      
      glClear(GL_COLOR_BUFFER_BIT);
      
      
      glEnable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
      glEnable(GL_POLYGON_SMOOTH);
      
      glBegin(GL_POLYGON);
        double ang = 0*2*M_PI/3 + offset;
        glColor3f(1.0,0.48,0);
        glVertex3f(cos(ang)*0.7,sin(ang)*0.7,0);
      
        ang = 1*2*M_PI/3 + offset;
        glColor3f(0.0,0.6,1.0);
        glVertex3f(cos(ang)*0.7,sin(ang)*0.7,0);
        
        ang = 2*2*M_PI/3 + offset;
        glColor3f(0.1,1.0,0.1);
        glVertex3f(cos(ang)*0.7,sin(ang)*0.7,0);
      glEnd();
      
    }
};

#endif
