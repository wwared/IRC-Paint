#include "MainWindow.h"

#include <QtGui/QApplication>

#include <ctime>

/* *
   * TODO:
   * * Actually implement the user interface (underway)
   * * Split the image importing code into its own dialog (finer selection of conversion methods and whatnot)
   * * Implement a preferences dialog
   * * Add more brushes (namely selection brush and fill brush)
   * * Backport ascii.pl transformations (and make it work with selection ranges)
   * * Implement undo/redo
   * * Implement clipboard
   * * Use signals and slots where applicable for communication between the main window, widgets and brushes
   * * Add further options for html exporting
   * * Configurable grid color (after preferences dialog is added)
   *
   * */

int main(int argc, char *argv[]) {
    qsrand(std::time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
