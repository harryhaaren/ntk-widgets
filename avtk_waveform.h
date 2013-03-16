
#ifndef AVTK_WAVEFORM_H
#define AVTK_WAVEFORM_H

#include <FL/Fl_Widget.H>
#include <vector>
#include <string>
#include <cairomm/context.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace Cairo;

class AvtkWaveform : public Fl_Widget
{
  public:
    AvtkWaveform(int _x, int _y, int _w, int _h, const char *_label=0 ):
        Fl_Widget(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      
      highlight = false;
      newWaveform = false;
      
      waveformCr = 0;
      waveformSurf = 0;
      
      data = new std::vector<float>();
      
      srand (time(NULL));
      
      for (int i = 0; i < 700; i++)
      {
        data->push_back( rand() / (float(RAND_MAX)*3) );
      }
      
      newWaveform = true;
      
    }
    
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    cairo_t*          waveformCr;
    cairo_surface_t*  waveformSurf;
    
    bool newWaveform;
    
    std::vector<float>* data;
    
    void setDataPtr( std::vector<float>* d )
    {
      //cout << "AvtkWaveform: setDataPtr = " << data << endl;
      data = d;
    }
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        cairo_t *cr = Fl::cairo_cc();
        
        //Cairo::Context cr = Cairo::Context( c_cr );
        //Cairo::Context cairoContext = Context( c_cr, false );
        //Cairo::Context* cr = &cairoContext;
        
        cairo_save(cr);
        
        
        if ( not data )
        {
          // draw X
          cairo_move_to( cr,  x    , y     );
          cairo_line_to( cr,  x + w, y + h );
          cairo_move_to( cr,  x    , y + h );
          cairo_line_to( cr,  x + w, y     );
          cairo_set_source_rgb ( cr, 0.2,0.2,0.2);
          cairo_stroke(cr);
          
          // draw text
          cairo_move_to( cr,  x + (w/2.f) - 65, y + (h/2.f) + 10 );
          cairo_set_source_rgb ( cr, 0.6,0.6,0.6);
          cairo_set_font_size( cr, 20 );
          cairo_show_text( cr, "No data loaded" );
          
          return;
        }
        
        //cout << "waveform have data" << endl;
        
        if ( newWaveform )
        {
          if ( !waveformCr )
          {
            // create the waveform surface and context
            //cout << "waveform draw() creating new objects" << endl;
            waveformSurf= cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, w, h);
            waveformCr  = cairo_create ( waveformSurf );
            //cout << "waveform draw() creating objects done" << endl;
          }
          
          
          // clear the surface
          cairo_rectangle(waveformCr, 0, 0, w, h);
          cairo_set_source_rgb (waveformCr, 0.1,0.1,0.1);
          cairo_fill( waveformCr );
          
          // don't draw every sample
          int sampleCountForDrawing = -1;
          
          float currentTop = 0.f;
          float previousTop = 0.f;
          float currentSample = 0.f;
          
          // find how many samples per pixel
          int samplesPerPix = data->size() / w;
          //cout << "width = " << w << "  sampsPerPx " << samplesPerPix << endl;
          
          // loop over each pixel value we need
          for( int p = 0; p < w; p++ )
          {
            float average = 0.f;
            
            // calc value for this pixel
            for( int i = 0; i < samplesPerPix; i++ )
            {
              float tmp = data->at(i + (p * samplesPerPix) );
              if ( tmp < 0 )
              {
                tmp = -tmp;
              }
              average += tmp;
            }
            average =(average / samplesPerPix);
            
            cairo_move_to( waveformCr, p, (h/2) - (average * h )  );
            cairo_line_to( waveformCr, p, (h/2) + (average * h )  );
          }
          
          // stroke the waveform
          cairo_set_source_rgb( waveformCr, 1.0,1.0,1.0);
          cairo_stroke( waveformCr );
          
          newWaveform = false;
        }
        
        cairo_set_source_surface(cr, waveformSurf, x, y);
        cairo_rectangle( cr, x, y, w, h);
        cairo_paint(cr);
        
        // stroke rim
        cairo_set_line_width(cr, 0.9);
        cairo_rectangle(cr, x, y, w, h);
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
        cairo_stroke( cr );
        
        //cout << "waveform draw() done" << endl;
        
        cairo_restore(cr);
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

#endif

