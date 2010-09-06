#include <QtGui>

#include "Palette.h"
#include "ColorSwatch.h"
#include "ColorPicker.h"

#include <QGridLayout>

Palette::Palette(QWidget *parent, QMap<int, QRgb>* c) : QWidget(parent), colors(c), bg(1), fg(0) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout = new QGridLayout(this);
    setLayout(layout);

    picker = new ColorPicker(this, c);
    swatch = new ColorSwatch(this, c);
    connect(picker, SIGNAL(bgColorPicked(int)), swatch, SLOT(bgColorChanged(int)));
    connect(picker, SIGNAL(fgColorPicked(int)), swatch, SLOT(fgColorChanged(int)));

    layout->setColumnMinimumWidth(0, 7);
    layout->addWidget(picker, 0, 1);
    layout->addWidget(swatch, 1, 1);
}
