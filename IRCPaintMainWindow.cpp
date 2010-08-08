#include "IRCPaintMainWindow.h"

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
}

bool IRCPaintMainWindow::exportToTxt(const QString& fname, const QImage& background, const QImage& foreground, const QList<QList<QChar> >& text) {
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("IRC Paint"), tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int x = 0;
    int y = 0;
    foreach(QList<QChar> l, text) {
        QRgb bg = qRgb(1,2,3);
        QRgb fg = qRgb(1,2,3);
        foreach(QChar c, l) {
            bool fgchange = false;
            QRgb oldfg = fg;
            fg = foreground.pixel(x,y);
            if (fg != oldfg) {
                out << "" << rgbToIrc(fg);
                fgchange = true;
            }
            QRgb oldbg = bg;
            bg = background.pixel(x,y);
            if (bg != oldbg) {
                if (fgchange) {
                    out << "," << rgbToIrc(bg);
                } else {
                    out << "," << rgbToIrc(bg);
                }
            }
            out << c;
            ++x;
        }
        out << "\n";
        x = 0;
        ++y;
    }
    QApplication::restoreOverrideCursor();
    return true;
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
