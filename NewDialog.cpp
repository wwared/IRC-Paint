#include "NewDialog.h"

#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

NewDialog::NewDialog(QWidget *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {
    w = new QSpinBox(this);
    w->setMinimum(1);
    w->setValue(26);

    QLabel* lw = new QLabel(this);
    lw->setText(tr("Width:"));

    h = new QSpinBox(this);
    h->setMinimum(1);
    h->setValue(16);

    QLabel* lh = new QLabel(this);
    lh->setText(tr("Height:"));

    ok = new QPushButton(tr("&Ok"));
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));

    cancel = new QPushButton(tr("&Cancel"));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

    layout = new QGridLayout(this);
    layout->addWidget(lw, 0, 0, 1, 1);
    layout->addWidget(w, 0, 2, 1, 1);
    layout->addWidget(lh, 1, 0, 1, 1);
    layout->addWidget(h, 1, 2, 1, 1);
    layout->addWidget(ok, 2, 1, 1, 1);
    layout->addWidget(cancel, 2, 2, 1, 1);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("New file"));
}
