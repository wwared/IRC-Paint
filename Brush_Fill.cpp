#include <QtGui>
#include "MainWidget.h"

#include "Brush_Fill.h"

bool Brush_Fill::onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        if (event->button() == Qt::LeftButton) {
            QRgb from = widget->background.pixel(x, y);
            QRgb to = widget->getBGColor().rgb();
            if (from != to) {
                fill(&widget->background, x, y, from, to);
                return true;
            }
        } else if (event->button() == Qt::RightButton) {
            QRgb from = widget->foreground.pixel(x, y);
            QRgb to = widget->getFGColor().rgb();
            if (from != to) {
                fill(&widget->foreground, x, y, from, to);
                return true;
            }
        }
    }
    return false;
}

void Brush_Fill::fill(QImage* img, int x, int y, QRgb from, QRgb to) {
    img->setPixel(x, y, to);
    widget->update(widget->pixelRect(x, y));
    if (x != img->width()-1 && img->pixel(x+1, y) == from)
        fill(img, x+1, y, from, to);
    if (x != 0 && img->pixel(x-1, y) == from)
        fill(img, x-1, y, from, to);
    if (y != img->height()-1 && img->pixel(x, y+1) == from)
        fill(img, x, y+1, from, to);
    if (y != 0 && img->pixel(x, y-1) == from)
        fill(img, x, y-1, from, to);
}
