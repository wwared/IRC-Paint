#ifndef BRUSH_RECT_H
#define BRUSH_RECT_H

#include "Brush.h"

#include <QColor>
#include <QPoint>

class Brush_Rect : public Brush {
    QPoint start, end;
    int xstart, ystart, xend, yend;
    bool drawPreview, drawOnBg;
    QColor col;
public:
    Brush_Rect(MainWidget* w) : Brush(w), drawPreview(false), drawOnBg(false), xstart(0), ystart(0), xend(0), yend(0) {}
    bool onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseRelease(QMouseEvent *event, int x, int y, bool insideWidget);
    void onWidgetPaint(QPaintEvent *event, QPainter &painter);
};

#endif // BRUSH_RECT_H
