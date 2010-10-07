#include <QtGui>
#include "MainWidget.h"

#include "Brush_Rect.h"
#include "BRect_Command.h"

bool Brush_Rect::onMouseClick(QMouseEvent* event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        start = event->pos();
        end = event->pos();
        xstart = x;
        ystart = y;
        drawPreview = true;
        if (event->button() == Qt::LeftButton) {
            drawOnBg = true;
            col = widget->getBGColor().rgb();
        } else {
            drawOnBg = false;
            col = widget->getFGColor().rgb();
        }
        widget->update(QRect(start,end).adjusted(-3,-3,3,3));
    }
    return false;
}

bool Brush_Rect::onMouseMove(QMouseEvent* event, int , int , bool insideWidget) {
    QPoint oldend = end;
    if (insideWidget) {
        end = event->pos();
    }
    widget->update(QRect(start,end).united(QRect(start,oldend)).adjusted(-3,-3,3,3));
    return false;
}

bool Brush_Rect::onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget) {
    drawPreview = false;
    if (insideWidget) {
        end = event->pos();
        undo->push(new BRect_Command(widget, drawOnBg ? &widget->background : &widget->foreground, col, xstart, ystart, x, y));
        return true;
    } else {
        widget->update(QRect(start,end).adjusted(-3,-3,3,3));
        return false;
    }
}

void Brush_Rect::onWidgetPaint(QPaintEvent* , QPainter &painter) {
    if (drawPreview) {
        QPen p(col);
        p.setWidth(3);
        painter.setPen(p);
        painter.drawRect(QRect(start, end));
    }
}
