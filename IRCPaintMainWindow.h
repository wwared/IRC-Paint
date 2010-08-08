#ifndef IRCPAINTMAINWINDOW_H
#define IRCPAINTMAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QApplication>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "MainWidget.h"

class IRCPaintMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit IRCPaintMainWindow();
    bool exportToTxt(const QString& fname, const QImage& background, const QImage& foreground, const QList<QList<QChar> >& text);
    int rgbToIrc(QRgb c);
    QRgb ircToRgb(int i);
    void swapIrcColor(int i, QRgb c);

private:
    QMap<int, QRgb> colors;
    MainWidget* mwidget;
};

#endif // IRCPAINTMAINWINDOW_H
