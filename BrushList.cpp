#include "BrushList.h"

#include <QGridLayout>
#include <QActionGroup>
#include <QToolButton>

BrushList::BrushList(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);

    bCursorAct = new QAction(tr("Cursor"), this);
    bCursorAct->setIcon(QIcon(":/icons/cursor.png"));
    bCursorAct->setCheckable(true);
    bCursorAct->setData(BrushT_Cursor);
    connect(bCursorAct, SIGNAL(triggered()), this, SLOT(buttonClicked()));
    bCursorBut = new QToolButton(this);
    bCursorBut->setDefaultAction(bCursorAct);

    bPenAct = new QAction(tr("Pencil"), this);
    bPenAct->setIcon(QIcon(":/icons/pencil.png"));
    bPenAct->setCheckable(true);
    bPenAct->setData(BrushT_Pen);
    connect(bPenAct, SIGNAL(triggered()), this, SLOT(buttonClicked()));
    bPenBut = new QToolButton(this);
    bPenBut->setDefaultAction(bPenAct);

    bFillAct = new QAction(tr("Fill"), this);
    bFillAct->setIcon(QIcon(":/icons/paintcan.png"));
    bFillAct->setCheckable(true);
    bFillAct->setData(BrushT_Fill);
    connect(bFillAct, SIGNAL(triggered()), this, SLOT(buttonClicked()));
    bFillBut = new QToolButton(this);
    bFillBut->setDefaultAction(bFillAct);

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
    group->addAction(bCursorAct);
    group->addAction(bPenAct);
    group->addAction(bFillAct);
    group->addAction(bLineAct);
    group->addAction(bRectAct);

    layout = new QGridLayout(this);
    layout->addWidget(bCursorBut, 0, 0);
    layout->addWidget(bPenBut, 0, 1);
    layout->addWidget(bFillBut, 1, 0);
    layout->addWidget(bLineBut, 1, 1);
    layout->addWidget(bRectBut, 2, 0);
    layout->setRowStretch(3, 1);

    bCursorAct->trigger();
}

void BrushList::buttonClicked() {
    QAction* a = qobject_cast<QAction*>(sender());
    if (a) {
        emit brushSelected((BrushType)a->data().toInt());
    }
}
