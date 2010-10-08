#ifndef BPEN_COMMAND_H
#define BPEN_COMMAND_H

#include <QUndoCommand>
#include <QPoint>
#include <QRgb>

class MainWidget;
class QImage;

class BPen_Command : public QUndoCommand {
    typedef std::pair<QPoint, QRgb> cell;
    int curid;
    MainWidget* widget;
    QImage* img;
public:
    QList<cell> cells;
    QList<cell> changed;

    BPen_Command(MainWidget* w, QImage* i, int id, int x, int y, QRgb c);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* other);
    int id() const { return curid; }
};

#endif // BPEN_COMMAND_H
