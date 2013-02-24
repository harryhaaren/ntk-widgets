
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
    }
    
    int overlay_sides;
    
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
      
      for (int j=0; j< 3; j++) {
        double ang = j*2*M_PI/3;
        glColor3f(float(j)/3,float(j)/3,float(j)/3);
        glVertex3f(cos(ang),sin(ang),0);
      }
      glEnd();
    }
    
    void draw_overlay()
    {
      // the valid() property may be used to avoid reinitializing your
      // GL transformation for each redraw:
      if (!valid())
      {
        valid(1);
        glLoadIdentity();
        glViewport(0,0,w(),h());
      }
      // draw an amazing graphic:
      gl_color(FL_RED);
      glBegin(GL_LINE_LOOP);
      for (int j=0; j<overlay_sides; j++) {
        double ang = j*2*M_PI/overlay_sides;
        glVertex3f(cos(ang),sin(ang),0);
      }
      glEnd();
    }
};

#endif
