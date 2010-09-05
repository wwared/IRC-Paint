#include <QtGui>
#include "MainWidget.h"

#include "Brush_Pen.h"

bool Brush_Pen::onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        if (event->button() == Qt::LeftButton) {
            widget->setBGImagePixel(x, y);
        } else if (event->button() == Qt::RightButton) {
            widget->setFGImagePixel(x, y);
        }
        return true;
    }
    return false;
}

bool Brush_Pen::onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        if (event->buttons() & Qt::LeftButton) {
            widget->setBGImagePixel(x, y);
        } else if (event->buttons() & Qt::RightButton) {
            widget->setFGImagePixel(x, y);
        }
        return true;
    }
    return false;
}
