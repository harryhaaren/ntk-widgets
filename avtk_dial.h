
#include <FL/Fl_Dial.H>

class AvtkDial : public Fl_Dial
{
  public:
    AvtkDial(int _x, int _y, int _w, int _h, const char* _label=0):
        Fl_Dial(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      highlight = false;
      label = _label;
    }
    
    
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        cairo_t *cr = Fl::cairo_cc();
        
        cairo_save( cr );
        
        cairo_set_line_width(cr, 1.5);
        //cairo_rectangle( cr, x+1, y+1, w-2, h-2 );
        
        float radius = 13;
        
        cairo_arc(cr, x+w/2,y+h/2, radius, 2.46, 0.75 );
        
        cairo_set_source_rgb(cr, 0.6, 0.6, 0.6 );
        cairo_stroke(cr);
        
        float angle = 2.46 + ( 4.54 * value() );
        cairo_set_line_width(cr, 1.7);
        cairo_arc(cr, x+w/2,y+h/2, radius-1, 2.46, angle );
        cairo_line_to(cr, x+w/2,y+h/2);
        cairo_stroke(cr);
        cairo_arc(cr, x+w/2,y+h/2, radius+1, 2.46, angle );
        cairo_line_to(cr, x+w/2,y+h/2);
        cairo_stroke(cr);
        
        cairo_set_source_rgba(cr, 1.0, 0.48,   0, 0.8);
        cairo_stroke(cr);
        
        cairo_restore( cr );
        
        draw_label();
      }
    }
    
    void resize(int X, int Y, int W, int H)
    {
      Fl_Dial::resize(X,Y,W,H);
      x = X;
      y = Y;
      w = W;
      h = H;
      redraw();
    }
    
    int handle(int event)
    {
      //cout << "handle event type = " << event << " value = " << value() << endl;
      
      Fl_Dial::handle( event );
      
      switch(event) {
        case FL_PUSH:
          highlight = 1;
          redraw();
          return 1;
        case FL_DRAG:
          {
            //handle_drag( );
            redraw();
          }
          return 1;
        case FL_RELEASE:
          if (highlight) {
            highlight = 0;
            redraw();
            // never do anything after a callback, as the callback
            // may delete the widget!
          }
          return 1;
        default:
          return Fl_Widget::handle(event);
      }
    }
};
