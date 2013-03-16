
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
      
      /*
      // now draw outline box over openGL content
      cairo_t *cr = Fl::cairo_cc();
      
      cairo_save( cr );
      
      // stroke rim
      cairo_set_line_width(cr, 1.9);
      cairo_rectangle(cr, x, y, width, height);
      cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
      cairo_stroke( cr );
      
      cairo_restore( cr );
      */
    }
};

#endif
