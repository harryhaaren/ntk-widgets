
#ifndef AVTK_FILTERGRAPH_H
#define AVTK_FILTERGRAPH_H


#include "avtk_helpers.h"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Slider.H>
#include <valarray>
#include <string>

class AvtkFiltergraph : public Fl_Slider
{
  public:
    enum Type {
      FILTER_LOWPASS = 0,
      FILTER_HIGHPASS,
      FILTER_BANDPASS,
      FILTER_LOWSHELF,
      FILTER_HIGHSHELF,
      //FILTER_NOTCH,
      //FILTER_PEAK,
    };
    
    AvtkFiltergraph(int _x, int _y, int _w, int _h, const char *_label = 0, Type _type = FILTER_LOWPASS):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      graphType = _type;
      
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      
      mouseClickedX = 0;
      mouseClickedY = 0;
      mouseClicked = false;
      
      active = true;
      highlight = false;
      
      gain = 0;
    }
    
    void setType(Type t)
    {
      graphType = t;
      redraw();
    }
    
    Type graphType;
    bool active;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    int mouseClickedX;
    int mouseClickedY;
    bool mouseClicked;
    
    float gain;
    
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
        
        switch( graphType )
        {
          case FILTER_LOWPASS:    drawLowpass(cr);      break;
          case FILTER_HIGHPASS:   drawHighpass(cr);     break;
          case FILTER_BANDPASS:   drawBandpass(cr);     break;
          case FILTER_LOWSHELF:   drawLowshelf(cr);     break;
          case FILTER_HIGHSHELF:  drawHighshelf(cr);    break;
          default:
            cout << "Filtergraph: unknown filter type selected!" << endl;
        }
        
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
                mouseClickedX = Fl::event_x();
                mouseClickedY = Fl::event_y();
                mouseClicked = true;
              }
              
              float deltaX = mouseClickedX - Fl::event_x();
              float deltaY = mouseClickedY - Fl::event_y();
              
              float valX = value();
              valX -= deltaX / 100.f;
              float valY = gain;
              valY -= deltaY / 100.f;
              
              if ( valX > 1.0 ) valX = 1.0;
              if ( valX < 0.0 ) valX = 0.0;
              
              if ( valY > 1.0 ) valY = 1.0;
              if ( valY < 0.0 ) valY = 0.0;
              
              //handle_drag( value + deltaY );
              set_value( valX );
              gain = valY;
              
              mouseClickedX = Fl::event_x();
              mouseClickedY = Fl::event_y();
              redraw();
              do_callback();
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
    
  private:
    void drawLowpass(cairo_t* cr)
    {
      // draw the cutoff line:
        // move to bottom left, draw line to middle left
        cairo_move_to( cr, x , y + h );
        cairo_line_to( cr, x , y + (h*0.47));
        
        float cutoff = 0.1 + value() * 0.8;
        
        // Curve
        cairo_curve_to( cr, x + w * cutoff    , y+(h*0.5)  ,   // control point 1
                            x + w * cutoff    , y+(h * 0.0),   // control point 2
                            x + w * cutoff +10, y+ h       );  // end of curve 1
        
        
        if ( active )
          cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.2 );
        else
          cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
        
        cairo_close_path(cr);
        cairo_fill_preserve(cr);
        
        avtk_stroke_line(cr, true);
        
        cairo_stroke(cr);
    }
        
    void drawHighpass(cairo_t* cr)
    {
      // draw the cutoff line:
      float cutoff = 0.9 - (value()*0.8);
      
      // move to bottom right
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
      
      avtk_stroke_line(cr, true);
    }
    void drawBandpass(cairo_t* cr)
    {
      // draw the cutoff line:
      float cutoff = value();
      
      // move to bottom right
      cairo_move_to( cr, x, y + h );
      
      // spacer amount
      float spc = w/10.f;
      
      int cp1 = x + w*cutoff - 2*spc;
      if (cp1 < x + 2 ) cp1 = x + 2;
      
      int cp2 = x + w*cutoff - 1*spc;
      if (cp2 < x  ) cp2 = x;
      
      cairo_curve_to( cr, cp1         , y + h       ,  // control point 1
                          cp2         , y +(h * 0.3),  // control point 2
                          x + w*cutoff, y+ (h/ 3.5) ); // end of curve 1
      
      cp1 = x + w*cutoff + 1*spc;
      if (cp1 > x + w) cp1 = x + w;
      
      cp2 = x + w*cutoff + 2*spc;
      if (cp2 > x + w - 2) cp2 = x + w - 2;
      
      cairo_curve_to( cr, cp1    , y +(h * 0.3), // control point 1
                          cp2    , y + h       , // control point 2
                          x +  w , y + h      ); // end of curve 1
      
      
      cairo_line_to( cr, x + w, y + h );
      
      if ( active )
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.2 );
      else
        cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
      
      cairo_close_path(cr);
      cairo_fill_preserve(cr);
      
      avtk_stroke_line(cr, true);
    }
    
    
    
    void drawLowshelf(cairo_t* cr)
    {
      //cairo_save( cr );
      
      //cairo_rectangle( cr, x, y, w, h );
      //cairo_clip( cr );
      double dx = x;
      double dy = y;
      double dw = w;
      double dh = h;
      cairo_clip_extents(cr, &dx, &dy, &dw, &dh );
      
      // draw the cutoff line:
      float cutoff = value();
      
      // spacer amount
      float spc = w/10.f;
      
      float yGain = (gain-0.5) * (h / 1.5);
      
      // move to bottom right, middle right, middle cutoff
      cairo_move_to( cr, x + w, y + h );
      cairo_line_to( cr, x + w, y + (h/2.) );
      cairo_line_to( cr, x + (w*cutoff), y + (h/2.) );
      
      int cp1 = x+(w*cutoff)-2*spc;
      int cp2 = x+(w*cutoff)-4*spc;
      int end = x+(w*cutoff)-6*spc;
      
      if ( cp1 < x ) cp1 = x;
      if ( cp2 < x ) cp2 = x;
      if ( end < x ) end = x;
      
      cairo_curve_to( cr, cp1 , y + h/2.         , // control point 1
                          cp2 , y + h/2. + yGain , // control point 2
                          end , y + h/2. + yGain); // end of curve
      
      
      cairo_line_to( cr, x, y + h/2. +yGain );
      cairo_line_to( cr, x, y + h );
      cairo_close_path(cr);
      
      if ( active )
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.2 );
      else
        cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
      
      
      cairo_fill_preserve(cr);
      
      avtk_stroke_line(cr, true);
      
      cairo_reset_clip( cr );
      //cairo_restore( cr );
    }
    
    
    
    void drawHighshelf(cairo_t* cr)
    {
    }
    
    
};

#endif // AVTK_FILTERGRAPH_H
