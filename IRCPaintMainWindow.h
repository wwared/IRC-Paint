#ifndef IRCPAINTMAINWINDOW_H
#define IRCPAINTMAINWINDOW_H

#include <QMainWindow>

#include "MainWidget.h"

class IRCPaintMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit IRCPaintMainWindow();
    bool exportToTxt(const QString& fname);
    bool importFromTxt(const QString& fname);
    bool importFromImg(const QString& fname, int maxWidth); // maxWidth is the maximum number of ascii cells
    int rgbToIrc(QRgb c);
    QRgb ircToRgb(int i);
    void swapIrcColor(int i, QRgb c);

protected:
    //void closeEvent(QCloseEvent *);

private:
    QRgb closestColor(const QRgb& c);

    QMap<int, QRgb> colors;
    MainWidget* mwidget;
};

#endif // IRCPAINTMAINWINDOW_H
