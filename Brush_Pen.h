#ifndef BRUSH_PEN_H
#define BRUSH_PEN_H

#include "Brush.h"

class QUndoStack;
class BPen_Command;

class Brush_Pen : public Brush {
    QUndoStack* undo;
    int id;
public:
    Brush_Pen(MainWidget* w, QUndoStack* u) : Brush(w), undo(u), id(55) {}
    bool onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget);
    bool onMouseRelease(QMouseEvent *, int, int, bool);
};

#endif // BRUSH_PEN_H
