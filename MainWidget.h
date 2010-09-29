#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <utility>
#include <algorithm>

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QList>
#include <QChar>
#include <QMap>

// Brushes
#include "BrushType.h"
#include "Brush.h"
#include "Brush_Pen.h"
#include "Brush_Fill.h"
#include "Brush_Line.h"
#include "Brush_Rect.h"

class QUndoStack;

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
    typedef QList<QList<QChar> > asc_text;
    Q_PROPERTY(asc_text text READ getText)
    Q_PROPERTY(bool showGrid READ gridShown WRITE setGrid)
    Q_PROPERTY(int xsize READ rectWidth WRITE setRectWidth)
    Q_PROPERTY(int ysize READ rectHeight WRITE setRectHeight)
    Q_PROPERTY(int xasc READ ascWidth)
    Q_PROPERTY(int yasc READ ascHeight)

    // Brushes
    friend class Brush;
    friend class Brush_Pen;
    friend class Brush_Fill;
    friend class Brush_Line;
    friend class Brush_Rect;

public:
    explicit MainWidget(QWidget *parent, QMap<int, QRgb>* c, QUndoStack* u);
    ~MainWidget();

    void swapAscii(int w, int h, QList<QList<QChar> > t, QImage b, QImage f);
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
    bool gridShown() const { return showGrid; } // whether grid is shown
    void setBGImage(const QImage &newImage); // replaces the background image
    void setFGImage(const QImage &newImage); // same for foreground
    QImage getBGImage() const { return background; } // returns a copy of the background
    QImage getFGImage() const { return foreground; } // same for foreground
    asc_text getText() const { return text; } // same for text
    QImage getRenderedImage(bool g) const; // returns a rendered ascii
    QSize sizeHint() const;
    void addRows(int place, int n); // adds n blank rows of the current bg/fg color before the row place
    void delRows(int place, int n);
    void addColumns(int place, int n); // same but columns
    void delColumns(int place, int n);
    void swapColor(QRgb c1, QRgb c2);
    void clearAscii();
    QRect pixelRect(int i, int j) const;
    QRect pixelRect(QPoint p) const;

public slots:
    void setGrid(bool g); // show/hide grid
    void bgColorChanged(int i); // bg color change
    void fgColorChanged(int i); // fg color change
    void setBrush(BrushType b); // brush change

signals:
    void somethingChanged(bool); // emitted when something changes the ascii (argument is true)

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);


private:
    void setBGImagePixel(const QPoint &pos);
    void setFGImagePixel(const QPoint &pos);
    void setBGImagePixel(int x, int y);
    void setFGImagePixel(int x, int y);

    QMap<int, QRgb>* colors;
    QUndoStack* undo;
    QColor bgColor, fgColor, selColor;
    QImage background, foreground;
    int xasc, yasc; // ascii size
    int xsize, ysize; // actual physical pixel size for the rects
    asc_text text; // the ascii text
    bool showGrid; // whether to display 1-pixel black grid between rects
    int lastx, lasty; // last ascii cell selected
    QList<std::pair<BrushType, Brush*> > brushes;
    QList<std::pair<BrushType, Brush*> >::iterator current_brush;
};

#endif // MAINWIDGET_H
