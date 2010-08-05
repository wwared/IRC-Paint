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
    Brush_Line(MainWidget* w) : Brush(w), drawPreview(false), drawOnBg(false), xstart(0), ystart(0), xend(0), yend(0) {}
    void onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget);
    void onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget);
    void onMouseRelease(QMouseEvent *event, int x, int y, bool insideWidget);
    void onWidgetPaint(QPaintEvent *event, QPainter &painter);
};

#endif // BRUSH_LINE_H