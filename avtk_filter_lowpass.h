
#include <FL/Fl_Widget.H>
#include <valarray>
#include <string>

class AvtkFilterLowpass : public Fl_Slider
{
  public:
    AvtkFilterLowpass(int _x, int _y, int _w, int _h, const char *_label):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      
      active = true;
      highlight = false;
    }
    
    bool active;
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
        
        
        // fill background
        cairo_rectangle( cr, x, y, w, h);
        cairo_set_source_rgb( cr, 28 / 255.f,  28 / 255.f ,  28 / 255.f  );
        cairo_fill( cr );
        
        
        // set up dashed lines, 1 px off, 1 px on
        double dashes[1];
        dashes[0] = 2.0;
        
        cairo_set_dash ( cr, dashes, 1, 0.0);
        cairo_set_line_width( cr, 1.0);
        
        // loop over each 2nd line, drawing dots
        cairo_set_line_width(cr, 1.0);
        cairo_set_source_rgb(cr, 0.4,0.4,0.4);
        for ( int i = 0; i < 4; i++ )
        {
          cairo_move_to( cr, x + ((w / 4.f)*i), y );
          cairo_line_to( cr, x + ((w / 4.f)*i), y + h );
        }
        for ( int i = 0; i < 4; i++ )
        {
          cairo_move_to( cr, x    , y + ((w / 4.f)*i) );
          cairo_line_to( cr, x + w, y + ((w / 4.f)*i) );
        }
        
        
        cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
        cairo_stroke(cr);
        cairo_set_dash ( cr, dashes, 0, 0.0);
        
        
        
        // draw the cutoff line:
          // move to bottom left, draw line to middle left
          cairo_move_to( cr, x , y + h );
          cairo_line_to( cr, x , y + (h*0.47));
          
          float cutoff = value();
          
          // Curve
          cairo_curve_to( cr, x + w * cutoff    , y+(h*0.5)  ,   // control point 1
                              x + w * cutoff    , y+(h * 0.0),   // control point 2
                              x + w * cutoff +10, y+ h       );  // end of curve 1
          
          
          if ( active )
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.2 );
          else
            cairo_set_source_rgba( cr,  28 / 255.f,  28 / 255.f ,  28 / 255.f , 0.2 );
          
          cairo_close_path(cr);
          cairo_fill_preserve(cr);
          
          // stroke cutoff line
          cairo_set_line_width(cr, 1.5);
          if ( active )
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
          else
            cairo_set_source_rgba( cr,  28 / 255.f,  28 / 255.f ,  28 / 255.f , 1 );
          cairo_stroke(cr);
        
        
        // stroke rim
        cairo_rectangle(cr, x, y, w, h);
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
        cairo_stroke( cr );
        
        
        cairo_restore( cr );
      }
    }
    
    void resize(int X, int Y, int W, int H)
    {
      Fl_Widget::resize(X,Y,W,H);
      x = X;
      y = Y;
      w = W;
      h = H;
      redraw();
    }
    
    int handle(int event)
    {
      return 0;
      
      switch(event)
      {
        case FL_PUSH:
          highlight = 0;
          redraw();
          return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t != highlight) {
              redraw();
            }
          }
          return 1;
        case FL_RELEASE:
          if (highlight) {
            highlight = 0;
            redraw();
            do_callback();
          }
          return 1;
        case FL_SHORTCUT:
          if ( test_shortcut() )
          {
            do_callback();
            return 1;
          }
          return 0;
        default:
          return Fl_Widget::handle(event);
      }
    }
};
