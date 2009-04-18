#include "RenderWidget.h"

#include "Engine.h"
#include "Timing.h"

#include "Lua/LuaEngine.h"

#include <QtGui/QKeyEvent>
#include <QtDebug>

RenderWidget::RenderWidget(QWidget* parent)
  : QGLWidget(parent)
{
  setFocus();
}

RenderWidget::~RenderWidget()
{
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
  updateGL(); // calls paintGL() indirectly
  engineEndLoop(secs);
}

void RenderWidget::keyPressEvent ( QKeyEvent * event )
{
  unsigned int k = event->key();
  luaSetEngineBoolean("keystate", k, true);
}

void RenderWidget::keyReleaseEvent ( QKeyEvent * event )
{
  unsigned int k = event->key();
  luaSetEngineBoolean("keystate", k, false);
}
