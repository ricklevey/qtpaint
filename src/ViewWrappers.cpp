#include "PlotView.hpp"
#include <QGraphicsScene>

#include "conversion.h"

extern "C" {

  SEXP
  qt_qview(SEXP rscene, SEXP rescaleMode, SEXP ropengl)
  {
    SEXP ans;
    QGraphicsScene *scene = unwrapQObject(rscene, QGraphicsScene);
    QGraphicsView *view =
      new QViz::PlotView(scene, 0,
                         (QViz::PlotView::RescaleMode)asInteger(rescaleMode));
    // Do we want to make OpenGL an option?
    if (asLogical(ropengl))
      view->setViewport(new QGLWidget);
    ans = wrapQWidget(view);
    // Make sure scene is only deleted after all views are gone
    // FIXME: leaky if view is later assigned a new scene -- does not
    // seem possible to detect that though
    // Possible hack: specially hash view -> ref, delete ref when view
    // switches scene, but again we cannot detect that
    addQObjectReference(scene, view);
    //    R_SetExternalPtrProtected(ans, rscene);
    return ans;
  }

  SEXP
  qt_qfitScene_QGraphicsView(SEXP rview) {
    QGraphicsView *view = unwrapQObject(rview, QGraphicsView);
    view->fitInView(view->sceneRect());
    return rview;
  }

  SEXP
  qt_qsetMatrix_QGraphicsView(SEXP extp, SEXP rmatrix)
  {
    QGraphicsView *view = unwrapQObject(extp, QGraphicsView);
    view->setMatrix(asQMatrix(rmatrix));
    return extp;
  }
  SEXP qt_qmatrix_QGraphicsView(SEXP extp, SEXP inverted)
  {
    QGraphicsView *view = unwrapQObject(extp, QGraphicsView);
    return asRMatrix(view->matrix(), asLogical(inverted));
  }

}

