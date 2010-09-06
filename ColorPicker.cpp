#include <QtGui>

#include "ColorPicker.h"
#include <QtDebug>

ColorPicker::ColorPicker(QWidget *parent, QMap<int, QRgb>* c) : QWidget(parent), colors(c) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize ColorPicker::sizeHint() const {
    return QSize(53,209);
}

void ColorPicker::mouseReleaseEvent(QMouseEvent* event) {
    int i = event->pos().x() / 25;
    int j = event->pos().y() / 25;
    if (i > 1 || j > 7) {
        QWidget::mouseReleaseEvent(event);
        return;
    }
    if (event->button() == Qt::LeftButton) {
        emit bgColorPicked(i ? (j*2)+1 : j*2);
    } else if (event->button() == Qt::RightButton) {
        emit fgColorPicked(i ? (j*2)+1 : j*2);
    }
}

void ColorPicker::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    for (int i = 0; i < 3; ++i)
        painter.drawLine(25*i, 0, 25*i, 200);
    for (int i = 0; i < 9; ++i)
        painter.drawLine(0, 25*i, 50, 25*i);
    for (int i = 0; i < 8; ++i) // first column (0,2,4,6,8,10,12,14)
        painter.fillRect(1, (25*i)+1, 24, 24, (*colors)[i*2]);
    for (int i = 0; i < 8; ++i) // second column (1,3,5,7,9,11,13,15)
        painter.fillRect(26, (25*i)+1, 24, 24, (*colors)[(i*2)+1]);
}
