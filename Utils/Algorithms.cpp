/*
 *  Algorithms.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 6.1.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Algorithms.h"
#include <cmath>

// struct Goodness : public unary_function<Point2, bool>
// {
//  Goodness(const Point2& aXe, const Point2& aVe) {
//    xe = aXe;
//    ve = aVe;
//  }
//  
//  bool operator()(const Point2& v1, const Point2& v2)
//  {
//    return 1 - xe.dot(xe-p)/(xe.length()*(xe-p).length());
//  }
// 
//  Point2 xe;
//  Vector2 ve;
// };
// 
// /*! 
//  Determine a tactical position 
// */
// Point2 findTacticalPosition(
//  const RoadMap& map, 
//  const Point2& xn,    // NPC position 
//  const Vector2& vn,   // NPC direction
//  const Point2& xe,    // Enemy position
//  const Vector2& ve)
// {
//   Vector2 d = (xe-xn0).length();
//   
//   WayPoints wayPoints;
//   insideCircle(map, wayPoints.begin(), back_inserter(wayPoints));
//   WayPoints::iterator end = convexHull(wayPoints.begin(), wayPoints.end());
//   max_element(wayPoints.end(), end, Goodness(xe));
// }