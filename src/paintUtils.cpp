#include "paintUtils.hpp"
#include <R_ext/Arith.h>

using namespace QViz;

/* these two functions break primitives on NA */

#define UPDATE_ATTR(x, setter)                      \
  if (x && prev_ ## x != x[k]) {                    \
    p->setter(x[k]);                                \
    prev_ ## x = x[k];                              \
  }

void PaintUtils::drawPolylines(Painter *p, double *x, double *y, QColor *stroke,
                               int n)
{
  int j = 0, i = 0, k = 0;
  QColor prev_stroke = stroke ? stroke[0] : QColor();
  for (i = 0; i < n; i++) {
    if (x[i] == NA_REAL || y[i] == NA_REAL) {
      UPDATE_ATTR(stroke, setStrokeColor);
      p->drawPolyline(x + j, y + j, i - j);
      j = i;
      k++;
    }
  }
  if (n && stroke) p->setStrokeColor(stroke[k]);
  p->drawPolyline(x + j, y + j, i - j);
}

void PaintUtils::drawPolygons(Painter *p, double *x, double *y, QColor *stroke,
                              QColor *fill, int n)
{
  int j = 0, i = 0, k = 0;
  QColor prev_stroke = stroke ? stroke[0] : QColor(),
    prev_fill = fill ? fill[0] : QColor();
  for (i = 0; i < n; i++) {
    if (x[i] == NA_REAL || y[i] == NA_REAL) {
      UPDATE_ATTR(stroke, setStrokeColor);
      UPDATE_ATTR(fill, setFillColor);
      p->drawPolygon(x + j, y + j, i - j);
      j = i;
      k++;
    }
  }
  if (n) {
    if (stroke) p->setStrokeColor(stroke[k]);
    if (fill) p->setFillColor(fill[k]);
  }
  p->drawPolygon(x + j, y + j, i - j);
}