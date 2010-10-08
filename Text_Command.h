#ifndef TEXT_COMMAND_H
#define TEXT_COMMAND_H

#include <QUndoCommand>
#include <QPoint>
#include <QChar>

class MainWidget;

class Text_Command : public QUndoCommand {
    typedef std::pair<QPoint, QChar> cell;
    MainWidget* widget;
public:
    QList<cell> chars;
    QList<cell> changed;

    Text_Command(MainWidget* w, int x, int y, QChar c);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* other);
    int id() const { return 5; }
};

#endif // TEXT_COMMAND_H
