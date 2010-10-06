#include <QtGui>

#include "BFill_Command.h"
#include "MainWidget.h"

void BFill_Command::undo() {
    foreach (cell c, changed) {
        img->setPixel(c.first, c.second);
        widget->update(widget->pixelRect(c.first));
    }
}

void BFill_Command::redo() {
    changed.clear();
    pixels.clear();
    pixels << QPoint(x, y);
    for (int i = 0; i < pixels.size(); ++i) {
        if (img->pixel(pixels[i]) != from)
            continue;
        int fy = pixels[i].y();
        for (int fx = pixels[i].x(); fx < img->width() && img->pixel(fx, fy) == from; ++fx) {
            changed << std::make_pair(QPoint(fx, fy), img->pixel(fx, fy));
            img->setPixel(fx, fy, to);
            widget->update(widget->pixelRect(fx, fy));
            if (fy < img->height()-1 && img->pixel(fx, fy+1) == from)
                pixels << QPoint(fx, fy+1);
            if (fy > 0 && img->pixel(fx, fy-1) == from)
                pixels << QPoint(fx, fy-1);
        }
        for (int fx = pixels[i].x()-1; fx >= 0 && img->pixel(fx, fy) == from; --fx) {
            changed << std::make_pair(QPoint(fx, fy), img->pixel(fx, fy));
            img->setPixel(fx, fy, to);
            widget->update(widget->pixelRect(fx, fy));
            if (fy < img->height()-1 && img->pixel(fx, fy+1) == from)
                pixels << QPoint(fx, fy+1);
            if (fy > 0 && img->pixel(fx, fy-1) == from)
                pixels << QPoint(fx, fy-1);
        }
    }
}
