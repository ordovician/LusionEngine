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

#include "Types.h"

void gltTranslate(const Point2& pos);
void gltVertex(const Point2& pos);
void gltVertex(const Rect2& rect);
void gltVertex(const Segment2& segment);
void gltVertex(ConstPointIterator2 begin, ConstPointIterator2 end, const Point2& position, real angle);
void gltVertex(ConstPointIterator2 begin, ConstPointIterator2 end);

void drawCircle(double radius);
void draw(const Rect2& box);
void draw(const Points2& poly);


// Debug
void gltPerformanceTest(int n);
void trigPerformanceTest(int n);