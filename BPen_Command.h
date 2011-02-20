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
    QImage* other;
public:
    QList<cell> cells;
    QList<cell> changed;
    QList<cell> otherChanged; // only used if other is not NULL

    BPen_Command(MainWidget* w, QImage* i, QImage* o, int id, int x, int y, QRgb c);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* other);
    int id() const { return curid; }
};

#endif // BPEN_COMMAND_H
