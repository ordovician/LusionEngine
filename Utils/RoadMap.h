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
 
class ClosestPointFinder : public CollisionAction
{
public:
  // Constructors
  ClosestPointFinder(Shape* obstacles);
  
  // Operations
  void discCollision(const Circle& circle, Shape* shape, Points2& points);
  bool execute(Shape* me, Shape* other, Points2& points, real start_time, real delta_time);
  bool nearestObstacle(const Point2& c, Point2& point_result);
  
private:
  real iT, iDt;
  Points2 iPoints;
  Shape* iObstacles;
};
