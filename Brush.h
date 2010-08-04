#ifndef BRUSH_H
#define BRUSH_H

class MainWidget;
class QMouseEvent;
class QPaintEvent;

class Brush {
protected:
    MainWidget* widget;
public:
    Brush(MainWidget* w) : widget(w) {} // the x and y coordinates below are ascii coordinates, [0,xasc) and [0,yasc)
    virtual void onMouseClick(QMouseEvent* event, int x, int y); // Brush mouse events are only called when the paintable area is clicked
    virtual void onMouseRelease(QMouseEvent* event, int x, int y); // brush paint events are always called, but are not required to be implemented
    virtual void onMouseMove(QMouseEvent* event, int x, int y);
    virtual void onWidgetPaint(QPaintEvent* event);
};

#endif // BRUSH_H
