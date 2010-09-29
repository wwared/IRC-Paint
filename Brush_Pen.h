#ifndef BRUSH_PEN_H
#define BRUSH_PEN_H

#include "Brush.h"

class QUndoStack;

class Brush_Pen : public Brush {
    QUndoStack* undo;
public:
    Brush_Pen(MainWidget* w, QUndoStack* u) : Brush(w), undo(u) {}
    bool onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget);
};

#endif // BRUSH_PEN_H
