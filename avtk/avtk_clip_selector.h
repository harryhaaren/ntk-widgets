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


#ifndef AVTK_CLIP_SELECTOR_H
#define AVTK_CLIP_SELECTOR_H

#include <FL/Fl_Button.H>

#include <string>

#include "../gridlogic.hxx"
#include "../gclipselectoraction.hxx"

#include "../worker.hxx"
#include "../looper.hxx"
#include "../audiobuffer.hxx"
#include "../eventhandler.hxx"


extern void luppp_tooltip(std::string s);

namespace Avtk
{

class ClipState
{
  public:
    ClipState() :
      state(GridLogic::STATE_EMPTY),
      name("")
    {}
    
    void setName(std::string n = "---")
    {
      name = n;
    }
    
    void setState(GridLogic::State s)
    {
      state = s;
    }
    
    std::string getName()
    {
      return name;
    }
    
    GridLogic::State getState()
    {
      return state;
    }
  
  private:
    GridLogic::State state;
    std::string name;
};

class ClipSelector : public Fl_Button
{
  public:
    
    
    ClipSelector(int _x, int _y, int _w, int _h, const char *_label, bool master = false):
        Fl_Button(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      _master = master;
      
      highlight = false;
      mouseOver = false;
    }
    
    
    int ID;
    
    static const int numClips = 10;
    ClipState clips[numClips];
    
    bool _master;
    bool mouseOver;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    void setID( int id )
    {
      ID = id;
    }
    
    /** converts the Looper::State into the UI represnted ClipSelector state.
     * It puts some of the data into clips[], and stores unique state into the class.
    **/
    void setState( int clipNum, GridLogic::State s )
    {
#ifdef DEBUG_CLIP
      cout << "setState clipNum = " << clipNum << "  state = " << s << endl;
#endif
      switch(s)
      {
        case GridLogic::STATE_RECORDING:
            clips[clipNum].setName();
            break;
        case GridLogic::STATE_STOPPED:
        case GridLogic::STATE_EMPTY:
        case GridLogic::STATE_PLAYING:
        case GridLogic::STATE_PLAY_QUEUED:
        case GridLogic::STATE_RECORD_QUEUED:
        case GridLogic::STATE_STOP_QUEUED:
        default: break;
      }
      
      clips[clipNum].setState( s );
      
      redraw();
    }
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        if ( value() )
        {
          highlight = true;
        }
        else
        {
          highlight = false;
        }
        
        cairo_t *cr = Fl::cairo_cc();
        
        cairo_save( cr );
        
        int clipWidth  = w - 2;
        int clipHeight = (h / numClips);
        
        int drawY = y + 1;
        for( int i = 0; i < numClips; i++) // draw each clip
        {
          cairo_rectangle( cr, x+2, drawY, clipWidth-1, clipHeight - 4 );
          cairo_set_source_rgba(cr, 66 / 255.f,  66 / 255.f ,  66 / 255.f, 0.4);
          cairo_fill(cr);
          
          cairo_rectangle( cr, x+2, drawY, clipHeight-4, clipHeight - 4 );
          
          if ( clips[i].getState() == GridLogic::STATE_RECORDING )
          {
            cairo_set_source_rgba(cr, 1.f,  0 / 255.f ,  0 / 255.f, 1.f);
            cairo_fill(cr);
            cairo_arc( cr, x+14, drawY+13, 4.3, 0, 6.29 );
            cairo_set_source_rgba(cr, 0, 0, 0, 1.f);
            cairo_fill(cr);
          }
          else if ( clips[i].getState() == GridLogic::STATE_PLAYING )
          {
            cairo_set_source_rgba(cr, 0.0, 1.0,   0, 1.f );
            cairo_fill(cr);
            cairo_move_to( cr, x+10, drawY+8 );
            cairo_line_to( cr, x+19, drawY+13 );
            cairo_line_to( cr, x+10, drawY+18 );
            cairo_close_path(cr);
            cairo_set_source_rgba(cr, 0, 0, 0, 1.f);
            cairo_fill(cr);
          }
          else if ( clips[i].getState() == GridLogic::STATE_PLAY_QUEUED )
          {
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
            cairo_fill(cr);
            cairo_move_to( cr, x+10, drawY+8 );
            cairo_line_to( cr, x+19, drawY+13 );
            cairo_line_to( cr, x+10, drawY+18 );
            cairo_close_path(cr);
            cairo_set_source_rgba(cr, 0, 0, 0, 1.f);
            cairo_fill(cr);
          }
          else if ( clips[i].getState() == GridLogic::STATE_STOP_QUEUED )
          {
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
            cairo_fill(cr);
            cairo_arc( cr, x+14, drawY+13, 4.3, 0, 6.29 );
            cairo_set_source_rgba(cr, 0, 0, 0, 1.f);
            cairo_set_line_width(cr, 2.2f);
            cairo_stroke(cr);
          }
          else if ( clips[i].getState() == GridLogic::STATE_RECORD_QUEUED )
          {
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
            cairo_fill(cr);
            cairo_arc( cr, x+14, drawY+13, 4.3, 0, 6.29 );
            cairo_set_source_rgba(cr, 0, 0, 0, 1.f);
            cairo_fill(cr);
          }
          else if ( clips[i].getState() == GridLogic::STATE_STOPPED )
          {
            cairo_set_source_rgba(cr, 1.0, 0.6,   0, 1.f);
            cairo_fill(cr);
            cairo_arc( cr, x+14, drawY+13, 4.3, 0, 6.29 );
            cairo_set_source_rgba(cr, 0, 0, 0, 1.f);
            cairo_set_line_width(cr, 2.2f);
            cairo_stroke(cr);
          }
          else
          {
            cairo_set_source_rgba(cr, 66 / 255.f,  66 / 255.f ,  66 / 255.f, 1.f);
            cairo_fill(cr);
          }
          
          cairo_rectangle( cr, x+2, drawY, clipWidth -1, clipHeight - 3 );
          
          float alpha = 1;
          cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, alpha);
          cairo_set_line_width( cr, 1.3);
          cairo_move_to( cr, x+clipHeight-1, drawY );
          cairo_line_to( cr, x+clipHeight-1, drawY + clipHeight - 2);
          cairo_stroke(cr);
          
          // clip name
          cairo_move_to( cr, x + clipHeight + 10, drawY + 16 );
          cairo_set_source_rgba( cr, 255 / 255.f, 255 / 255.f , 255 / 255.f , 1 );
          cairo_set_font_size( cr, 10 );
          cairo_show_text( cr, clips[i].getName().c_str() );
          
          drawY += clipHeight;
        }
        
        cairo_restore( cr );
        
        //draw_label();
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
        case FL_ACTIVATE:
          {
          }
        case FL_DEACTIVATE:
          {
          }
          return 1;
        case FL_PUSH:
          highlight = 1;
          {
            // calculate the clicked clip number
            int clipHeight = (h / numClips);
            int clipNum = ( (Fl::event_y() ) - y ) / clipHeight;
            if (clipNum >= numClips)
              clipNum = numClips -1; // fix for clicking the lowest pixel
            
            // handle right clicks: popup menu
            if ( Fl::event_state(FL_BUTTON3) )
            {
              if ( _master )
              {
                
                redraw();
                return 1;
              }
              
              
              Fl_Menu_Item rclick_menu[] =
              {
                { "Load" },
                { "Bars",  0,   0, 0, FL_SUBMENU },
                  {"1       "},
                  {"2"},
                  {"4"},
                  {"8"},
                  {"16"},
                  {"32"},
                  {"64"},
                  {0},
                //{ "Record" },
                { "Use as tempo" },
                { 0 }
              };
              Fl_Menu_Item *m = (Fl_Menu_Item*) rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
              if ( !m )
              {
                  return 0;
              }
              else if ( strcmp(m->label(), "Load") == 0 )
              {
                int loadFail = clipSelectorLoad( ID, clipNum );
                if ( !loadFail )
                {
                  clips[clipNum].setName();
                  clips[clipNum].setState(GridLogic::STATE_STOPPED);
                }
              }
              else if ( strcmp(m->label(), "1") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,1);
                writeToDspRingbuffer( &e );
              } else if ( strcmp(m->label(), "2") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,2);
                writeToDspRingbuffer( &e );
              } else if ( strcmp(m->label(), "4") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,4);
                writeToDspRingbuffer( &e );
              } else if ( strcmp(m->label(), "8") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,8);
                writeToDspRingbuffer( &e );
              } else if ( strcmp(m->label(), "16") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,16);
                writeToDspRingbuffer( &e );
              } else if ( strcmp(m->label(), "32") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,32);
                writeToDspRingbuffer( &e );
              } else if ( strcmp(m->label(), "64") == 0 ) {
                EventLooperLoopLength e = EventLooperLoopLength(ID, clipNum ,64);
                writeToDspRingbuffer( &e );
              }
              else if ( strcmp(m->label(), "Use as tempo") == 0 )
              {
                EventLooperUseAsTempo e (ID, clipNum);
                writeToDspRingbuffer( &e );
              }
              else if ( strcmp(m->label(), "Record") == 0 )
              {
                /*
                //recordingClip = clipNum;
                EventLooperState e = EventLooperState( ID, clipNum, Looper::STATE_RECORD_QUEUED);
                writeToDspRingbuffer( &e );
                */
              }
            }
            else
            {
              if ( _master )
              {
                EventGridLaunchScene e( clipNum );
                writeToDspRingbuffer( &e );
                redraw();
                return 1;
              }
              
              // write "pressed" event for this track,scene
              EventGridEvent e( ID, clipNum, true );
              writeToDspRingbuffer( &e );
            }
          }
          redraw();
          do_callback();
          return 1;
        case FL_DRAG:
          {
            int t = Fl::event_inside(this);
            if (t != highlight)
            {
              highlight = t;
              redraw();
            }
          }
          return 1;
        case FL_ENTER:
          mouseOver = true;
          // push help string to UI tooltip area
          luppp_tooltip( "Clip selector" );
          
          redraw();
          return 1;
        case FL_LEAVE:
          mouseOver = false;
          redraw();
          return 1;
        case FL_RELEASE:
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

#endif // AVTK_CLIP_SELECTOR_H

