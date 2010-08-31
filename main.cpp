#include "MainWindow.h"

#include <QtGui/QApplication>

/* *
   * TODO:
   * * Actually implement the user interface
   * * Split the image importing code into its own dialog (finer selection of conversion methods and whatnot)
   * * Implement settings tracking (and a preferences dialog)
   * * Add more brushes (namely selection brush and fill brush)
   * * Backport ascii.pl transformations (and make it work with selection ranges)
   * * Implement undo/redo
   * * Export as an image (both raw size and a larger size)
   * * Implement clipboard
   * * Use signals and slots where applicable for communication between the main window, widgets and brushes
   * * Implement scrollbars in the main window
   * * Make sure irc paint doesn't lag like a fat bitch when working with large asciis
   * * Add further options for html exporting
   *
   * */

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
