#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QMap>
#include <QRgb>

class QGridLayout;
class ColorSwatch;
class ColorPicker;

class Palette : public QWidget {
    Q_OBJECT
public:
    explicit Palette(QWidget *parent, QMap<int, QRgb>* c);

public slots:
    void bgColorChanged(int c); // called when a color is picked through ColorPicker
    void fgColorChanged(int c);

public:
    ColorPicker* picker; // public so other widgets can connect to its signals

private:
    QString ircToText(int i);

    QMap<int, QRgb>* colors;
    QGridLayout* layout;
    ColorSwatch* swatch;
    int fg, bg;
    QRgb fgr, bgr;
};

#endif // PALETTE_H
