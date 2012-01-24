/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
