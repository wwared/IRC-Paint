#ifndef DOCKTITLE_H
#define DOCKTITLE_H

#include <QWidget>
#include <QIcon>
class QString;

class DockTitle : public QWidget {
    Q_OBJECT
public:
    explicit DockTitle(const QString& fname, QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent*);

private:
    QIcon icon;
};

#endif // DOCKTITLE_H
