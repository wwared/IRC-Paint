#include <QtGui>
#include "MainWidget.h"

#include "Brush_Pen.h"

#include "BPen_Command.h"

bool Brush_Pen::onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        if (event->button() == Qt::LeftButton) {
            undo->push(new BPen_Command(widget, &widget->background, widget->alternate ? &widget->foreground : NULL, id, x, y, widget->getBGColor().rgb()));
        } else if (event->button() == Qt::RightButton) {
            undo->push(new BPen_Command(widget, &widget->foreground, widget->alternate ? &widget->background : NULL, id, x, y, widget->getFGColor().rgb()));
        }
        return true;
    }
    return false;
}

bool Brush_Pen::onMouseMove(QMouseEvent *event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        if (event->buttons() & Qt::LeftButton) {
            undo->push(new BPen_Command(widget, &widget->background, widget->alternate ? &widget->foreground : NULL, id, x, y, widget->getBGColor().rgb()));
        } else if (event->buttons() & Qt::RightButton) {
            undo->push(new BPen_Command(widget, &widget->foreground, widget->alternate ? &widget->background : NULL, id, x, y, widget->getFGColor().rgb()));
        }
        return true;
    }
    return false;
}

bool Brush_Pen::onMouseRelease(QMouseEvent *, int, int, bool) {
    id = id == 55 ? 555 : 55;
    return false;
}
