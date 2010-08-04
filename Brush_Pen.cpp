#include <QtGui>
#include "MainWidget.h"

#include "Brush_Pen.h"

void Brush_Pen::onMouseClick(QMouseEvent *event, int x, int y) {
    if (event->button() == Qt::LeftButton) {
        widget->setBGImagePixel(x, y);
    } else if (event->button() == Qt::RightButton) {
        widget->setFGImagePixel(x, y);
    }
}

void Brush_Pen::onMouseMove(QMouseEvent *event, int x, int y) {
    if (event->buttons() & Qt::LeftButton) {
        widget->setBGImagePixel(x, y);
    } else if (event->buttons() & Qt::RightButton) {
        widget->setFGImagePixel(x, y);
    }
}
