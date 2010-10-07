#ifndef BRUSH_RECT_H
#define BRUSH_RECT_H

#include "Brush.h"

#include <QRgb>
#include <QPoint>

class QUndoStack;

class Brush_Rect : public Brush {
    QPoint start, end;
    int xstart, ystart;
    bool drawPreview, drawOnBg;
    QRgb col;
    QUndoStack* undo;
public:
    Brush_Rect(MainWidget* w, QUndoStack* u) : Brush(w), undo(u), drawPreview(false), drawOnBg(false), xstart(0), ystart(0) {}
    bool onMouseClick(QMouseEvent* event, int x, int y, bool insideWidget);
    bool onMouseMove(QMouseEvent* event, int x, int y, bool insideWidget);
    bool onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget);
    void onWidgetPaint(QPaintEvent* event, QPainter &painter);
};

#endif // BRUSH_RECT_H
