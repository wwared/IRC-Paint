#include <QtGui>

#include "BLine_Command.h"
#include "MainWidget.h"

void BLine_Command::undo() {
    QPainter p(img);
    p.drawImage(qMin(xstart, xend), qMin(ystart, yend), changed);
    p.end();
    if (other) {
        p.begin(other);
        p.drawImage(qMin(xstart, xend), qMin(ystart, yend), *otherChanged);
        p.end();
    }
    widget->update(widget->pixelRect(xstart,ystart).united(widget->pixelRect(xend, yend)));
}

void BLine_Command::redo() {
    QPainter p(img);
    p.setPen(col);
    p.drawLine(xstart,ystart,xend,yend);
    p.end();
    if (other) {
        p.begin(other);
        p.setPen(col);
        p.drawLine(xstart,ystart,xend,yend);
        p.end();
    }
    widget->update(widget->pixelRect(xstart,ystart).united(widget->pixelRect(xend, yend)).adjusted(-3, -3, 3, 3));
}
