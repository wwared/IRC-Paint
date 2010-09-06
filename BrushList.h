#ifndef BRUSHLIST_H
#define BRUSHLIST_H

#include "BrushType.h"
#include <QWidget>
class QActionGroup;
class QToolButton;
class QGridLayout;

class BrushList : public QWidget {
    Q_OBJECT
public:
    explicit BrushList(QWidget *parent = 0);

public slots:
    void buttonClicked();

signals:
    void brushSelected(BrushType);

private:
    QActionGroup* group;

    QToolButton* bPenBut;
    QAction* bPenAct;
    QToolButton* bLineBut;
    QAction* bLineAct;
    QToolButton* bRectBut;
    QAction* bRectAct;
    QGridLayout* layout;
};

#endif // BRUSHLIST_H
