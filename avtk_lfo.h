
#include <FL/Fl_Button.H>

class AvtkLFO : public Fl_Slider
{
  public:
    AvtkLFO(int _x, int _y, int _w, int _h, const char *_label =0):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      
      highlight = false;
      mouseOver = false;
    }
    
    bool mouseOver;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        cairo_t *cr = Fl::cairo_cc();
        
        cairo_save( cr );
        

        // WAVEFORM graph
        cairo_rectangle( cr, x, y, w, h );
        cairo_set_source_rgb( cr,28 / 255.f,  28 / 255.f ,  28 / 255.f  );
        cairo_fill(cr);
        
        
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
        
        
      
      // ADSR graph plotting
        float wavetableMod = value();
        float lfoAmp = value();
        float volume = value();
        
        
        
        // Waveform data: WavetableMod
        {
          int drawX = x;
          int drawY = y + h - 2;
          
          cairo_set_line_width(cr, 3.3);
          cairo_rectangle( cr, x, y + h - 2, w * wavetableMod, 1);
          cairo_set_source_rgba( cr, 25 / 255.f, 255 / 255.f ,   0 / 255.f , 0.7 );
          cairo_stroke( cr );
        }
        // Waveform data: Volume
        {
          int drawX = x+w-3;
          int drawY = y;
          
          float volume = 0.7;
          cairo_set_line_width(cr, 2.4);
          cairo_rectangle( cr, drawX, drawY+ h*(1-lfoAmp), 1,  (h*lfoAmp) ); 
          cairo_set_source_rgba( cr, 255 / 255.f, 104 / 255.f ,   0 / 255.f , 1 );
          cairo_stroke( cr );
        }
        
        
        // sinewave (in graph 1)
        int x1 = x;
        int xS = w;
        int y1 = y + 2; // set down a litte
        int yS = h;
        cairo_move_to( cr, x1, y1 + yS / 2 );
        
        int m1x = x1 + xS / 6;
        int m1y = (y1 + yS / 2)   -   53 * lfoAmp;
        
        int m2x = x1 + xS / 3;
        int m2y = m1y;
        
        int endX = x1 + xS / 2;
        int endY = y1 + yS / 2;
        cairo_curve_to( cr, m1x, m1y, m2x, m2y, endX, endY);
        
        int m3x = x1 + 2 * xS / 3;
        int m3y = y1 + yS / 2.f + ((yS + 7) * 0.5 * lfoAmp);
        
        int m4x = x1 + 5 * xS / 6;
        int m4y = m3y;
        
        int end2X = x1 + xS;
        int end2Y = y1 + yS / 2;
        cairo_curve_to( cr, m3x, m3y, m4x, m4y, end2X, end2Y);
        
        cairo_set_source_rgba( cr, 25 / 255.f, 255 / 255.f ,   0 / 255.f , 1 );
        cairo_set_line_width(cr, 1.6);
        cairo_stroke( cr );
        
        
        
        
        // stroke rim
        cairo_set_line_width(cr, 1.4);
        cairo_rectangle(cr, x, y, w, h);
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
        cairo_stroke( cr );
        
        cairo_restore( cr );
        
        draw_label();
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
      switch(event) {
        case FL_PUSH:
          highlight = 1;
          redraw();
          return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t != highlight) {
              highlight = t;
              redraw();
            }
          }
          return 1;
        case FL_ENTER:
          mouseOver = true;
          redraw();
          return 1;
        case FL_LEAVE:
          mouseOver = false;
          redraw();
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