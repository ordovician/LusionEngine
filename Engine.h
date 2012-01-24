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

#pragma once

#include <Geometry/Vector2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>

#include "Timing.h"
#include "Types.h"

#include <QtCore/QSize>

class Shape;
class Sprite;
class CircleShape;
class RectShape2;
class SegmentShape2;
class View;

//#define USE_TEXTURES

void engineInit();

// Constructors
Sprite*        createSprite();
Sprite*        createSprite(View* view);
Sprite*        createSprite(const Point2& aPos, real aDir, real aSpeed);
CircleShape*   createCircle(const Circle& circle);
RectShape2*    createRect2(const Rect2& rect);
SegmentShape2* createSegment2(const Segment2& seg);

// Accessors
bool isDone();
void setDone(bool done);
void setViewportHeight(int height);
void setViewportWidth(int width);
void setViewport(int width, int height);
void setViewport(const QSize size);
QSize getViewport();
void setWorldView(const Rect2& rect);
void lookAt(const Point2& p);
Rect2 worldView();

int     ticksLeft(int start_ticks);
void    startTimer();
int     stopTimer();

// Request
bool stateOfKey(int key);

// Operations
void renderFrame(real start_time);
void engineEndLoop(real start_time);
