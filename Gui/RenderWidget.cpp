#include "RenderWidget.h"

#include "Engine.h"
#include "Timing.h"

#include <QtGui/QKeyEvent>

RenderWidget::RenderWidget(QWidget* parent)
  : QGLWidget(parent)
{
  // Makes this widget grab all keyboard input
  //grabKeyboard();
}

RenderWidget::~RenderWidget()
{
  //releaseKeyboard();
}

void RenderWidget::initializeGL()
{
  // Initialized Lua and OpenGL.
  // Lua script does some GL initialization which
  // is why it needs to be called here
  engineInit();

  startTimer(ticksPerFrame());     // 50-millisecond timer
}

void RenderWidget::resizeGL(int w, int h)
{
  // setup viewport, projection etc.:
  setViewport(w, h);
  glViewport(0, 0, (GLint)w, (GLint)h);
}

void RenderWidget::paintGL()
{
  renderFrame(secondsPassed());
}

void RenderWidget::timerEvent ( QTimerEvent * event )
{
  Q_UNUSED(event);
  real secs = secondsPassed();
  engineBeginLoop(secs);
  updateGL(); // calls paintGL() indirectly
  engineEndLoop(secs);
}

void RenderWidget::keyPressEvent ( QKeyEvent * event )
{
  unsigned int k = event->key();
  if (k < 256)
    setKeyState(k, 1);
  // QGLWidget::keyPressEvent ( QKeyEvent * event )
}

void RenderWidget::keyReleaseEvent ( QKeyEvent * event )
{
  unsigned int k = event->key();
  if (k < 256)
    setKeyState(k, 0);
  // QGLWidget::keyPressEvent ( QKeyEvent * event )
}
