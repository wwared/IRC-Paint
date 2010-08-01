#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QList>
#include <QChar>

/* *TODO:
   * Some way of loading files/changing background/foreground images (and initiating a redraw)
   */

class MainWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ getBGColor WRITE setBGColor)
    Q_PROPERTY(QColor fgColor READ getFGColor WRITE setFGColor)
    Q_PROPERTY(QColor selColor READ getSelColor WRITE setSelColor)
    Q_PROPERTY(QImage bgImage READ getBGImage)
    Q_PROPERTY(QImage fgImage READ getFGImage)
    Q_PROPERTY(bool showGrid READ gridShown WRITE setGrid)
    Q_PROPERTY(int xsize READ rectWidth WRITE setRectWidth)
    Q_PROPERTY(int ysize READ rectHeight WRITE setRectHeight)
    Q_PROPERTY(int xasc READ ascWidth)
    Q_PROPERTY(int yasc READ ascHeight)

public:
    explicit MainWidget(QWidget *parent = 0);

    void setBGColor(const QColor &newColor) { bgColor = newColor; }
    void setFGColor(const QColor &newColor) { fgColor = newColor; }
    void setSelColor(const QColor &newColor) { selColor = newColor; }
    QColor getBGColor() const { return bgColor; }
    QColor getFGColor() const { return fgColor; }
    QColor getSelColor() const { return selColor; }
    void setRectWidth(int x);
    void setRectHeight(int y);
    int rectWidth() const { return xsize; }
    int rectHeight() const { return ysize; }
    int ascWidth() const { return xasc; }
    int ascHeight() const { return yasc; }
    bool gridShown() const { return showGrid; }
    void setGrid(bool g);
    void setBGImage(const QImage &newImage);
    void setFGImage(const QImage &newImage);
    QImage getBGImage() const { return background; }
    QImage getFGImage() const { return foreground; }
    QSize sizeHint() const;
    void addRows(int place, int n); // adds n blank rows of the current bg/fg color before the row place
    void delRows(int place, int n);
    void addColumns(int place, int n); // same but columns
    void delColumns(int place, int n);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void setBGImagePixel(const QPoint &pos);
    void setFGImagePixel(const QPoint &pos);
    QRect pixelRect(int i, int j) const;

    QColor bgColor, fgColor, selColor;
    QImage background, foreground;
    int xasc, yasc; // ascii size
    int xsize, ysize; // actual physical pixel size for the rects
    QList<QList<QChar> > text; // the ascii text
    bool showGrid; // whether to display 1-pixel black grid between rects
    int lastx, lasty; // last ascii cell selected
};

#endif // MAINWIDGET_H
