#include <Geometry/Ray2.hpp>

/*!
    \class Ray2 Ray2.h
    \brief Describes a 2D ray.

    A ray has a start point and extends
    infinitly in one direction. In some sense it is a like a line
    segment with only one end point.

*/

#include <cmath>
#include <assert.h>

using namespace std;

// Constructors
Ray2::Ray2()
{
}

Ray2::Ray2(const Point2& origin, const Vector2& dir)
{
    iV[0] = origin;
    iV[1] = dir.unit();
}

// Accessors
Point2 Ray2::origin() const
{
  return iV[0];
}

Vector2 Ray2::direction() const
{
  return iV[1];
}

// Calculations
bool Ray2::intersect(const Segment2& seg) const
{
  Vector2 d = direction();
  Vector2 v = seg.toVector();
  
  Vector2 num = origin()-seg.source();
  real denom  = d.cross(v);
  
  // Check if line segments are parallel
  if (denom == 0.0f)
    return false;
  
  real t = v.cross(num)/denom;                                   
  real s = d.cross(num)/denom;

  // Check if line segments intersect
  if (s < 0.0f || s > 1.0f || t < 0.0f) 
    return false;
    
  return true;
}

// Line intersection algo from: http://www.cgafaq.info/wiki/Intersection_Of_2D_Line_Segments
/*!
  Returns true if segments 's1' and 's2' intersect. 'result' will then contain the intersection point.
  Otherwise 'result' is undefined. 's1' and 's2' is not considered to intersect if they are parallell. 
  
  Itersection test is not fully accurate. It might report full intersection even when line segments
  are parallell because it does not use exact numbers. 
  
*/
bool Ray2::intersection(const Segment2& seg, Vector2& result) const
{
  Vector2 d = direction();
  Vector2 v = seg.toVector();
  
  Vector2 num = origin()-seg.source();
  real denom  = d.cross(v);
  
  // Check if line segments are parallel
  if (denom == 0.0f)
    return false;
  
  real t = v.cross(num)/denom;                                   
  real s = d.cross(num)/denom;

  // Check if line segments intersect
  if (s < 0.0f || s > 1.0f || t < 0.0f) 
    return false;
    
  // Calculate intersection point
  result = origin() + direction()*t;
  return true;
}
 
/*!
  Find closest intersection point on polygon
*/
bool Ray2::intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Vector2& result) const
{
  assert(false); // TODO: Think this is buggy. Why substact origin() from v and why no test on end to begin part
  bool found_point = false;
  Vector2 v, v_prev;
  Point2  p, p_prev;
  ConstPointIterator2 it;
  for (it = begin+1; it != end; ++it) {
    if (!intersection(Segment2(*(it-1), *it), p)) continue;
    v = p - origin();
    if (v.squaredLength() >= v_prev.squaredLength()) continue;
    v_prev = v;
    p_prev = p;
    found_point = true;
  }
  result = p_prev;
  return found_point;
}
 
/*!
  Find number of times ray intersects polygon defined by iterators
*/
int Ray2::noIntersections(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  int n = 0;
  Vector2 v, v_prev;
  Point2  p, p_prev;
  ConstPointIterator2 it;
  for (it = begin+1; it != end; ++it) {
    if (intersect(Segment2(*(it-1), *it)))
      ++n;
  }
  if (intersect(Segment2(*(end-1), *begin)))
    ++n;
  return n;
}
 
real Ray2::squaredDistance(const Point2& p) const
{
  Vector2 v = p-origin();  
  real t = direction().dot(v);
  if (t < 0.0f)
      return v.squaredLength();
  real len = v.cross(direction());
  return len*len;
}
 
real Ray2::distance(const Point2& p) const
{
  Vector2 v = p-origin();  
  real t = direction().dot(v);
  if (t < 0.0f)
      return v.length();
  return abs(v.cross(direction()));
}

Point2 Ray2::nearestPoint(const Point2& p) const
{
  Vector2 v = p-origin();
  real t = direction().dot(v);
  if (t < 0.0f)
    return origin();
  
  return origin()+t*direction();
}

bool Ray2::intersect(const Rect2& r) const
{
  if (intersect(Segment2(r.bottomLeft(), r.topLeft())))
    return true;
    
  if (intersect(Segment2(r.topLeft(), r.topRight())))
    return true;

  if (intersect(Segment2(r.bottomLeft(), r.bottomRight())))
    return true;
    
  if (intersect(Segment2(r.bottomRight(), r.topRight())))
    return true;
    
  return false;
}
                       
// Operators
bool Ray2::operator==(const Ray2& r) const
{
  return origin() == r.origin() && direction() == r.direction();
}

bool Ray2::operator!=(const Ray2& r) const
{
  return origin() != r.origin() || direction() != r.direction();  
}