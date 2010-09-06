#include <QtGui>

#include "DockTitle.h"

#include <QString>

DockTitle::DockTitle(const QString& fname, QWidget *parent) : QWidget(parent), icon(fname) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QSize DockTitle::minimumSizeHint() const {
    return QSize(19,19);
}

QSize DockTitle::sizeHint() const {
    return QSize(19,19);
}

void DockTitle::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
    painter.drawRoundedRect(0, 1, dockWidget->isFloating() ? width()-1 : width()-3, 17, 5, 5);
    painter.drawPixmap((width()/2)-8, 2, icon.pixmap(16, 16));
}
