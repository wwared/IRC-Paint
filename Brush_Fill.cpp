#include <QtGui>
#include "MainWidget.h"

#include "Brush_Fill.h"

bool Brush_Fill::onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        if (event->button() == Qt::LeftButton) {
            QRgb from = widget->background.pixel(x, y);
            QRgb to = widget->getBGColor().rgb();
            if (from != to) {
                pixels << QPoint(x, y);
                fill(&widget->background, from, to);
                return true;
            }
        } else if (event->button() == Qt::RightButton) {
            QRgb from = widget->foreground.pixel(x, y);
            QRgb to = widget->getFGColor().rgb();
            if (from != to) {
                pixels << QPoint(x, y);
                fill(&widget->foreground, from, to);
                return true;
            }
        }
    }
    return false;
}

void Brush_Fill::fill(QImage* img, QRgb from, QRgb to) {
    for (int i = 0; i < pixels.size(); ++i) {
        img->setPixel(pixels[i], to);
        widget->update(widget->pixelRect(pixels[i]));
        int y = pixels[i].y();
        for (int x = pixels[i].x()+1; x < img->width() && img->pixel(x, y) == from; ++x) {
            img->setPixel(x, y, to);
            widget->update(widget->pixelRect(x, y));
            if (y < img->height()-1 && img->pixel(x, y+1) == from)
                pixels << QPoint(x, y+1);
            if (y > 0 && img->pixel(x, y-1) == from)
                pixels << QPoint(x, y-1);
        }
        for (int x = pixels[i].x()-1; x >= 0 && img->pixel(x, y) == from; --x) {
            img->setPixel(x, y, to);
            widget->update(widget->pixelRect(x, y));
            if (y < img->height()-1 && img->pixel(x, y+1) == from)
                pixels << QPoint(x, y+1);
            if (y > 0 && img->pixel(x, y-1) == from)
                pixels << QPoint(x, y-1);
        }
    }
    pixels.clear();
}
