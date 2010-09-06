#include "BrushList.h"

#include <QGridLayout>
#include <QActionGroup>
#include <QToolButton>

BrushList::BrushList(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);

    bPenAct = new QAction(tr("Pencil"), this);
    bPenAct->setIcon(QIcon(":/icons/pencil.png"));
    bPenAct->setCheckable(true);
    bPenAct->setData(BrushT_Pen);
    connect(bPenAct, SIGNAL(triggered()), this, SLOT(buttonClicked()));
    bPenBut = new QToolButton(this);
    bPenBut->setDefaultAction(bPenAct);

    bLineAct = new QAction(tr("Line"), this);
    bLineAct->setIcon(QIcon(":/icons/line.png"));
    bLineAct->setCheckable(true);
    bLineAct->setData(BrushT_Line);
    connect(bLineAct, SIGNAL(triggered()), this, SLOT(buttonClicked()));
    bLineBut = new QToolButton(this);
    bLineBut->setDefaultAction(bLineAct);

    bRectAct = new QAction(tr("Rectangle"), this);
    bRectAct->setIcon(QIcon(":/icons/rect.png"));
    bRectAct->setCheckable(true);
    bRectAct->setData(BrushT_Rect);
    connect(bRectAct, SIGNAL(triggered()), this, SLOT(buttonClicked()));
    bRectBut = new QToolButton(this);
    bRectBut->setDefaultAction(bRectAct);

    group = new QActionGroup(this);
    group->setExclusive(true);
    group->addAction(bPenAct);
    group->addAction(bLineAct);
    group->addAction(bRectAct);

    layout = new QGridLayout(this);
    layout->addWidget(bPenBut, 0, 0);
    layout->addWidget(bLineBut, 0, 1);
    layout->addWidget(bRectBut, 1, 0);
    layout->setRowStretch(99, 1);

    bPenAct->trigger();
}

void BrushList::buttonClicked() {
    QAction* a = qobject_cast<QAction*>(sender());
    if (a) {
        emit brushSelected((BrushType)a->data().toInt());
    }
}
