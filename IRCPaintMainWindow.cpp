#include "IRCPaintMainWindow.h"

#include <QtGui/QApplication>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <cmath>

IRCPaintMainWindow::IRCPaintMainWindow() {
    colors[0]  = qRgb(255,255,255);
    colors[1]  = qRgb(0  ,0  ,  0);
    colors[2]  = qRgb(0  ,0  ,127);
    colors[3]  = qRgb(0  ,147,  0);
    colors[4]  = qRgb(255,0  ,  0);
    colors[5]  = qRgb(127,0  ,  0);
    colors[6]  = qRgb(156,0  ,156);
    colors[7]  = qRgb(252,127,  0);
    colors[8]  = qRgb(255,255,  0);
    colors[9]  = qRgb(0  ,252,  0);
    colors[10] = qRgb(0  ,147,147);
    colors[11] = qRgb(0  ,255,255);
    colors[12] = qRgb(0  ,0  ,252);
    colors[13] = qRgb(255,0  ,255);
    colors[14] = qRgb(127,127,127);
    colors[15] = qRgb(210,210,210);
    mwidget =  new MainWidget(this, &colors);
    setCentralWidget(mwidget);
    QApplication::setWindowIcon(QIcon(":/IRCPaint.png"));
}

bool IRCPaintMainWindow::exportToTxt(const QString& fname) {
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("IRC Paint"), tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QImage background = mwidget->getBGImage();
    QImage foreground = mwidget->getFGImage();
    QList<QList<QChar> > text = mwidget->getText();
    int x = 0;
    int y = 0;
    foreach(QList<QChar> l, text) {
        QRgb bg = qRgb(0,0,0);
        QRgb fg = qRgb(0,0,0);
        bool first = true;
        foreach(QChar c, l) {
            if (c == mwidget->transparentCh)
                c = ' ';
            QRgb oldfg = fg;
            fg = foreground.pixel(x,y);
            QRgb oldbg = bg;
            bg = background.pixel(x,y);
            if (bg != oldbg || first) { // background color changes, ^Cf,b (or first character in a line)
                int b = rgbToIrc(bg);
                if (c.isDigit() && b < 10) {
                    out << "" << rgbToIrc(fg) << ",0" << b;
                } else {
                    out << "" << rgbToIrc(fg) << "," << b;
                }
            } else if (fg != oldfg) { // only foreground color changes, ^Cf
                int f = rgbToIrc(fg);
                if (c.isDigit() && f < 10) {
                    out << "0" << f;
                } else {
                    out << "" << f;
                }
            }
            out << c;
            ++x;
            first = false;
        }
        out << "\n";
        x = 0;
        ++y;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

IRCPaintMainWindow::Lab IRCPaintMainWindow::rgbToLab(const QRgb &c) {
    double r = double(qRed(c))   / 255.0;
    double g = double(qGreen(c)) / 255.0;
    double b = double(qBlue(c))  / 255.0;

#define CONVENIENCE(x) if (x > 0.04045) { \
        x = std::pow(((x+0.055)/1.055), 2.4); \
    } else { \
        x /= 12.92; \
    }
    CONVENIENCE(r)
    CONVENIENCE(g)
    CONVENIENCE(b)
#undef CONVENIENCE
    r *= 100.0;
    g *= 100.0;
    b *= 100.0;

    double x = ((r * 0.4124) + (g * 0.3576) + (b * 0.1805)) / 95.047;
    double y = ((r * 0.2126) + (g * 0.7152) + (b * 0.0722)) / 100.0;
    double z = ((r * 0.0193) + (g * 0.1192) + (b * 0.9505)) / 108.883;

#define CONVENIENCE(x) if (x > 0.008856) { \
        x = std::pow(x, 1.0/3.0); \
    } else { \
        x = (7.787 * x) + (16.0 / 116.0); \
    }
    CONVENIENCE(x)
    CONVENIENCE(y)
    CONVENIENCE(z)
#undef CONVENIENCE

    Lab lab;
    lab.l = (116.0 * y) - 16.0;
    lab.a = 500.0 * (x - y);
    lab.b = 200.0 * (y - z);
    return lab;
}

int IRCPaintMainWindow::rgbToIrc(QRgb c) {
    int i = colors.key(c, -1);
    if (i == -1)
        qCritical("FATAL: colors does NOT contain irc representation of %d", c);
    return i;
}

QRgb IRCPaintMainWindow::ircToRgb(int i) {
    while (i > 15)
        i -= 15;
    return colors[i];
}

void IRCPaintMainWindow::swapIrcColor(int i, QRgb c) {
    while (i > 15)
        i -= 15;
    QRgb oldc = colors[i];
    colors[i] = c;
    mwidget->swapColor(oldc, c);
}
