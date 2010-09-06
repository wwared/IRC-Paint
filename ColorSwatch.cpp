#include <QtGui>

#include "ColorSwatch.h"

ColorSwatch::ColorSwatch(QWidget *parent, QMap<int, QRgb>* c) : QWidget(parent), colors(c), bg(&(*c)[1]), fg(&(*c)[0]) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize ColorSwatch::sizeHint() const {
    return QSize(46, 46);
}

void ColorSwatch::bgColorChanged(int i) {
    while (i > 15)
        i -= 15;
    bg = &(*colors)[i];
    update();
}

void ColorSwatch::fgColorChanged(int i) {
    while (i > 15)
        i -= 15;
    fg = &(*colors)[i];
    update();
}

void ColorSwatch::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    painter.drawRect(0, 0, 31, 31);
    painter.fillRect(1, 1, 30, 30, QColor::fromRgb(*fg));
    painter.drawRect(14, 14, 31, 31);
    painter.fillRect(15, 15, 30, 30, QColor::fromRgb(*bg));
}
