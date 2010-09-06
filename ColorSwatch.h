#ifndef COLORSWATCH_H
#define COLORSWATCH_H

#include <QWidget>
#include <QMap>
#include <QRgb>

class ColorSwatch : public QWidget {
    Q_OBJECT
public:
    explicit ColorSwatch(QWidget *parent, QMap<int, QRgb>* c);
    QSize sizeHint() const;

public slots:
    void bgColorChanged(int);
    void fgColorChanged(int);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QMap<int, QRgb>* colors;
    QRgb* fg;
    QRgb* bg;
};

#endif // COLORSWATCH_H
