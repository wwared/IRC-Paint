#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QSpinBox>

class QGridLayout;
class QLabel;
class QPushButton;

class NewDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewDialog(QWidget *parent = 0);
    int getw() { return w->value(); }
    int geth() { return h->value(); }

private:
    QGridLayout* layout;

    QSpinBox* w;
    QSpinBox* h;

    QPushButton* ok;
    QPushButton* cancel;
};

#endif // NEWDIALOG_H
