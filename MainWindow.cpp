#include "MainWindow.h"

#include <QtGui/QApplication>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExp>
#include <QFileInfo>
#include <QScrollArea>
#include <QAction>
#include <QCloseEvent>
#include <QSettings>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QInputDialog>
#include <QDockWidget>
#include <QUndoStack>

#include <cmath>

#include "MainWidget.h"
#include "Palette.h"
#include "ColorPicker.h"
#include "BrushList.h"
#include "NewDialog.h"

MainWindow::MainWindow() : toolbarSize(16, 16), displayTitle(true) {
    colors[0]  = qRgb(255,255,255);
    colors[1]  = qRgb(0  ,0  ,0  );
    colors[2]  = qRgb(0  ,0  ,127);
    colors[3]  = qRgb(0  ,147,0  );
    colors[4]  = qRgb(255,0  ,0  );
    colors[5]  = qRgb(127,0  ,0  );
    colors[6]  = qRgb(156,0  ,156);
    colors[7]  = qRgb(252,127,0  );
    colors[8]  = qRgb(255,255,0  );
    colors[9]  = qRgb(0  ,252,0  );
    colors[10] = qRgb(0  ,147,147);
    colors[11] = qRgb(0  ,255,255);
    colors[12] = qRgb(0  ,0  ,252);
    colors[13] = qRgb(255,0  ,255);
    colors[14] = qRgb(127,127,127);
    colors[15] = qRgb(210,210,210);

    undo = new QUndoStack(this);
    connect(undo, SIGNAL(cleanChanged(bool)), this, SLOT(setCleanState(bool)));

    scroll = new QScrollArea(this);
    mwidget =  new MainWidget(this, &colors, undo);
    connect(mwidget, SIGNAL(somethingChanged(bool)), this, SLOT(setWindowModified(bool)));

    dock_b = new QDockWidget(tr("Brushes"), this);
    dock_b->setObjectName("Brush List");
    dock_b->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock_b->setFeatures(QDockWidget::DockWidgetMovable);

    blist = new BrushList(dock_b);
    connect(blist, SIGNAL(brushSelected(BrushType)), mwidget, SLOT(setBrush(BrushType)));

    dock_b->setWidget(blist);

    addDockWidget(Qt::RightDockWidgetArea, dock_b);

    dock_p = new QDockWidget(tr("Palette"), this);
    dock_p->setObjectName("Color Palette");
    dock_p->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock_p->setFeatures(QDockWidget::DockWidgetMovable);

    palette = new Palette(dock_p, &colors);
    connect(palette->picker, SIGNAL(bgColorPicked(int)), mwidget, SLOT(bgColorChanged(int)));
    connect(palette->picker, SIGNAL(fgColorPicked(int)), mwidget, SLOT(fgColorChanged(int)));

    dock_p->setWidget(palette);
    addDockWidget(Qt::RightDockWidgetArea, dock_p);

    for (int i = 0; i < maxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    clearRecentFilesAction = new QAction(tr("&Clear Recent Files"), this);
    connect(clearRecentFilesAction, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));

    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/icons/page_white.png"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/icons/folder.png"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/icons/disk.png"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setIcon(QIcon(":/icons/save_as.png"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setIcon(QIcon(":/icons/cross.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    undoAction = undo->createUndoAction(this, tr("&Undo"));
    undoAction->setIcon(QIcon(":/icons/arrow_undo.png"));
    undoAction->setShortcut(QKeySequence::Undo);

    redoAction = undo->createRedoAction(this, tr("&Redo"));
    redoAction->setIcon(QIcon(":/icons/arrow_redo.png"));
    redoAction->setShortcut(QKeySequence::Redo);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon(":/IRCPaint.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setIcon(QIcon(":/icons/qt.png"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    showGridAction = new QAction(tr("&Show Grid"), this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(mwidget->gridShown());
    connect(showGridAction, SIGNAL(toggled(bool)), mwidget, SLOT(setGrid(bool)));

    alternatePaintAction = new QAction(tr("&Alternate Paint"), this);
    alternatePaintAction->setCheckable(true);
    alternatePaintAction->setChecked(mwidget->paintMode());
    connect(alternatePaintAction, SIGNAL(toggled(bool)), mwidget, SLOT(setMode(bool)));

    exportAsHtmlAction = new QAction(tr("as &HTML"), this);
    exportAsHtmlAction->setIcon(QIcon(":/icons/html.png"));
    connect(exportAsHtmlAction, SIGNAL(triggered()), this, SLOT(exportAsHtml()));

    exportAsImgAction = new QAction(tr("as &Image"), this);
    exportAsImgAction->setIcon(QIcon(":/icons/image.png"));
    connect(exportAsImgAction, SIGNAL(triggered()), this, SLOT(exportAsImage()));

    exportAsTermAction = new QAction(tr("as ANSI/VT100 &Terminal"), this);
    exportAsTermAction->setIcon(QIcon(":/icons/application_xp_terminal.png"));
    connect(exportAsTermAction, SIGNAL(triggered()), this, SLOT(exportAsTerminal()));

    importImgAction = new QAction(tr("from &Image"), this);
    importImgAction->setIcon(QIcon(":/icons/image.png"));
    connect(importImgAction, SIGNAL(triggered()), this, SLOT(importImage()));

    addRowAction = new QAction(tr("&Add Row"), this);
    connect(addRowAction, SIGNAL(triggered()), mwidget, SLOT(addRows()));

    delRowAction = new QAction(tr("&Delete Row"), this);
    connect(delRowAction, SIGNAL(triggered()), mwidget, SLOT(delRows()));

    addColumnAction = new QAction(tr("&Add Column"), this);
    connect(addColumnAction, SIGNAL(triggered()), mwidget, SLOT(addColumns()));

    delColumnAction = new QAction(tr("&Delete Column"), this);
    connect(delColumnAction, SIGNAL(triggered()), mwidget, SLOT(delColumns()));

    newDialog = 0;

    fileMenu = menuBar()->addMenu(tr("&File"));
    QList<QAction*> actions;
    actions << newAction << openAction << saveAction << saveAsAction;
    fileMenu->addActions(actions);
    actions.clear();
    recentFileSeparatorAction = fileMenu->addSeparator();
    for (int i = 0; i < maxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    importMenu = fileMenu->addMenu(tr("&Import"));
    importMenu->addAction(importImgAction);
    exportMenu = fileMenu->addMenu(tr("&Export"));
    actions << exportAsHtmlAction << exportAsImgAction << exportAsTermAction;
    exportMenu->addActions(actions);
    actions.clear();
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    actions << showGridAction << alternatePaintAction;
    toolsMenu->addActions(actions);
    actions.clear();
    // preferences go here
    toolsMenu->addSeparator();
    rowMenu = toolsMenu->addMenu(tr("&Rows"));
    actions << addRowAction << delRowAction;
    rowMenu->addActions(actions);
    actions.clear();
    columnMenu = toolsMenu->addMenu(tr("&Columns"));
    actions << addColumnAction << delColumnAction;
    columnMenu->addActions(actions);
    actions.clear();

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    fileToolbar = addToolBar(tr("&File Toolbar"));
    fileToolbar->setObjectName("File Toolbar");
    actions << newAction << openAction << saveAction;
    fileToolbar->addActions(actions);
    actions.clear();

    scroll->setBackgroundRole(QPalette::Dark);
    scroll->setWidget(mwidget);
    setCentralWidget(scroll);
    readSettings();
    setWindowIcon(QIcon(":/IRCPaint.png"));
    setCurrentFile("");
}

bool MainWindow::okToContinue() {
    if (isWindowModified()) {
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("The document has been modified."));
        b.setInformativeText(tr("Do you want to save your changes?"));
        b.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        int h = b.exec();
        if (h == QMessageBox::Save) {
            return save();
        } else if (h == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::newFile() {
    if (okToContinue()) {
        if (!newDialog) {
            newDialog = new NewDialog(this);
        }
        if (newDialog->exec() == QDialog::Accepted) {
            undo->clear();
            mwidget->clearAscii(newDialog->getw(), newDialog->geth());
            setCurrentFile("");
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (okToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setCurrentFile(const QString& fname) {
    undo->setClean();
    curFile = fname;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = QFileInfo(curFile).fileName();
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFiles();
    }

    if (displayTitle) {
        setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(getName()));
    } else {
        setWindowTitle(tr("%1[*]").arg(shownName));
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About IRC Paint"),
                       tr("<h1>IRC Paint 0.1</h1>"
                          "<p>Licensed under the <a href=\"http://sam.zoy.org/wtfpl/\">WTFPL</a>.<br>"
                          "Source available at <a href=\"http://github.com/Pedrobear/IRC-Paint\">github</a>.</p>"
                          "<p>Thanks goes to <b>everyone</b>. I love you all.</p>"
                          "<p>This program is better than:"
                          "<ul>"
                          "<li><a href=\"http://www.ircpaint.com/\">ircpaint</a> by <i>taras</i></li>"
                          "<li><a href=\"http://code.google.com/p/asciipumper/\">Ascii Pumper</a> by <i>Lampiasis</i></li>"
                          "</ul></p>"));
}

void MainWindow::open() {
    if (okToContinue()) {
        QString fname = QFileDialog::getOpenFileName(this, tr("Open Ascii"), ".", tr("Ascii files (*.txt);;All files (*)"));
        if (!fname.isEmpty()) {
            undo->clear();
            importFromTxt(fname);
        }
    }
}

bool MainWindow::save() {
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return exportToTxt(curFile);
    }
}

bool MainWindow::saveAs() {
    QString fname = QFileDialog::getSaveFileName(this, tr("Save Ascii"), ".", tr("Ascii files (*.txt);;All files (*)"));
    if (fname.isEmpty())
        return false;
    return exportToTxt(fname);
}

QString MainWindow::getName() const {
#define H(x, y) case x: return y
    switch (qrand()%15) {
        H(0 , "h"); H(1 , "IRC Paint"); H(2 , "Hungry Fungus"); H(3 , "Ascii Pimper"); H(4 , "Calculator");
        H(5 , "Internet Explorer"); H(6 , "Mozilla Firefox"); H(7 , "Safari"); H(8 , "Google Chrome"); H(9 , "Opera");
        H(10, "mIRC"); H(11, "XChat"); H(12, "ChatZilla"); H(13, "IceChat"); H(14, "Notepad");
    default: return "h";
    }
#undef H
}

void MainWindow::readSettings() {
    QSettings settings("BR Software inc.", "IRC Paint");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    bool g = settings.value("showGrid", true).toBool();
    mwidget->setGrid(g);
    showGridAction->setChecked(g);
    setToolbarSize(settings.value("toolbarSize", toolbarSize).toSize());
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFiles();
    displayTitle = settings.value("displayTitle", displayTitle).toBool();
}

void MainWindow::writeSettings() {
    QSettings settings("BR Software inc.", "IRC Paint");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("showGrid", mwidget->gridShown());
    settings.setValue("toolbarSize", toolbarSize);
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("displayTitle", displayTitle);
}

void MainWindow::setToolbarSize(const QSize& s) {
    toolbarSize = s;
    fileToolbar->setIconSize(toolbarSize);
}

void MainWindow::setCleanState(bool b) {
    setWindowModified(!b);
}

void MainWindow::updateRecentFiles() {
    QStringList::iterator it;
    for (it = recentFiles.begin(); it != recentFiles.end(); ++it) {
        if (!QFile::exists(*it))
            recentFiles.erase(it);
    }
    for (int i = 0; i < maxRecentFiles; ++i) {
        if (i < recentFiles.count()) {
            recentFileActions[i]->setText(QString("&%1 %2").arg(i+1).arg(QFileInfo(recentFiles[i]).fileName()));
            recentFileActions[i]->setData(recentFiles[i]);
            recentFileActions[i]->setVisible(true);
        } else {
            recentFileActions[i]->setVisible(false);
        }
    }
    recentFileSeparatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::openRecentFile() {
    if (okToContinue()) {
        QAction* a = qobject_cast<QAction*>(sender());
        if (a) {
            undo->clear();
            importFromTxt(a->data().toString());
        }
    }
}

void MainWindow::clearRecentFiles() {
    recentFiles.clear();
    updateRecentFiles();
}

bool MainWindow::exportAsImage() {
    QMessageBox b;
    b.setIcon(QMessageBox::Question);
    b.setText(tr("Show grid in the exported image?"));
    b.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    bool g = (b.exec() == QMessageBox::Yes) ? true : false;
    QString fname = QFileDialog::getSaveFileName(this, tr("Export as Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp *.ppm *.tiff *.tif *.xbm *.xpm);;"
                                                                                      "Portable Network Graphics (*.png);;"
                                                                                      "Joint Photographic Experts Group (*.jpg *.jpeg);;"
                                                                                      "Windows Bitmap (*.bmp);;"
                                                                                      "Portable Pixmap (*.ppm);;"
                                                                                      "Tagged Image File Format (*.tiff *.tif);;"
                                                                                      "X11 Bitmap (*.xbm);;"
                                                                                      "X11 Pixmap (*.xpm)"));
    if (fname.isEmpty())
        return false;
    return exportToImg(fname, g);
}

bool MainWindow::exportAsHtml() {
    QString fname = QFileDialog::getSaveFileName(this, tr("Export as HTML"), ".", tr("HTML Files (*.html *.htm);;All files (*)"));
    if (fname.isEmpty())
        return false;
    return exportToHtml(fname);
}

bool MainWindow::exportAsTerminal() {
    QString fname = QFileDialog::getSaveFileName(this, tr("Export as ANSI/VT100 Terminal file"), ".", tr("Text Files (*.txt);;All files (*)"));
    if (fname.isEmpty())
        return false;
    return exportToTerminal(fname);
}

void MainWindow::importImage() {
    if (okToContinue()) {
        QMessageBox b;
        b.setIcon(QMessageBox::Question);
        b.setText(tr("Use bilinear filtering?"));
        b.setInformativeText(tr("May result in a smoother ascii when the image has to be scaled down."));
        b.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        bool s = (b.exec() == QMessageBox::Yes) ? true : false;
        int w = QInputDialog::getInt(this, tr("IRC Paint"), tr("Maximum width for the resulting ascii (in ascii cells):"), 70, 1);
        QString fname = QFileDialog::getOpenFileName(this, tr("Import from Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.gif *.bmp *.pbm *.pgm *.ppm *.tiff *.tif *.xbm *.xpm);;"
                                                                                            "Portable Network Graphics (*.png);;"
                                                                                            "Joint Photographic Experts Group (*.jpg *.jpeg);;"
                                                                                            "Graphic Interchange Format (*.gif);;"
                                                                                            "Windows Bitmap (*.bmp);;"
                                                                                            "Portable Bitmap (*.pbm);;"
                                                                                            "Portable Graymap (*.pgm);;"
                                                                                            "Portable Pixmap (*.ppm);;"
                                                                                            "Tagged Image File Format (*.tiff *.tif);;"
                                                                                            "X11 Bitmap (*.xbm);;"
                                                                                            "X11 Pixmap (*.xpm)"));
        if (!fname.isEmpty())
            importFromImg(fname, w, s);
    }
}

bool MainWindow::exportToTxt(const QString& fname) {
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("Cannot write file %1").arg(file.fileName()));
        b.setInformativeText(tr("%2").arg(file.errorString()));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QImage background = mwidget->getBGImage();
    QImage foreground = mwidget->getFGImage();
    QList<QList<QChar> > text = mwidget->getText();
    int x = 0;
    int y = 0;
    foreach(QList<QChar> l, text) {
        QRgb bg = qRgb(0,0,0);
        QRgb fg = qRgb(0,0,0);
        bool first = true;
        foreach(QChar c, l) {
            if (c.isNull())
                c = ' ';
            QRgb oldfg = fg;
            fg = foreground.pixel(x,y);
            QRgb oldbg = bg;
            bg = background.pixel(x,y);
            if (bg != oldbg || first) { // background color changes, ^Cf,b (or first character in a line)
                int b = rgbToIrc(bg);
                if (c.isDigit() && b < 10) {
                    out << "" << rgbToIrc(fg) << ",0" << b;
                } else {
                    out << "" << rgbToIrc(fg) << "," << b;
                }
            } else if (fg != oldfg) { // only foreground color changes, ^Cf
                int f = rgbToIrc(fg);
                if (c.isDigit() && f < 10) {
                    out << "0" << f;
                } else {
                    out << "" << f;
                }
            }
            out << c;
            ++x;
            first = false;
        }
        out << "\n";
        x = 0;
        ++y;
    }
    setCurrentFile(fname);
    QApplication::restoreOverrideCursor();
    return true;
}

bool MainWindow::exportToImg(const QString& fname, bool grid) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (!mwidget->getRenderedImage(grid).save(fname)) {
        QApplication::restoreOverrideCursor();
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("Cannot write file %1").arg(QFileInfo(fname).fileName()));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
        return false;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool MainWindow::exportToTerminal(const QString& fname) {
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("Cannot write file %1").arg(file.fileName()));
        b.setInformativeText(tr("%2").arg(file.errorString()));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QImage background = mwidget->getBGImage();
    QImage foreground = mwidget->getFGImage();
    QList<QList<QChar> > text = mwidget->getText();
    int x = 0;
    int y = 0;
    foreach(QList<QChar> l, text) {
        QRgb bg = qRgb(0,0,0);
        QRgb fg = qRgb(0,0,0);
        bool first = true;
        foreach(QChar c, l) {
            if (c.isNull())
                c = ' ';
            QRgb oldfg = fg;
            fg = foreground.pixel(x,y);
            QRgb oldbg = bg;
            bg = background.pixel(x,y);
            if (bg != oldbg || fg != oldfg || first) {
                if (!first)
                    out << "\033[0m";
                out << ircToTerminal(rgbToIrc(fg), false) << ircToTerminal(rgbToIrc(bg), true);
            }
            out << c;
            ++x;
            first = false;
        }
        out << "\033[0m\n";
        x = 0;
        ++y;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool MainWindow::exportToHtml(const QString& fname) {
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("Cannot write file %1").arg(file.fileName()));
        b.setInformativeText(tr("%2").arg(file.errorString()));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QImage background = mwidget->getBGImage();
    QImage foreground = mwidget->getFGImage();
    QList<QList<QChar> > text = mwidget->getText();
    int x = 0;
    int y = 0;
    QString filename = QFileInfo(file).baseName();
    out << QString("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
                   "<html>\n"
                   "    <head>\n"
                   "        <title>%1</title>\n"
                   "        <meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n"
                   "        <style type=\"text/css\">\n"
                   "            pre { margin: 0; }\n")
            .arg(filename[0].toUpper()+filename.right(filename.size()-1));
    for (int i = 0; i < 16; ++i) {
        out << QString("            .f%1 { color: %2; }\n"
                       "            .b%1 { background-color: %2; }\n")
                .arg(i).arg(rgbToHtml(colors[i]));
    }
    out <<  "        </style>\n"
            "    </head>\n"
            "    <body>\n\n"
            "<pre>\n";
    foreach(QList<QChar> l, text) {
        QRgb bg = qRgb(0,0,0);
        QRgb fg = qRgb(0,0,0);
        bool first = true;
        QString str;
        foreach(QChar c, l) {
            if (c.isNull()) {
                str = ' ';
            } else if (c == '<') {
                str = "&lt;";
            } else if (c == '>') {
                str = "&gt;";
            } else if (c == '&') {
                str = "&amp;";
            } else {
                str = c;
            }
            QRgb oldfg = fg;
            fg = foreground.pixel(x,y);
            QRgb oldbg = bg;
            bg = background.pixel(x,y);
            if (bg != oldbg || fg != oldfg || first) {
                if (!first)
                    out << "</span>";
                out << QString("<span class=\"f%1 b%2\">").arg(rgbToIrc(fg)).arg(rgbToIrc(bg));
            }
            out << str;
            ++x;
            first = false;
        }
        out << "</span>\n";
        x = 0;
        ++y;
    }
    out <<  "</pre>\n\n"
            "    </body>\n"
            "</html>";
    QApplication::restoreOverrideCursor();
    return true;
}

bool MainWindow::importFromTxt(const QString& fname) {
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("Cannot read file %1").arg(file.fileName()));
        b.setInformativeText(tr("%2").arg(file.errorString()));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
        return false;
    }
    static const QRegExp stripCodes("[]|\\d{1,2}(?:,\\d{1,2})?");
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QStringList lines;
    int width = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lines << line;
        width = qMax(width, line.replace(stripCodes, "").length());
    }
    file.close();
    int height = lines.size();
    QImage bg(width, height, QImage::Format_RGB32);
    bg.fill(colors[0]);
    QImage fg(width, height, QImage::Format_RGB32);
    fg.fill(colors[1]);
    QList<QList<QChar> > text;
    QRgb bgCol = colors[0];
    QRgb fgCol = colors[1];
    int x = 0;
    int y = 0;
    foreach (QString line, lines) {
        bgCol = colors[0];
        fgCol = colors[1];
        QList<QChar> tLine;
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == '') {                                                   // ^C
                i = qMin(i+1, line.length());
                if (line[i].isDigit()) {                                            // ^C#
                    i = qMin(i+1, line.length());
                    if (line[i].isDigit()) {                                        // ^C##
                        i = qMin(i+1, line.length());
                        if (line[i] == ',') {                                       // ^C##,
                            i = qMin(i+1, line.length());
                            if (line[i].isDigit()) {                                // ^C##,#
                                i = qMin(i+1, line.length());
                                if (line[i].isDigit()) {                            // ^C##,##
                                    fgCol = ircToRgb(line.mid(i-4,2).toInt());
                                    bgCol = ircToRgb(line.mid(i-1,2).toInt());
                                    continue;
                                }
                                --i;
                                fgCol = ircToRgb(line.mid(i-3,2).toInt());
                                bgCol = ircToRgb(line.mid(i,1).toInt());
                                continue;
                            }
                            --i;
                            fgCol = ircToRgb(line.mid(i-2,2).toInt());
                            --i;
                            continue;
                        }
                        --i;
                        fgCol = ircToRgb(line.mid(i-1,2).toInt());
                        continue;
                    } else if (line[i] == ',') {                                    // ^C#,
                        i = qMin(i+1, line.length());
                        if (line[i].isDigit()) {                                    // ^C#,#
                            i = qMin(i+1, line.length());
                            if (line[i].isDigit()) {                                // ^C#,##
                                bgCol = ircToRgb(line.mid(i-1,2).toInt());
                                fgCol = ircToRgb(line.mid(i-3,1).toInt());
                                continue;
                            }
                            --i;
                            bgCol = ircToRgb(line.mid(i,1).toInt());
                            fgCol = ircToRgb(line.mid(i-2,1).toInt());
                            continue;
                        }
                        --i;
                        fgCol = ircToRgb(line.mid(i-1,1).toInt());
                        --i;
                        continue;
                    }
                    --i;
                    fgCol = ircToRgb(line.mid(i,1).toInt());
                    continue;
                }
                --i;
                fgCol = ircToRgb(1);
                bgCol = ircToRgb(0);
                continue;
            }
            if (line[i].isPrint() && line[i] != '' && line[i] != '' && line[i] != '' && line[i] != '') {
                bg.setPixel(x, y, bgCol);
                fg.setPixel(x, y, fgCol);
                tLine << line[i];
                ++x;
            }
        }
        while (tLine.size() < width) {
            tLine << QChar();
        }
        text << tLine;
        ++y;
        x = 0;
    }
    mwidget->swapAscii(width, height, text, bg, fg);
    setCurrentFile(fname);
    QApplication::restoreOverrideCursor();
    return true;
}

bool MainWindow::importFromImg(const QString& fname, int maxWidth, bool smooth) {
    QImage image(fname);
    if (image.isNull()) {
        QMessageBox b;
        b.setIcon(QMessageBox::Warning);
        b.setText(tr("Cannot import file %1").arg(QFileInfo(fname).fileName()));
        b.setStandardButtons(QMessageBox::Ok);
        b.exec();
        return false;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QMap<int, Lab> labColors;
    for (int i = 0; i < 16; ++i)
        labColors[i] = rgbToLab(colors[i]);
    if (image.width() > maxWidth/2) {
        if (smooth) {
            image = image.scaledToWidth(maxWidth/2, Qt::SmoothTransformation);
        } else {
            image = image.scaledToWidth(maxWidth/2, Qt::FastTransformation);
        }
    }
    QImage irc(image.width()*2, image.height(), QImage::Format_RGB32);
    QImage fg = irc;
    fg.fill(colors[1]);
    for (int y = 0; y < image.height(); ++y) {
        int j = 0;
        for (int x = 0; x < image.width(); ++x) {
            QRgb color;
            if (qAlpha(image.pixel(x,y)) != 255) {
                color = colors[0];
            } else {
                color = closestColor(image.pixel(x, y), labColors);
            }
            irc.setPixel(x+j, y, color);
            ++j;
            irc.setPixel(x+j, y, color);
        }
    }
    QList<QList<QChar> > t;
    QList<QChar> tmp;
    for (int i = 0; i < irc.width(); ++i)
        tmp << QChar();
    for (int i = 0; i < irc.height(); ++i)
        t << tmp;
    mwidget->swapAscii(irc.width(), irc.height(), t, irc, fg);
    QApplication::restoreOverrideCursor();
    setCurrentFile("");
    return true;
}

QRgb MainWindow::closestColor(const QRgb& c, const QMap<int, Lab>& labColors) {
    double shortestDistance;
    int index;
    Lab col = rgbToLab(c);
    for (int i = 0; i < 16; ++i) {
        double distance = std::sqrt(std::pow(col.l - labColors[i].l, 2) + std::pow(col.a - labColors[i].a, 2) + std::pow(col.b - labColors[i].b, 2));
        if (i == 0 || distance < shortestDistance) {
            shortestDistance = distance;
            index = i;
        }
    }
    return colors[index];
}

MainWindow::Lab MainWindow::rgbToLab(const QRgb &c) {
    double r = double(qRed(c))   / 255.0;
    double g = double(qGreen(c)) / 255.0;
    double b = double(qBlue(c))  / 255.0;

#define CONVENIENCE(x) if (x > 0.04045) { \
        x = std::pow(((x+0.055)/1.055), 2.4); \
    } else { \
        x /= 12.92; \
    }
    CONVENIENCE(r)
    CONVENIENCE(g)
    CONVENIENCE(b)
#undef CONVENIENCE
    r *= 100.0;
    g *= 100.0;
    b *= 100.0;

    double x = ((r * 0.4124) + (g * 0.3576) + (b * 0.1805)) / 95.047;
    double y = ((r * 0.2126) + (g * 0.7152) + (b * 0.0722)) / 100.0;
    double z = ((r * 0.0193) + (g * 0.1192) + (b * 0.9505)) / 108.883;

#define CONVENIENCE(x) if (x > 0.008856) { \
        x = std::pow(x, 1.0/3.0); \
    } else { \
        x = (7.787 * x) + (16.0 / 116.0); \
    }
    CONVENIENCE(x)
    CONVENIENCE(y)
    CONVENIENCE(z)
#undef CONVENIENCE

    Lab lab;
    lab.l = (116.0 * y) - 16.0;
    lab.a = 500.0 * (x - y);
    lab.b = 200.0 * (y - z);
    return lab;
}

int MainWindow::rgbToIrc(QRgb c) {
    int i = colors.key(c, -1);
    if (i == -1)
        qCritical("FATAL: colors does NOT contain irc representation of %d", c);
    return i;
}

QRgb MainWindow::ircToRgb(int i) {
    while (i > 15)
        i -= 15;
    return colors[i];
}

QString MainWindow::rgbToHtml(QRgb c) {
    QString r = QString("%1").arg(qRed(c), 0, 16);
    if (r.length() == 1) {
        r = "0"+r;
    }
    QString g = QString("%1").arg(qGreen(c), 0, 16);
    if (g.length() == 1) {
        g = "0"+g;
    }
    QString b = QString("%1").arg(qBlue(c), 0, 16);
    if (b.length() == 1) {
        b = "0"+b;
    }
    return "#"+r+g+b;
}

QString MainWindow::ircToTerminal(int i, bool bg) {
    while (i > 15)
        i -= 15;
    int ret = bg ? 40 : 30;
    bool bright = false;
    switch (i) {
    case 0: // white
        bright = true;
    case 15: // light gray
        ret += 7;
        break;
    case 14: // dark grey
        bright = true;
    case 1: // black
        break;
    case 12: // blue
        bright = true;
    case 2: // dark blue
        ret += 4;
        break;
    case 9: // lime green
        bright = true;
    case 3: // dark green
        ret += 2;
        break;
    case 4: // red
        bright = true;
    case 5: // dark red
        ret += 1;
        break;
    case 13: // pink
        bright = true;
    case 6: // purple
        ret += 5;
        break;
    case 8: // yellow
        bright = true;
    case 7: // orange (no better choice)
        ret += 3;
        break;
    case 11: // sky blue
        bright = true;
    case 10: // teal
        ret += 6;
        break;
    default:
        qCritical("FATAL: unexpected fall through in irc -> terminal conversion");
    }
    return QString("\033[%1%2m").arg(bright ? (bg ? "5;" : "1;") : "").arg(ret);
}

void MainWindow::swapIrcColor(int i, QRgb c) {
    while (i > 15)
        i -= 15;
    QRgb oldc = colors[i];
    colors[i] = c;
    mwidget->swapColor(oldc, c);
}
