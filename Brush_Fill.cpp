#include <QtGui>

#include "MainWidget.h"

#include "Brush_Fill.h"
#include "BFill_Command.h"

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
        } else if (event->button() == Qt::RightButton) {
            img = &widget->foreground;
            from = img->pixel(x, y);
            to = widget->getFGColor().rgb();
            if (from == to)
                return false;
        } else {
            return false;
        }
        undo->push(new BFill_Command(widget, img, from, to, x, y));
        return true;
    }
    return false;
}
