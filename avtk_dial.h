
#include <FL/Fl_Slider.H>

class AvtkDial : public Fl_Slider
{
  public:
    AvtkDial(int _x, int _y, int _w, int _h, const char* _label=0):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      mouseClickedY = 0;
      mouseClicked = false;
      
      highlight = false;
      label = _label;
    }
    
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    int mouseClickedY;
    bool mouseClicked;
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        cairo_t *cr = Fl::cairo_cc();
        
        cairo_save( cr );
        
        float radius = 15;
        cairo_set_line_width(cr, 1.5);
        cairo_move_to( cr, x+w/2,y+h/2);
        cairo_line_to( cr, x+w/2,y+h/2);
        cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 0 );
        cairo_stroke(cr);
        
        cairo_arc(cr, x+w/2,y+h/2, radius, 2.46, 0.75 );
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1 );
        cairo_stroke(cr);
        
        float angle = 2.46 + ( 4.54 * value() );
        cairo_set_line_width(cr, 1.7);
        cairo_arc(cr, x+w/2,y+h/2, radius-1.8, 2.46, angle );
        cairo_line_to(cr, x+w/2,y+h/2);
        cairo_set_source_rgb(cr, 0.4, 0.4, 0.4 );
        cairo_stroke(cr);
        cairo_arc(cr, x+w/2,y+h/2, radius+1.8, 2.46, angle );
        cairo_line_to(cr, x+w/2,y+h/2);
        cairo_set_source_rgb(cr, 0.4, 0.4, 0.4 );
        cairo_set_line_width(cr, 2.2);
        cairo_set_source_rgba(cr, 1.0, 0.48,   0, 0.8);
        cairo_stroke(cr);
        
        
        cairo_restore( cr );
        
        draw_label();
      }
    }
    
    void resize(int X, int Y, int W, int H)
    {
      Fl_Slider::resize(X,Y,W,H);
      x = X;
      y = Y;
      w = W;
      h = H;
      redraw();
    }
    
    int handle(int event)
    {
      //cout << "handle event type = " << event << " value = " << value() << endl;
      
      //Fl_Slider::handle( event );
      
      switch(event) {
        case FL_PUSH:
          highlight = 1;
          redraw();
          return 1;
        case FL_DRAG:
          {
            if ( Fl::event_state(FL_BUTTON1) )
            {
              if ( mouseClicked == false ) // catch the "click" event
              {
                mouseClickedY = Fl::event_y();
                mouseClicked = true;
              }
              
              float deltaY = mouseClickedY - Fl::event_y();
              
              float val = value();
              val += deltaY / 100.f;
              
              if ( val > 1.0 ) val = 1.0;
              if ( val < 0.0 ) val = 0.0;
              
              set_value( val );
              
              mouseClickedY = Fl::event_y();
              redraw();
              do_callback(); // makes FLTK call "extra" code entered in FLUID
            }
          }
          return 1;
        case FL_RELEASE:
          if (highlight) {
            highlight = 0;
            redraw();
            // never do anything after a callback, as the callback
            // may delete the widget!
          }
          mouseClicked = false;
          return 1;
        default:
          return Fl_Widget::handle(event);
      }
    }
};
