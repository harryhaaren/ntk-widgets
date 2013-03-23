
#ifndef AVTK_HELPERS_H
#define AVTK_HELPERS_H

#include <iostream>

using namespace std;

inline void avtk_stroke_line(cairo_t* cr, bool active)
{
  // stroke cutoff lines, or border backgrounds
  cairo_set_line_width(cr, 1.5);
  if ( active )
    cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.8 );
  else
    cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 1 );
}

#endif // AVTK_HELPERS_H

