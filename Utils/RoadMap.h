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

#include <Base/Action.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Circle.hpp>

// Forward references
class Shape;

// Functions
Shape* minkowskiSum(const Shape* group);
 
class ClosestPointFinder : public CollisionAction
{
public:
  // Constructors
  ClosestPointFinder(Shape* obstacles, const Rect2& bbox);
  
  // Operations
  void discCollision(const Circle& circle, Shape* shape, Points2& points);
  bool execute(Shape* me, Shape* other, Points2& points, real start_time, real delta_time);
  bool nearestObstacle(const Point2& c, Point2& point_result);
  bool equidistantVertex(const Vector2& c1, const Vector2&  c2, Vector2& c_v);
  bool retractSample(const Vector2& c, Vector2& c_v);
  
private:
  real iT, iDt;
  Points2 iPoints;
  Rect2   iBBox;
  Shape* iObstacles;
};
