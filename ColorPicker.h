#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QMap>
#include <QRgb>

class ColorPicker : public QWidget {
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent, QMap<int, QRgb>* c);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent*);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void bgColorPicked(int);
    void fgColorPicked(int);

private:
    QMap<int, QRgb>* colors;
};

#endif // COLORPICKER_H
