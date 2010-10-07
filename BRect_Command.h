#ifndef BRECT_COMMAND_H
#define BRECT_COMMAND_H

#include <utility>

#include <QUndoCommand>
#include <QRgb>
#include <QPoint>
#include <QList>

class QImage;
class MainWidget;

class BRect_Command : public QUndoCommand {
    int xstart, ystart, xend, yend;
    QRgb col;
    QImage* img;
    MainWidget* widget;
    typedef std::pair<QPoint, QRgb> cell;
    QList<cell> changed;
public:
    BRect_Command(MainWidget* w, QImage* i, QRgb c, int xs, int ys, int xe, int ye) : xstart(xs), ystart(ys), xend(xe), yend(ye), col(c), img(i), widget(w) { }
    void undo();
    void redo();
};

#endif // BRECT_COMMAND_H
