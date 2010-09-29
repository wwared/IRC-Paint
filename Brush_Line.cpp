#include <QtGui>
#include "MainWidget.h"

#include "Brush_Line.h"
#include "BLine_Action.h"

bool Brush_Line::onMouseClick(QMouseEvent *event, int x, int y, bool insideWidget) {
    if (insideWidget) {
        start = event->pos();
        end = event->pos();
        xstart = x;
        ystart = y;
        drawPreview = true;
        if (event->button() == Qt::LeftButton) {
            drawOnBg = true;
            col = widget->getBGColor();
        } else {
            drawOnBg = false;
            col = widget->getFGColor();
        }
        widget->update(QRect(start,end).adjusted(-3,-3,3,3));
    }
    return false;
}

bool Brush_Line::onMouseMove(QMouseEvent *event, int , int , bool insideWidget) {
    QPoint oldend = end;
    if (insideWidget) {
        end = event->pos();
    }
    widget->update(QRect(start,end).united(QRect(start,oldend)).adjusted(-3,-3,3,3));
    return false;
}

bool Brush_Line::onMouseRelease(QMouseEvent *event, int x, int y, bool insideWidget) {
    drawPreview = false;
    if (insideWidget) {
        end = event->pos();
        BLine_Action* act = new BLine_Action(widget, drawOnBg ? &widget->background : &widget->foreground, col, xstart, ystart, x, y);
        undo->push(act);
        return true;
    } else {
        widget->update(QRect(start,end).adjusted(-3,-3,3,3));
        return false;
    }
}

void Brush_Line::onWidgetPaint(QPaintEvent *, QPainter &painter) {
    if (drawPreview) {
        QPen p(col);
        p.setWidth(3);
        painter.setPen(p);
        painter.drawLine(start, end);
    }
}
