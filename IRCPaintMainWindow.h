#ifndef IRCPAINTMAINWINDOW_H
#define IRCPAINTMAINWINDOW_H

#include <QMainWindow>

#include "MainWidget.h"

class IRCPaintMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit IRCPaintMainWindow();
    bool exportToTxt(const QString& fname);
    bool exportToTerminal(const QString& fname);
    bool exportToHtml(const QString& fname);
    bool importFromTxt(const QString& fname);
    bool importFromImg(const QString& fname, int maxWidth, bool smooth); // maxWidth is the maximum number of ascii cells; if smooth is true, use bilinear interpolation while scaling down
    int rgbToIrc(QRgb c);
    QRgb ircToRgb(int i);
    QString rgbToHtml(QRgb c);
    QString ircToTerminal(int i, bool bg);
    void swapIrcColor(int i, QRgb c);

protected:
    //void closeEvent(QCloseEvent *);

private:
    struct Lab {
        double l, a, b;
    };
    Lab rgbToLab(const QRgb& c);
    QRgb closestColor(const QRgb& c, const QMap<int, Lab>& labColors);

    QMap<int, QRgb> colors;
    MainWidget* mwidget;
};

#endif // IRCPAINTMAINWINDOW_H
