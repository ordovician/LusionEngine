#include <Geometry/Circle.hpp>

/*!
    \class Circle Circle.h
    \brief Describes a 2d line segment.

*/

#include <cmath>

using namespace std;

// Constructors
Circle::Circle()
{
}

Circle::Circle(const Point2& center, real radius) : iCenter(center), iRadius(radius)
{
}

// Accessors
Point2 Circle::center() const
{
  return iCenter;
}

real Circle::radius() const
{
  return iRadius;
}

void Circle::setCenter(const Point2& center)
{
  iCenter = center;
}

void Circle::setRadius(real radius)
{
  iRadius = radius;
}
     
// Request
bool Circle::inside(const Point2& p) const
{
  return (p-iCenter).length() < iRadius;
}

/*!
  Test for intersection assuming that both circles are solid.
*/ 
bool Circle::intersect(const Circle& c) const
{
  return (c.center()-iCenter).length() < c.radius() + iRadius;
}

/*!
  Test for intersection assuming that both circle and rectangle are solid.
  Algorithm from: http://tog.acm.org/GraphicsGems/gems/CircleRect.c
*/        
bool Circle::intersect(const Rect2& rect) const
{    
  real r2 = iRadius*iRadius;
  
  // Translate coordinate system placing circle at center
  Vector2 rmax = rect.max() - iCenter;
  Vector2 rmin = rect.min() - iCenter;
  
  // Left of circle center?
  if (rmax.x() < 0.0f) {      
    if (rmax.y() < 0.0f)      // Lower left corner
      return rmax.squaredLength() < r2;
    else if (rmin.y() > 0.0f) // Upper left corner
      return rmax.x()*rmax.x() + rmin.y()*rmin.y() < r2;
    else                      // Is left of circle
      return abs(rmax.x()) < iRadius;
  }
  // Right of circle center?
  else if (rmin.x() > 0.0f) {
    if (rmax.y() < 0.0f)      // Lower right corner
      return rmin.x()*rmin.x() + rmax.y()*rmax.y() < r2;
    else if (rmin.y() > 0.0f) // Upper right corner
      return rmin.squaredLength() < r2;
    else                      // Is right of circle
      return rmin.x() < iRadius;
  }
  // rectangle on circle vertical centerline
  else {
    if (rmax.y() < 0.0f)      // Rect below circle
      return abs(rmax.y()) < iRadius;
    else if (rmin.y() > 0.0f) // Rect above circle
      return rmin.y() < iRadius;
    else                      // Rect contains circle center
      return true;            
  }
  
  return false;
}

bool Circle::intersect(const Segment2& seg) const
{
  return seg.distance(center()) < radius();
}

bool Circle::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  ConstPointIterator2 it = begin, prev = begin;
  for (++it; it != end; ++it, ++prev) {
    if (intersect(Segment2(*prev, *it)))
      return true;    
  }
  return false;
}
    
// Operators
bool Circle::operator==(const Circle& circle) const
{
  return center() == circle.center() && radius() == circle.radius();
}

bool Circle::operator!=(const Circle& circle) const
{
  return center() != circle.center() || radius() != circle.radius();
}