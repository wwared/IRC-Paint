#include "Brush.h"

void Brush::onWidgetPaint(QPaintEvent *, QPainter&) {
    return;
}

bool Brush::onMouseClick(QMouseEvent *, int , int, bool) {
    return false;
}

bool Brush::onMouseRelease(QMouseEvent *, int , int, bool) {
    return false;
}

bool Brush::onMouseMove(QMouseEvent *, int , int, bool) {
    return false;
}
