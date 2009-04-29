#include <Geometry/Segment2.hpp>
#include <cmath>

#include <iostream>

using namespace std;

/*!
    \class Segment2 Segment2.h
    \brief Describes a 2d line segment.

    The purpose is to do different geometric computations on line segments
    with each other or other geometric primitives. Instances of the class
    can not display itself on screen nor is the class meant to be subclassed.
*/

// Constructors
Segment2::Segment2() : iData(0)
{
}

Segment2::Segment2(const Point2& p, const Point2& q, void *data) : iData(data)
{
    iV[0] = p;
    iV[1] = q;
}

// Accessors
Point2 Segment2::source() const
{
  return iV[0];
}

Point2 Segment2::target() const
{
  return iV[1];
}

Point2 Segment2::vertex(int i) const
{
  return iV[i];
}

Point2 Segment2::side(int i) const
{
  return i == 0 ? left() : right();
}

/*! Returns left most endpoint or first endpoint if equal */
Point2 Segment2::left() const
{
  return iV[0].min(iV[1]);
}

/*! Returns right most endpoint or second endpoint if equal */
Point2 Segment2::right() const
{
  return iV[1].max(iV[0]);
}

real Segment2::xmin() const
{
  return min(iV[0].x(), iV[1].x());
}

real Segment2::ymin() const
{
  return min(iV[0].y(), iV[1].y());  
}

real Segment2::xmax() const
{
  return max(iV[0].x(), iV[1].x());  
}

real Segment2::ymax() const
{
  return max(iV[0].y(), iV[1].y());  
}

long Segment2::tag() const
{
  return (long)data();
}

void  Segment2::setTag(long tag)
{
  setData((void*)tag);
}

/*! 
  Segments typically need to refer to the next segment in a polygon 
  or the face that the segment bounds. 'data' is just a way to store
  any arbitrary information like that.
*/
void Segment2::setData(void *data)
{
  iData = data;
}

void* Segment2::data() const
{
  return iData;
}

// Request
bool Segment2::isDegenerate() const
{
  return iV[0] == iV[1];
}

/*! true if point \a p is below line defined by segment */
bool Segment2::isAbove(const Point2& p) const
{
  return (right()-left()).cross(p-left()) < 0;  
}

/*! true if point \a p is above line defined by segment */
bool Segment2::isBelow(const Point2& p) const
{
  return (right()-left()).cross(p-left()) > 0;
}

/*! true if point \a p is on line defined by segment */
bool Segment2::isOn(const Point2& p) const
{
  return (right()-left()).cross(p-left()) == 0;
}

bool Segment2::isOnEnd(const Point2& p) const
{
  return p == iV[0] || p == iV[1];
}

// Line intersection algo from: http://www.cgafaq.info/wiki/Intersection_Of_2D_Line_Segments
/*!
  Returns true if this segment intersect \a seg. 
    
  Line segments are defined at endpoints so 
  linesegments e.g. sharing endpoints are said to intersect, unless they are parallell.
  Parallell line segments are defined as not intersecting even if they overlap.  
  
  Itersection test is not fully accurate. It might report full intersection even when line segments
  are parallell because it does not use exact numbers.   
*/
bool Segment2::intersect(const Segment2& seg) const
{
  Vector2 v1 = toVector();
  Vector2 v2 = seg.toVector();
  
  Vector2 num = source()-seg.source();
  real denom = v1.cross(v2);  // distance from end of s1 to line with same 
                              // direction as s2 placed as start of s1
  // Check if line segments are parallel
  if (denom == 0.0f)
    return false;
  
  real r = v2.cross(num)/denom;   // denom is whole length, while v2.cross(num) is length 
                                  // from s1 start to interection. r is thus fraction of full to intersection                                  
  real s = v1.cross(num)/denom;
  
  // Check if line segments intersect
  return !(r > 1.0f || r < 0.0f || s > 1.0f || s < 0.0f);
}

/*!
  True if segment intersects rectangle \a r. False if segment is either inside
  or outside the rectangle. Intersection is defined on the closed rectangle and segment
  so touch each others endpoints or boundaries counts as intersection.
*/
bool Segment2::intersect(const Rect2& r) const
{
//  Vector2 max = r.max();
//  Vector2 min = r.min();
//  
//  // Source to the right?
//  if (source().x() > max.x()) {
//    if (target.x() > max.x())
//      return false;
//      
//    // Source above rect?
//    if (source.y() > max.y()) {
//      if (target.y() > max.y())
//        return false;
//      if (target.y() > min.y()) {
//        if (target.x() > min.x())
//          return true;
//        
//      }  
//      if (target.y() < min.y()) {
//        
//      }        
//    }
//    // Source below rectÀ
//    else if (source.y() < min.y()) {
//      if (target.y() < min.y())
//        return false;
//      if (taget.y() > max.y())
//        
//    }
//  }
//  // Source to the left?  
//  else if (source().x() < min.x()) {
//    if (target.x() < min.x())
//      return false;
//
//  }
  
  
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

// Calculations
Segment2 Segment2::opposite() const
{
  return Segment2(iV[1], iV[0]);
}

Vector2 Segment2::toVector() const
{
  return iV[1]-iV[0];
}

/*! Slope of segment relative to the left most point */
real Segment2::slope() const
{
  Vector2 v = right()-left();
  if (v.x() == 0)
    return REAL_MAX;
  else
    return v.y()/v.x();
}

real Segment2::distance(const Point2& p) const
{
  return (nearestPoint(p) - p).length();
}

/*!
  Finds the point on segment that is nearest to point \a p. This point
  can be either on the segment, or at one of the end points.
*/
Point2 Segment2::nearestPoint(const Point2& p) const
{   
  const Vector2 v = toVector();
  real dot_s = (p-source()).dot(v);

  // See if 'source' is the nearest point, meaing angle is obtuse ( > 90 )
  if (dot_s <= 0.0)
    return source();
  
  // See if 'target' is the nearest point  
  real dot_t = (p-target()).dot(-v);  
  if (dot_t <= 0.0)
    return target();
  
  return source() + v*(dot_s/(dot_s+dot_t));  
}

// Line intersection algo from: http://www.cgafaq.info/wiki/Intersection_Of_2D_Line_Segments
/*!
  Returns true if this segment intersect \a seg. \a result will then contain the intersection point.
  Otherwise \a result is undefined.
  
  Line segments are defined at endpoints so 
  linesegments e.g. sharing endpoints are said to intersect, unless they are parallell.
  Parallell line segments are defined as not intersecting even if they overlap.  
  
  Itersection test is not fully accurate. It might report full intersection even when line segments
  are parallell because it does not use exact numbers.   
*/
bool Segment2::intersection(const Segment2& seg, Vector2& result) const
{
  Vector2 v1 = toVector();
  Vector2 v2 = seg.toVector();
  
  Vector2 num = source() - seg.source();
  real denom = v1.cross(v2);  // distance from end of s1 to line with same 
                              // direction as s2 placed as start of s1
  
  // Check if line segments are parallel
  if (denom == 0.0f)
    return false;
  
  real r = v2.cross(num)/denom;   // denom is whole length, while v2.cross(num) is length 
                                  // from s1 start to interection. r is thus fraction of full to intersection                                  
  real s = v1.cross(num)/denom;
  
  // Check if line segments intersect
  if (r > 1.0f || r < 0.0f || s > 1.0f || s < 0.0f) 
    return false;
    
  // Calculate intersection point
  result = source()+v1*r;        // Could also find this with s, starting from s2 and with v2
  
  return true;
}
            
// Operators
bool Segment2::operator==(const Segment2& q) const
{
  return source() == q.source() && target() == q.target();
}

bool Segment2::operator!=(const Segment2& q) const
{
  return source() != q.source() || target() != q.target();  
}
