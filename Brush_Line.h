#ifndef BRUSH_LINE_H
#define BRUSH_LINE_H

#include "Brush.h"

#include <QColor>
#include <QPoint>

class Brush_Line : public Brush {
    QPoint start, end;
    int xstart, ystart, xend, yend;
    bool drawPreview, drawOnBg;
    QColor col;
public:
    Brush_Line(MainWidget* w) : Brush(w), drawPreview(false), xstart(0), drawOnBg(false), ystart(0), xend(0), yend(0) {}
    bool onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseRelease(QMouseEvent *event, int x, int y, bool insideWidget);
    void onWidgetPaint(QPaintEvent *event, QPainter &painter);
};

#endif // BRUSH_LINE_H
