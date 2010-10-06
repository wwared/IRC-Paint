#ifndef BFILL_COMMAND_H
#define BFILL_COMMAND_H

#include <utility>

#include <QUndoCommand>
#include <QColor>
#include <QList>
#include <QPoint>
#include <QRgb>

class QImage;
class MainWidget;

class BFill_Command : public QUndoCommand {
    int x, y;
    QRgb from, to;
    typedef std::pair<QPoint, QRgb> cell;
    QList<cell> changed;
    QList<QPoint> pixels;
    QImage* img;
    MainWidget* widget;
public:
    BFill_Command(MainWidget* w, QImage* i, QRgb f, QRgb t, int xx, int yy) : x(xx), y(yy), from(f), to(t), img(i), widget(w) { }
    void undo();
    void redo();
};

#endif // BFILL_COMMAND_H
