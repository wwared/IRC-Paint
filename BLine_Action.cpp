#include <QtGui>

#include "BLine_Action.h"
#include "MainWidget.h"

void BLine_Action::undo() {
    QPainter p(img);
    p.drawImage(qMin(xstart, xend), qMin(ystart, yend), changed);
    p.end();
    widget->update(widget->pixelRect(xstart,ystart).united(widget->pixelRect(xend, yend)));
}

void BLine_Action::redo() {
    QPainter p(img);
    p.setPen(col);
    p.drawLine(xstart,ystart,xend,yend);
    p.end();
    widget->update(widget->pixelRect(xstart,ystart).united(widget->pixelRect(xend, yend)).adjusted(-3,-3,3,3));
}
