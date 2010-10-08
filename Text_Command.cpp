#include "Text_Command.h"

#include "MainWidget.h"

Text_Command::Text_Command(MainWidget *w, int x, int y, QChar c) : widget(w) {
    chars << std::make_pair(QPoint(x, y), c);
}

void Text_Command::undo() {
    foreach (cell c, changed) {
        widget->text[c.first.y()][c.first.x()] = c.second;
        widget->update(widget->pixelRect(c.first));
    }
}

void Text_Command::redo() {
    changed.clear();
    foreach (cell c, chars) {
        changed << std::make_pair(c.first, widget->text[c.first.y()][c.first.x()]);
        widget->text[c.first.y()][c.first.x()] = c.second;
        widget->update(widget->pixelRect(c.first));
    }
}

bool Text_Command::mergeWith(const QUndoCommand* other) {
    Text_Command* cmd = dynamic_cast<Text_Command*>(const_cast<QUndoCommand*>(other));
    if (chars.size() > 500 || cmd->id() != id() || !cmd)
        return false;
    foreach (cell c, cmd->chars) {
        foreach (cell h, chars) {
            if (c.first == h.first) {
                if (c.second == h.second)
                    return true;
                return false;
            }
        }
        chars << c;
    }
    foreach (cell c, cmd->changed)
        changed << c;
    return true;
}
