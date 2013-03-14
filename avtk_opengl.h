
#ifndef AVTK_OPENGL_H
#define AVTK_OPENGL_H

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

class AvtkOpenGL : public Fl_Gl_Window
{
  public:
    AvtkOpenGL(int x,int y,int w,int h,const char *l=0):
        Fl_Gl_Window(x,y,w,h,l)
    {
      overlay_sides = 3;
      offset = 0;
      damage(FL_DAMAGE_ALL);
    }
    
    int overlay_sides;
    float offset;
    
    static void static_update(void* inst)
    {
      AvtkOpenGL* instance = (AvtkOpenGL*)inst;
      instance->offset += 0.1;
      
      Fl::repeat_timeout(0.05, &AvtkOpenGL::static_update, inst);
      
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
      // draw an amazing but slow graphic:
      glClear(GL_COLOR_BUFFER_BIT);
      
      glBegin(GL_POLYGON);
      
        double ang = 0*2*M_PI/3 + offset;
        glColor3f(1.0,0.48,0);
        glVertex3f(cos(ang),sin(ang),0);
      
        ang = 1*2*M_PI/3 + offset;
        glColor3f(0.0,0.6,1.0);
        glVertex3f(cos(ang),sin(ang),0);
        
        ang = 2*2*M_PI/3 + offset;
        glColor3f(0.1,1.0,0.1);
        glVertex3f(cos(ang),sin(ang),0);
      
      glEnd();
    }
};

#endif
