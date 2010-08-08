#include "IRCPaintMainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    IRCPaintMainWindow w;
    w.show();

    return a.exec();
}
