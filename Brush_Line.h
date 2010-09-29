#ifndef BRUSH_LINE_H
#define BRUSH_LINE_H

#include "Brush.h"

#include <QColor>
#include <QPoint>

class QUndoStack;

class Brush_Line : public Brush {
    QPoint start, end;
    int xstart, ystart;
    bool drawPreview, drawOnBg;
    QColor col;
    QUndoStack* undo;
public:
    Brush_Line(MainWidget* w, QUndoStack* u) : Brush(w), undo(u), drawPreview(false), drawOnBg(false), xstart(0), ystart(0) {}
    bool onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseRelease(QMouseEvent *event, int x, int y, bool insideWidget);
    void onWidgetPaint(QPaintEvent *event, QPainter &painter);
};

#endif // BRUSH_LINE_H
