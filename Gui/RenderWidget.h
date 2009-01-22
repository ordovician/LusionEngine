#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QtOpenGL/QGLWidget>

class RenderWidget : public QGLWidget
{
public:
    RenderWidget(QWidget* parent = 0);
    ~RenderWidget();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void timerEvent ( QTimerEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
};

#endif // RENDERWIDGET_H
