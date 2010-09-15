#include <QtGui>
#include "MainWidget.h"

#include "Brush_Fill.h"

bool Brush_Fill::onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        QRgb from, to;
        QImage* img;
        if (event->button() == Qt::LeftButton) {
            img = &widget->background;
            from = img->pixel(x, y);
            to = widget->getBGColor().rgb();
            if (from == to)
                return false;
            pixels << QPoint(x, y);
        } else if (event->button() == Qt::RightButton) {
            img = &widget->foreground;
            from = img->pixel(x, y);
            to = widget->getFGColor().rgb();
            if (from == to)
                return false;
            pixels << QPoint(x, y);
        } else {
            return false;
        }
        for (int i = 0; i < pixels.size(); ++i) {
            if (img->pixel(pixels[i]) != from)
                continue;
            img->setPixel(pixels[i], to);
            widget->update(widget->pixelRect(pixels[i]));
            int fy = pixels[i].y();
            for (int fx = pixels[i].x()+1; fx < img->width() && img->pixel(fx, fy) == from; ++fx) {
                img->setPixel(fx, fy, to);
                widget->update(widget->pixelRect(fx, fy));
                if (fy < img->height()-1 && img->pixel(fx, fy+1) == from)
                    pixels << QPoint(fx, fy+1);
                if (fy > 0 && img->pixel(fx, fy-1) == from)
                    pixels << QPoint(fx, fy-1);
            }
            for (int fx = pixels[i].x()-1; fx >= 0 && img->pixel(fx, fy) == from; --fx) {
                img->setPixel(fx, fy, to);
                widget->update(widget->pixelRect(fx, fy));
                if (fy < img->height()-1 && img->pixel(fx, fy+1) == from)
                    pixels << QPoint(fx, fy+1);
                if (fy > 0 && img->pixel(fx, fy-1) == from)
                    pixels << QPoint(fx, fy-1);
            }
        }
        pixels.clear();
        return true;
    }
    return false;
}
