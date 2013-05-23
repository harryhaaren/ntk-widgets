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


#ifndef AVTK_OSCILLATOR_H
#define AVTK_OSCILLATOR_H

#include <FL/Fl_Slider.H>

namespace Avtk
{

class Oscillator : public Fl_Slider
{
  public:
    Oscillator(int _x, int _y, int _w, int _h, const char *_label =0):
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
        cairo_set_source_rgb( cr, 28 / 255.f,  28 / 255.f ,  28 / 255.f );
        cairo_fill( cr );
        
        // draw guides
        double dashes[2];
        dashes[0] = 2.0;
        dashes[1] = 2.0;
        cairo_set_dash ( cr, dashes, 2, 0.0);
        cairo_set_line_width( cr, 1.0);
        cairo_set_source_rgb ( cr, 0.4,0.4,0.4);
        for ( int i = 0; i < 4; i++ )
        {
          cairo_move_to( cr, x + ((w / 4.f)*i), y );
          cairo_line_to( cr, x + ((w / 4.f)*i), y + h );
        }
        for ( int i = 0; i < 4; i++ )
        {
          cairo_move_to( cr, x    , y + (( h / 4.f)*i) );
          cairo_line_to( cr, x + w, y + (( h / 4.f)*i) );
        }
        cairo_stroke( cr );
        cairo_set_dash ( cr, dashes, 0, 0.0); // disable dashes: 0 dashes
        
        /*
        // Waveform data: WavetableMod
        {
          int drawX = X;
          int drawY = y + 79;
          
          cairo_rectangle(drawX, drawY, 138 * values[WAVETABLE1_POS+num], 2);
          setColour( cr, COLOUR_GREEN_1, 0.9 );
          cairo_stroke();
        }
        
        // Waveform data: Volume
        {
          int drawX = X+135;
          int drawY = Y;
          
          float volume = 0.7;
          cairo_rectangle(drawX, drawY+ 82*(1-values[OSC1_VOL+num]), 2,  (82*values[OSC1_VOL+num]) ); 
          setColour( cr, COLOUR_PURPLE_1, 1.0 );
          cairo_stroke();
        }
        // graph center circle:
        {
          cairo_save();
          cairo_arc(X + Xs/4.f + (Xs/2.f) * values[WAVETABLE1_POS+num],
                  y + h/4.f + (Ys/2.f) * (1-values[OSC1_VOL+num]),
                  7, 0, 6.28 );
          cairo_set_line_width( 2.0 );
          setColour( cr, COLOUR_ORANGE_1, 1.0 );
          cairo_stroke();
          cairo_restore();
        }
        
        // Waveform select boxes
        {
          /*
          int drawX = x + 43;
          int drawY = y - 27;
          int boxXs= 105;
          int boxYs=  20;
          
          for(int i = 0; i < 5; i++)
          {
            cairo_move_to(drawX, drawY+1);
            cairo_line_to(drawX, drawY+boxYs-1);
            drawX += 21;
          }
          setColour( cr, COLOUR_BLUE_1 );
          cairo_stroke();
          * /
        }
        
        // Graph outline
        {
          cairo_rectangle( X, Y, w, h );
          setColour( cr, COLOUR_GREY_1 );
          cairo_set_line_width(1.1);
          cairo_stroke();
        }
        
        // Lower select boxes
        {
          int drawX = x - 11;
          int drawY = y + 91;
          
          // bg
          cairo_rectangle(drawX+1, drawY, 159, 17);
          cairo_set_source_rgb( cr, 28 / 255.f,  28 / 255.f ,  28 / 255.f );
          cairo_fill_preserve();
          
          setColour( cr, COLOUR_BLUE_1 );
          cairo_set_line_width(0.9);
          cairo_stroke();
          
          /*
          drawX += 162 / 4.f;
          for(int i = 0; i < 3; i++)
          {
            cairo_move_to(drawX, drawY+1);
            cairo_line_to(drawX, drawY+17-1);
            drawX += 162 / 4.f;
          }
          setColour( cr, COLOUR_BLUE_1 );
          cairo_stroke();
          * /
        }
        */
        
        // stroke rim
        cairo_rectangle(cr, x, y, w, h);
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
        cairo_stroke( cr );
        
        cairo_restore( cr );
        
        draw_label();
      }// if DAMAGE
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

} // Avtk

#endif // AVTK_OSCILLATOR_H

