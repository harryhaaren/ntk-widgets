
#include <FL/Fl_Widget.H>
#include <valarray>
#include <string>

class AvtkFilterHighpass : public Fl_Slider
{
  public:
    AvtkFilterHighpass(int _x, int _y, int _w, int _h, const char *_label = 0):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      
      mouseClickedY = 0;
      mouseClicked = false;
      
      active = true;
      highlight = false;
    }
    
    bool active;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    int mouseClickedY;
    bool mouseClicked;
    
    void set_active(bool a)
    {
      active = a;
      redraw();
    }
    
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
          cairo_move_to( cr, x    , y + ((h / 4.f)*i) );
          cairo_line_to( cr, x + w, y + ((h / 4.f)*i) );
        }
        
        
        cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
        cairo_stroke(cr);
        cairo_set_dash ( cr, dashes, 0, 0.0);
        
        
        
        // draw the cutoff line:
          float cutoff = value();
          
          // move to bottom right, draw line to middle right
          cairo_move_to( cr, x + w, y + h );
          cairo_line_to( cr, x + w, y + (h*0.47));
          
          // Curve
          cairo_curve_to( cr, x + w - (w*cutoff)    , y+(h*0.5)  ,   // control point 1
                              x + w - (w*cutoff)    , y+(h * 0.0),   // control point 2
                              x + w - (w*cutoff) -10, y+ h      );   // end of curve 1
          
          
          if ( active )
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.2 );
          else
            cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
          
          cairo_close_path(cr);
          cairo_fill_preserve(cr);
          
          // stroke cutoff line
          cairo_set_line_width(cr, 1.5);
          if ( active )
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.8 );
          else
            cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 1 );
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
        case FL_DRAG:
          {
            if ( Fl::event_state(FL_BUTTON1) )
            {
              if ( mouseClicked == false ) // catch the "click" event
              {
                cout << "mouse clicked!" << endl;
                mouseClickedY = Fl::event_y();
                mouseClicked = true;
              }
              
              float deltaY = mouseClickedY - Fl::event_y();
              
              float val = value();
              val += deltaY / 100.f;
              
              if ( val > 1.0 ) val = 1.0;
              if ( val < 0.0 ) val = 0.0;
              
              //handle_drag( value + deltaY );
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
            do_callback();
          }
          mouseClicked = false;
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
