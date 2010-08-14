#include <QtGui>

#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent, QMap<int, QRgb>* colors) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFocusPolicy(Qt::StrongFocus);

    colors = colors;
    bgColor = (*colors)[1];
    fgColor = (*colors)[15];
    selColor = Qt::yellow;
    xsize = 10;
    ysize = 20;
    showGrid = true;
    xasc = 26;
    yasc = 16;
    lastx = 0;
    lasty = 0;

    brushes << std::make_pair(BrushT_Pen, new Brush_Pen(this)) << std::make_pair(BrushT_Line, new Brush_Line(this)) << std::make_pair(BrushT_Rect, new Brush_Rect(this));
    current_brush = brushes.begin();

    background = QImage(xasc, yasc, QImage::Format_RGB32);
    background.fill(qRgb(255, 255, 255));
    foreground = QImage(xasc, yasc, QImage::Format_RGB32);
    foreground.fill(qRgb(0, 0, 0));
    QList<QChar> l;
    for (int i = 0; i < xasc; ++i) {
        l << QChar(' ');
    }
    for (int i = 0; i < yasc; ++i) {
        text << l;
    }
}

MainWidget::~MainWidget() {
    typedef std::pair<BrushType, Brush*> brushPair;
    foreach(brushPair p, brushes)
        delete p.second;
}

void MainWidget::setGrid(bool g) {
    if (g != showGrid) {
        showGrid = g;
        update();
        updateGeometry();
    }
}

void MainWidget::setRectWidth(int x) {
    if (x < 1)
        x = 1;

    if (x != xsize) {
        xsize = x;
        update();
        updateGeometry();
    }
}

void MainWidget::setRectHeight(int y) {
    if (y < 1)
        y = 1;

    if (y != ysize) {
        ysize = y;
        update();
        updateGeometry();
    }
}

QSize MainWidget::sizeHint() const {
    QSize size(xsize*xasc, ysize*yasc);
    if (showGrid)
        size += QSize(1, 1);
    return size;
}

void MainWidget::mousePressEvent(QMouseEvent *event) {
    int i = event->pos().x() / xsize;
    int j = event->pos().y() / ysize;
    if (j >= yasc || j < 0 || i >= xasc || i < 0) {
        current_brush->second->onMouseClick(event, i, j, false);
        return;
    }
    int oldx = lastx;
    int oldy = lasty;
    lastx = i;
    lasty = j;
    update(pixelRect(oldx,oldy));
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        current_brush->second->onMouseClick(event, i, j, true);
    } else {
        QWidget::mousePressEvent(event);
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {
    int i = event->pos().x() / xsize;
    int j = event->pos().y() / ysize;
    if (j >= yasc || j < 0 || i >= xasc || i < 0) {
        current_brush->second->onMouseMove(event, i, j, false);
        return;
    }
    int oldx = lastx;
    int oldy = lasty;
    lastx = i;
    lasty = j;
    update(pixelRect(oldx,oldy));
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) {
        current_brush->second->onMouseMove(event, i, j, true);
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event) {
    int i = event->pos().x() / xsize;
    int j = event->pos().y() / ysize;
    if (j >= yasc || j < 0 || i >= xasc || i < 0) {
        current_brush->second->onMouseRelease(event, i, j, false);
        return;
    }
    lastx = i;
    lasty = j;
    update(pixelRect(i,j));
    current_brush->second->onMouseRelease(event, i, j, true);
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    QChar c = event->text()[0];
    if (c.isPrint()) {
        text[lasty][lastx] = c;
        int oldx = lastx;
        int oldy = lasty;
        if (++lastx >= xasc) {
            lastx = 0;
            if (++lasty >= yasc) {
                lasty = 0;
            }
        }
        update(pixelRect(oldx, oldy).united(pixelRect(lastx, lasty)));
    } else if (event->key() == Qt::Key_Delete) {
        text[lasty][lastx] = ' ';
        update(pixelRect(lastx, lasty));
    } else if (event->key() == Qt::Key_Backspace) {
        int oldx = lastx;
        int oldy = lasty;
        if (--lastx < 0) {
            lastx = xasc-1;
            if (--lasty < 0) {
                lasty = yasc-1;
            }
        }
        text[lasty][lastx] = ' ';
        update(pixelRect(oldx, oldy).united(pixelRect(lastx, lasty)));
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        int oldx = lastx;
        int oldy = lasty;
        lastx = 0;
        if (++lasty >= yasc) {
            lasty = 0;
        }
        update(pixelRect(oldx, oldy).united(pixelRect(lastx, lasty)));
    } else if (event->key() == Qt::Key_Up) {
        int oldy = lasty;
        if (--lasty < 0) {
            lasty = oldy;
        }
        update(pixelRect(lastx, oldy).united(pixelRect(lastx, lasty)));
    } else if (event->key() == Qt::Key_Down) {
        int oldy = lasty;
        if (++lasty >= yasc) {
            lasty = oldy;
        }
        update(pixelRect(lastx, oldy).united(pixelRect(lastx, lasty)));
    } else if (event->key() == Qt::Key_Left) {
        int oldx = lastx;
        if (--lastx < 0) {
            lastx = oldx;
        }
        update(pixelRect(oldx, lasty).united(pixelRect(lastx, lasty)));
    } else if (event->key() == Qt::Key_Right) {
        int oldx = lastx;
        if (++lastx >= xasc) {
            lastx = oldx;
        }
        update(pixelRect(oldx, lasty).united(pixelRect(lastx, lasty)));
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (showGrid) {
        painter.setPen(palette().windowText().color());
        for (int i = 0; i <= xasc; ++i)
            painter.drawLine(xsize * i, 0,
                                 xsize * i, ysize * yasc);
        for (int j = 0; j <= yasc; ++j)
            painter.drawLine(0, ysize * j,
                                 xsize * xasc, ysize * j);
    }

    for (int i = 0; i < xasc; ++i) {
        for (int j = 0; j < yasc; ++j) {
            QRect rect = pixelRect(i, j);
            if (!event->region().intersect(rect).isEmpty()) {
                painter.fillRect(rect, QColor::fromRgb(background.pixel(i, j)));
                painter.setPen(QColor::fromRgb(foreground.pixel(i,j)));
                painter.drawText(rect.translated(1,0), Qt::AlignCenter, text.at(j).at(i));
            }
        }
    }
    QRect lastRect = pixelRect(lastx, lasty).adjusted(0,0,-1,-1);
    if (!event->region().intersect(lastRect).isEmpty() && hasFocus()) {
        painter.setPen(selColor);
        painter.drawRect(lastRect);
    }
    current_brush->second->onWidgetPaint(event, painter);
}

void MainWidget::swapColor(QRgb c1, QRgb c2) {
    for (int y = 0; y < yasc; ++y) {
        for (int x = 0; x < xasc; ++x) {
            if (background.pixel(x,y) == c1) {
                background.setPixel(x, y, c2);
            }
            if (foreground.pixel(x,y) == c1) {
                foreground.setPixel(x, y, c2);
            }
        }
    }
    update();
}

void MainWidget::setBGImagePixel(const QPoint &pos) {
    int i = pos.x() / xsize;
    int j = pos.y() / ysize;

    if (background.rect().contains(i, j)) {
        background.setPixel(i, j, bgColor.rgb());
        update(pixelRect(i, j));
    }
}

void MainWidget::setFGImagePixel(const QPoint &pos) {
    int i = pos.x() / xsize;
    int j = pos.y() / ysize;

    if (foreground.rect().contains(i, j)) {
        foreground.setPixel(i, j, fgColor.rgb());
        update(pixelRect(i, j));
    }
}

void MainWidget::setBGImagePixel(int x, int y) {
    if (background.rect().contains(x, y)) {
        background.setPixel(x, y, bgColor.rgb());
        update(pixelRect(x, y));
    }
}

void MainWidget::setFGImagePixel(int x, int y) {
    if (foreground.rect().contains(x, y)) {
        foreground.setPixel(x, y, fgColor.rgb());
        update(pixelRect(x, y));
    }
}

QRect MainWidget::pixelRect(int i, int j) const {
    if (showGrid) {
        return QRect(xsize * i + 1, ysize * j + 1, xsize - 1, ysize - 1);
    } else {
        return QRect(xsize * i, ysize * j, xsize, ysize);
    }
}

void MainWidget::addRows(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > yasc)
        place = yasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    QList<QChar> textl;
    QList<QRgb> bgRow, fgRow;
    for (int i = 0; i < xasc; ++i) {
        textl << QChar(' ');
        bgRow << bgColor.rgb();
        fgRow << fgColor.rgb();
    }
    for (int i = 0; i < n; ++i) {
        text.insert(place, textl);
        bg.insert(place, bgRow);
        fg.insert(place, fgRow);
    }
    QImage newbg(xasc, yasc+n, QImage::Format_RGB32);
    QImage newfg(xasc, yasc+n, QImage::Format_RGB32);
    for (int j = 0; j < yasc+n; ++j) {
        for (int i = 0; i < xasc; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    yasc += n;
    update();
    updateGeometry();
}

void MainWidget::delRows(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > yasc)
        place = yasc;
    if (n > yasc)
        n = yasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    for (int i = 0; i < n; ++i) {
        text.removeAt(place);
        bg.removeAt(place);
        fg.removeAt(place);
    }
    QImage newbg(xasc, yasc-n, QImage::Format_RGB32);
    QImage newfg(xasc, yasc-n, QImage::Format_RGB32);
    for (int j = 0; j < yasc-n; ++j) {
        for (int i = 0; i < xasc; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    yasc -= n;
    update();
    updateGeometry();
}

void MainWidget::addColumns(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > xasc)
        place = xasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < yasc; ++j) {
            text[j].insert(place, QChar(' '));
            bg[j].insert(place, bgColor.rgb());
            fg[j].insert(place, fgColor.rgb());
        }
    }
    QImage newbg(xasc+n, yasc, QImage::Format_RGB32);
    QImage newfg(xasc+n, yasc, QImage::Format_RGB32);
    for (int j = 0; j < yasc; ++j) {
        for (int i = 0; i < xasc+n; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    xasc += n;
    update();
    updateGeometry();
}

void MainWidget::delColumns(int place, int n) {
    if (n <= 0)
        return;
    if (place < 0)
        place = 0;
    if (place > xasc)
        place = xasc;
    if (n > xasc)
        n = xasc;

    QList<QList<QRgb> > bg, fg;
    for (int j = 0; j < yasc; ++j) {
        QList<QRgb> a, b;
        for (int i = 0; i < xasc; ++i) {
            a << background.pixel(i,j);
            b << foreground.pixel(i,j);
        }
        bg << a;
        fg << b;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < yasc; ++j) {
            text[j].removeAt(place);
            bg[j].removeAt(place);
            fg[j].removeAt(place);
        }
    }
    QImage newbg(xasc-n, yasc, QImage::Format_RGB32);
    QImage newfg(xasc-n, yasc, QImage::Format_RGB32);
    for (int j = 0; j < yasc; ++j) {
        for (int i = 0; i < xasc-n; ++i) {
            newbg.setPixel(i, j, bg[j][i]);
            newfg.setPixel(i, j, fg[j][i]);
        }
    }
    background = newbg;
    foreground = newfg;
    xasc -= n;
    update();
    updateGeometry();
}

void MainWidget::setBrush(BrushType b) {
    typedef std::pair<BrushType, Brush*> brushPair;
    foreach (brushPair p, brushes) {
        if (p.first == b) {
            current_brush = std::find(brushes.begin(), brushes.end(), p);
            return;
        }
    }
}
