#ifndef BRUSH_H
#define BRUSH_H

class MainWidget;
class QMouseEvent;
class QPaintEvent;
class QPainter;

class Brush {
protected:
    MainWidget* widget;
public:
    Brush(MainWidget* w) : widget(w) {} // the x and y coordinates below are ascii coordinates, [0,xasc) and [0,yasc)
    virtual ~Brush() {}
    virtual void onMouseClick(QMouseEvent* event, int x, int y, bool insideWidget); // insideWidget is true if the event occurred in the paintable area
    virtual void onMouseRelease(QMouseEvent* event, int x, int y, bool insideWidget); // brush paint events are always called, but are not required to be implemented
    virtual void onMouseMove(QMouseEvent* event, int x, int y, bool insideWidget); // IMPORTANT: when insideWidget is true, the x and y values are INVALID and NOT SUPPOSED TO BE USED
    virtual void onWidgetPaint(QPaintEvent* event, QPainter& painter);
};

#endif // BRUSH_H
