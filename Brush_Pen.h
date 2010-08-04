#ifndef BRUSH_PEN_H
#define BRUSH_PEN_H

#include "Brush.h"

class Brush_Pen : public Brush {
public:
    Brush_Pen(MainWidget* w) : Brush(w) {}
    void onMouseClick(QMouseEvent *event, int x, int y);
    void onMouseMove(QMouseEvent *event, int x, int y);
};

#endif // BRUSH_PEN_H
