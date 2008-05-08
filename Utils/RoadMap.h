/*
 *  LusionEngine
 *
 *  Created by Erik Engheim on 6.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
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
