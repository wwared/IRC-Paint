#include "IRCPaintMainWindow.h"
#include "MainWidget.h"

IRCPaintMainWindow::IRCPaintMainWindow() {
    MainWidget* dicks = new MainWidget(this);
    setCentralWidget(dicks);
}
