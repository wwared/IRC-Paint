#include <QtGui>

#include "DockTitle.h"

#include <QString>

DockTitle::DockTitle(const QString& fname, QWidget *parent) : QWidget(parent), icon(fname) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QSize DockTitle::minimumSizeHint() const {
    return QSize(16,16);
}

QSize DockTitle::sizeHint() const {
    return QSize(16,16);
}

void DockTitle::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    painter.drawLine(0, 15, width(), 15);
    painter.drawPixmap((width()/2)-8, 0, icon.pixmap(16, 16));
}
