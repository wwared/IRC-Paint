#include <cstdlib>

#include <QtGui>

#include "BRect_Command.h"
#include "MainWidget.h"

void BRect_Command::undo() {
    foreach (cell c, changed) {
        img->setPixel(c.first, c.second);
        widget->update(widget->pixelRect(c.first));
    }
}

void BRect_Command::redo() {
    changed.clear();
    int xl, xr, yb, yt;
    if (xstart < xend) {
        xl = xstart; xr = xend;
    } else {
        xl = xend; xr = xstart;
    }
    if (ystart < yend) {
        yt = ystart; yb = yend;
    } else {
        yt = yend; yb = ystart;
    }
    for (int i = xl; i <= xr; ++i) {
        changed << std::make_pair(QPoint(i, yt), img->pixel(i, yt))
                << std::make_pair(QPoint(i, yb), img->pixel(i, yb));
        img->setPixel(i, yt, col); img->setPixel(i, yb, col);
    }
    for (int i = yt+1; i < yb; ++i) {
        changed << std::make_pair(QPoint(xl, i), img->pixel(xl, i))
                << std::make_pair(QPoint(xr, i), img->pixel(xr, i));
        img->setPixel(xl, i, col); img->setPixel(xr, i, col);
    }
    widget->update(widget->pixelRect(xstart, ystart).united(widget->pixelRect(xend, yend)).adjusted(-3, -3, 3, 3));
}
