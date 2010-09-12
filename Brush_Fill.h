#ifndef BRUSH_FILL_H
#define BRUSH_FILL_H

#include "Brush.h"
#include <QRgb>

class QImage;

class Brush_Fill : public Brush {
public:
    Brush_Fill(MainWidget* w) : Brush(w) {}
    bool onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget);
private:
    void fill(QImage* img, int x, int y, QRgb from, QRgb to);
};

#endif // BRUSH_FILL_H
