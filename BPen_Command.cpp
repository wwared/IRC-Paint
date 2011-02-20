#include <QtGui>

#include "BPen_Command.h"

#include "MainWidget.h"

BPen_Command::BPen_Command(MainWidget* w, QImage* i, QImage* o, int id, int x, int y, QRgb c) : curid(id), widget(w), img(i), other(o) {
    cells << std::make_pair(QPoint(x, y), c);
}

void BPen_Command::undo() {
    foreach (cell c, changed) {
        img->setPixel(c.first, c.second);
        widget->update(widget->pixelRect(c.first));
    }
    if (other) {
        foreach (cell c, otherChanged) {
            other->setPixel(c.first, c.second);
            widget->update(widget->pixelRect(c.first));
        }
    }
}

void BPen_Command::redo() {
    changed.clear();
    otherChanged.clear();
    foreach (cell c, cells) {
        changed << std::make_pair(c.first, img->pixel(c.first));
        img->setPixel(c.first, c.second);
        if (other) {
            otherChanged << std::make_pair(c.first, other->pixel(c.first));
            other->setPixel(c.first, c.second);
        }
        widget->update(widget->pixelRect(c.first));
    }
}

bool BPen_Command::mergeWith(const QUndoCommand* other) {
    BPen_Command* cmd = dynamic_cast<BPen_Command*>(const_cast<QUndoCommand*>(other));
    if (cells.size() > 500 || cmd->id() != id() || !cmd)
        return false;
    foreach (cell c, cmd->cells) {
        foreach (cell h, cells) {
            if (c.first == h.first) {
                if (c.second == h.second)
                    return true;
                return false;
            }
        }
        cells << c;
    }
    foreach (cell c, cmd->changed)
        changed << c;
    foreach (cell c, cmd->otherChanged)
        otherChanged << c;
    return true;
}
