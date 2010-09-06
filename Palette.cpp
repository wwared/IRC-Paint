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
    connect(picker, SIGNAL(bgColorPicked(int)), this, SLOT(bgColorChanged(int)));
    connect(picker, SIGNAL(bgColorPicked(int)), swatch, SLOT(bgColorChanged(int)));
    connect(picker, SIGNAL(fgColorPicked(int)), this, SLOT(fgColorChanged(int)));
    connect(picker, SIGNAL(fgColorPicked(int)), swatch, SLOT(fgColorChanged(int)));

    layout->setColumnMinimumWidth(0, 7);
    layout->addWidget(picker, 0, 1);
    layout->addWidget(swatch, 1, 1);
}

void Palette::bgColorChanged(int) {
}

void Palette::fgColorChanged(int) {
}


QString Palette::ircToText(int i) {
    while (i > 15)
        i -= 15;
    switch (i) {
    case 0:  return tr("White");
    case 1:  return tr("Black");
    case 2:  return tr("Dark Blue");
    case 3:  return tr("Dark Green");
    case 4:  return tr("Red");
    case 5:  return tr("Dark Red");
    case 6:  return tr("Purple");
    case 7:  return tr("Orange");
    case 8:  return tr("Yellow");
    case 9:  return tr("Lime Green");
    case 10: return tr("Teal");
    case 11: return tr("Sky Blue");
    case 12: return tr("Blue");
    case 13: return tr("Pink");
    case 14: return tr("Dark Gray");
    case 15: return tr("Light Gray");
    default:
        qCritical("FATAL: unexpected fall through in irc -> text conversion");
        return tr("Error");
    }
}
