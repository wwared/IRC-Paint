#include <QtGui>
#include "MainWidget.h"

#include "Brush_Line.h"

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
        xend = x;
        yend = y;
        QPainter p;
        if (drawOnBg) {
            p.begin(&widget->background);
        } else {
            p.begin(&widget->foreground);
        }
        p.setPen(col);
        p.drawLine(xstart,ystart,xend,yend);
        p.end();
        widget->update(widget->pixelRect(xstart,ystart).united(widget->pixelRect(xend, yend)).adjusted(-3,-3,3,3));
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
