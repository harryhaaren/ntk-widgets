/*
 * Author: Harry van Haaren 2013
 *         harryhaaren@gmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */


#ifndef AVTK_COMPRESSOR_H
#define AVTK_COMPRESSOR_H


#include "avtk_helpers.h"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Slider.H>
#include <valarray>
#include <string>

namespace Avtk
{
  
class Compressor : public Fl_Slider
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
    
    Compressor(int _x, int _y, int _w, int _h, const char *_label = 0):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
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
      
      ratioVal = 0;
      makeupGain = 0;
      threshVal = 0;
    }
    
    void threshold(float t) {threshVal  = t; redraw();}
    void makeup   (float m) {makeupGain = m; redraw();}
    void ratio    (float r) {ratioVal   = r; redraw();}
    
    bool active;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    int mouseClickedX;
    int mouseClickedY;
    bool mouseClicked;
    
    float threshVal;
    float makeupGain;
    float ratioVal;
    
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
        // move to bottom left, draw line to middle left
        cairo_move_to( cr, x , y + h );
        
        cairo_line_to( cr, x , y + (h*0.47));
        
        float makeupGainPx = makeupGain * h * 0.5;
        
        float xDist = 0.1 * w;
        float yDist = 0.1 * h;
        
        float xThresh = x + (w * 0.25) + (w*0.5) * threshVal;
        float yThresh = y + (h * 0.25) + (h*0.5)*(1-threshVal);
        
        float startx = xThresh - xDist;
        float starty = yThresh + yDist;
        
        float cp1x = xThresh;
        float cp1y = yThresh - makeupGainPx;
        
        float cp2x = xThresh;
        float cp2y = yThresh - makeupGainPx;
        
        float endx = xThresh + (xDist*1.2);
        float endy = yThresh - (yDist*1.2)*(1-ratioVal) - makeupGainPx;
        
        // normal line (greyed)
        cairo_move_to( cr, x , y + h );
        cairo_line_to( cr, x + w, y );
        cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
        cairo_set_line_width(cr, 1.4);
        cairo_stroke( cr );
        
        cairo_move_to( cr, x , y + h - makeupGainPx );
        cairo_line_to( cr, startx, starty - makeupGainPx );
        
        // draw curve
        cairo_curve_to( cr, cp1x, cp1y, cp2x, cp2y, endx, endy );
        
        cairo_line_to( cr, x + w, y + (h/4)*ratioVal + (h)*(1-threshVal)*(0.5*ratioVal) - makeupGainPx );
        
        cairo_line_to( cr, x + w, y + h );
        cairo_line_to( cr, x , y + h );
        cairo_close_path(cr);
        
        
        /*
        // Curve
        cairo_curve_to( cr, x + w * cutoff    , y+(h*0.5)  ,   // control point 1
                            x + w * cutoff    , y+(h * 0.0),   // control point 2
                            x + w * cutoff +10, y+ h       );  // end of curve 1
        
        cairo_close_path(cr);
        */
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
        cairo_fill_preserve(cr);
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
        cairo_set_line_width(cr, 1.5);
        cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
        cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
        cairo_stroke( cr );
        
        // stroke outline
        cairo_rectangle(cr, x, y, w, h);
        cairo_set_source_rgba( cr,  126 / 255.f,  126 / 255.f ,  126 / 255.f , 0.8 );
        cairo_set_line_width(cr, 1.9);
        cairo_stroke( cr );
        
        if ( !active )
        {
          // big grey X
          cairo_set_line_width(cr, 20.0);
          cairo_set_source_rgba(cr, 0.4,0.4,0.4, 0.7);
          
          cairo_move_to( cr, x + (3 * w / 4.f), y + ( h / 4.f ) );
          cairo_line_to( cr, x + (w / 4.f), y + ( 3 *h / 4.f ) );
          
          cairo_move_to( cr, x + (w / 4.f), y + ( h / 4.f ) );
          cairo_line_to( cr, x + (3 * w / 4.f), y + ( 3 *h / 4.f ) );
          cairo_set_line_cap ( cr, CAIRO_LINE_CAP_BUTT);
          cairo_stroke( cr );
        }
        
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
          if ( Fl::event_button() == FL_RIGHT_MOUSE )
          {
            active = !active;
            redraw();
            do_callback();
          }
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
              float valY = makeupGain;
              valY += deltaY / 100.f;
              
              if ( valX > 1.0 ) valX = 1.0;
              if ( valX < 0.0 ) valX = 0.0;
              
              if ( valY > 1.0 ) valY = 1.0;
              if ( valY < 0.0 ) valY = 0.0;
              
              //handle_drag( value + deltaY );
              set_value( valX );
              makeupGain = valY;
              
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
};

} // Avtk

#endif // AVTK_COMPRESSOR_H
