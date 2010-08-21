#include "IRCPaintMainWindow.h"

#include <QtGui/QApplication>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExp>

#include <QtDebug>

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

    importFromTxt("C:\\Users\\Admin\\Desktop\\ascii\\WB\\colors.txt");

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
            if (c.isNull())
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

bool IRCPaintMainWindow::importFromTxt(const QString& fname) {
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("IRC Paint"), tr("Cannot open file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    static const QRegExp stripCodes("[]|\\d{1,2}(?:,\\d{1,2})?");
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QStringList lines;
    int width = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lines << line;
        width = qMax(width, QString(line).replace(stripCodes, "").length());
    }
    file.close();
    int height = lines.size();
    QImage bg(width, height, QImage::Format_RGB32);
    bg.fill(colors[0]);
    QImage fg(width, height, QImage::Format_RGB32);
    fg.fill(colors[1]);
    QList<QList<QChar> > text;
    QRgb bgCol = colors[0];
    QRgb fgCol = colors[1];
    int x = 0;
    int y = 0;
    foreach (QString line, lines) {
        bgCol = colors[0];
        fgCol = colors[1];
        QList<QChar> tLine;
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == '') {                                                   // ^C
                i = qMin(i+1, line.length());
                if (line[i].isDigit()) {                                            // ^C#
                    i = qMin(i+1, line.length());
                    if (line[i].isDigit()) {                                        // ^C##
                        i = qMin(i+1, line.length());
                        if (line[i] == ',') {                                       // ^C##,
                            i = qMin(i+1, line.length());
                            if (line[i].isDigit()) {                                // ^C##,#
                                i = qMin(i+1, line.length());
                                if (line[i].isDigit()) {                            // ^C##,##
                                    fgCol = ircToRgb(line.mid(i-4,2).toInt());
                                    bgCol = ircToRgb(line.mid(i-1,2).toInt());
                                    continue;
                                }
                                --i;
                                fgCol = ircToRgb(line.mid(i-3,2).toInt());
                                bgCol = ircToRgb(line.mid(i,1).toInt());
                                continue;
                            }
                            --i;
                            fgCol = ircToRgb(line.mid(i-2,2).toInt());
                            --i;
                            continue;
                        }
                        --i;
                        fgCol = ircToRgb(line.mid(i-1,2).toInt());
                        continue;
                    } else if (line[i] == ',') {                                    // ^C#,
                        i = qMin(i+1, line.length());
                        if (line[i].isDigit()) {                                    // ^C#,#
                            i = qMin(i+1, line.length());
                            if (line[i].isDigit()) {                                // ^C#,##
                                bgCol = ircToRgb(line.mid(i-1,2).toInt());
                                fgCol = ircToRgb(line.mid(i-3,1).toInt());
                                continue;
                            }
                            --i;
                            bgCol = ircToRgb(line.mid(i,1).toInt());
                            fgCol = ircToRgb(line.mid(i-2,1).toInt());
                            continue;
                        }
                        --i;
                        fgCol = ircToRgb(line.mid(i-1,1).toInt());
                        --i;
                        continue;
                    }
                    --i;
                    fgCol = ircToRgb(line.mid(i,1).toInt());
                    continue;
                }
                --i;
                fgCol = ircToRgb(1);
                bgCol = ircToRgb(0);
                continue;
            }
            if (line[i].isPrint() && line[i] != '' && line[i] != '' && line[i] != '' && line[i] != '' && line[i] != '') {
                bg.setPixel(x, y, bgCol);
                fg.setPixel(x, y, fgCol);
                tLine << line[i];
                ++x;
            }
        }
        while (tLine.size() < width) {
            tLine << '\0';
        }
        text << tLine;
        ++y;
        x = 0;
    }
    mwidget->swapAscii(width, height, text, bg, fg);
    QApplication::restoreOverrideCursor();
    return true;
}

QRgb IRCPaintMainWindow::closestColor(const QRgb &c) {
    double shortestDistance;
    int index;
    for (int i = 0; i < 16; ++i) {
        long rmean = ((long)qRed(colors[i]) - (long)qRed(c)) / 2L;
        long r = (long)qRed(colors[i])   - (long)qRed(c);
        long g = (long)qGreen(colors[i]) - (long)qGreen(c);
        long b = (long)qBlue(colors[i])  - (long)qBlue(c);
        double distance = std::sqrt((double)((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8)));
        if (i == 0 || distance < shortestDistance) {
            shortestDistance = distance;
            index = i;
        }
    }
    return colors[index];
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
