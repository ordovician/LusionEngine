/*
 *  LusionEngine
 *
 *  Created by Erik Engheim on 6.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Utils/RoadMap.h>
#include <Base/CircleShape.h>
#include <Engine.h>

#include <assert.h>

#include "Timing.h"

const real ACCURACY		=	0.5; // a measure for the accuracy during roadmap construction

// Constructors
ClosestPointFinder::ClosestPointFinder(Shape* obstacles) : iObstacles(obstacles) {
  assert(iObstacles != 0);
  iT = secondsPassed();
  iDt = 1.0;
}

// Operations
void ClosestPointFinder::discCollision(const Circle& circle, Shape* shape, Points2& points) 
{
  CircleShape c(circle);
  shape->collide(&c, iT, iDt, this);
  points = iPoints;
}

bool ClosestPointFinder::execute(Shape* me, Shape* other, Points2& points, real t, real dt) 
{
  iPoints = points;
  return true;
}

bool ClosestPointFinder::nearestObstacle(const Point2& c, Point2& point_result)
{
  bool is_collision = false;
  real radius = 0.0;
	
	Rect2 view = worldView();
  if (view.width() < view.height())
    radius = view.width();
  else 
    radius = view.height();
  
  real stepsize = radius;
  Points2 points;    
	while (stepsize > ACCURACY && radius > ACCURACY) {
    discCollision(Circle(c, radius), iObstacles, points);
    if (!points.empty()) {
      point_result = points[0];
      is_collision = true;
      stepsize = stepsize*0.5;
      radius = radius-stepsize;            
    }
    else {
      if (is_collision)
        stepsize = stepsize*0.5;
      else 
        stepsize = stepsize*2;
      radius = radius+stepsize;      
    }
	}
  return is_collision;
}