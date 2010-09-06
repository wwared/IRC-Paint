#include <QtGui>

#include "DockTitle.h"

#include <QString>
#include <QBrush>
#include <QLinearGradient>

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
    QLinearGradient grad(width()/2, 4, width()/2, 16);
    grad.setColorAt(0, palette().dark().color());
    grad.setColorAt(1, palette().window().color());
    QBrush b(grad);
    painter.setBrush(b);
    painter.fillRect(0, 0, width(), height(), grad);
    painter.drawPixmap((width()/2)-8, 0, icon.pixmap(16, 16));
}
