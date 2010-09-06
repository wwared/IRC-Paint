#-------------------------------------------------
#
# Project created by QtCreator 2010-07-12T19:32:25
#
#-------------------------------------------------

QT       += core gui

TARGET = IRCPaint
TEMPLATE = app

macx:ICON = IRCPaint.icns
win32:RC_FILE = IRCPaint.rc

SOURCES += main.cpp\
    MainWidget.cpp \
    Brush.cpp \
    Brush_Pen.cpp \
    Brush_Line.cpp \
    Brush_Rect.cpp \
    MainWindow.cpp \
    Palette.cpp \
    ColorSwatch.cpp \
    ColorPicker.cpp

HEADERS  += \
    MainWidget.h \
    Brush.h \
    Brush_Pen.h \
    Brush_Line.h \
    Brush_Rect.h \
    MainWindow.h \
    Palette.h \
    ColorSwatch.h \
    ColorPicker.h

OTHER_FILES += \
    IRCPaint.rc

RESOURCES += \
    IRCPaint.qrc
