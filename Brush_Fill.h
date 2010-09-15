#ifndef BRUSH_FILL_H
#define BRUSH_FILL_H

#include "Brush.h"
#include <QRgb>
#include <QList>
#include <QPoint>

class QImage;

class Brush_Fill : public Brush {
public:
    Brush_Fill(MainWidget* w) : Brush(w) {}
    bool onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget);
private:
    void fill(QImage* img, QRgb from, QRgb to);
    QList<QPoint> pixels;
};

#endif // BRUSH_FILL_H
