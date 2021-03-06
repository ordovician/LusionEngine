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

#include "Utils/PolygonUtils.h"
#include <Geometry/IO.hpp>
#include <Geometry/Ray2.hpp>

#include <numeric>
#include <iostream>

using namespace std;

ProjectPoint::ProjectPoint(const Vector2& v1) : v(v1) {
}

real ProjectPoint::operator()(const Point2& v1) {
  return v.dot(v1);
}

/*! 
  Projects the two polygons stored in functor onto vector v.
  Returns true if the projected polygons overlap.
*/
bool ProjectPolygon::operator()(const Vector2& v)
{
 // Project all points on poly1 and poly2 onto line v
 transform(pb, pe, proj1.begin(), ProjectPoint(v));
 transform(qb, qe, proj2.begin(), ProjectPoint(v));   

 real min1 = *min_element(proj1.begin(), proj1.end());
 real max1 = *max_element(proj1.begin(), proj1.end());  
 real min2 = *min_element(proj2.begin(), proj2.end());
 real max2 = *max_element(proj2.begin(), proj2.end());  
  
 return min1 > max2 || max1 < min2;
}

/*!
  Check if there is a collision between two polygons. First polygon is defined
  by point sequence 'pb' to 'pe'. Second polygon by 'qb' to 'qe'. 
*/
bool intersect(
  ConstPointIterator2 pb, // Start of first polygon
  ConstPointIterator2 pe, 
  ConstPointIterator2 qb, // Start of second polygon
  ConstPointIterator2 qe)
{
  Points2 d1(pe-pb), d2(qe-qb);  // holds directions
  
  // Find direction of each edge in polygons
  CalcDirection calcDir;  
  adjacent_difference(pb, pe, d1.begin(), calcDir);
  adjacent_difference(qb, qe, d2.begin(), calcDir);
  d1[0] = calcDir(d1[0], *(pe-1));
  d2[0] = calcDir(d2[0], *(qe-1));      
   
  // We have collision if it was not possible to find any separating axis
  // if ProjectPolygon is true for any of the directions then that direction
  // will be returned. This will make the equation false and signal no collision.
  return 
    find_if(d1.begin(), d1.end(), ProjectPolygon(pb, pe, qb, qe)) == d1.end() &&
    find_if(d2.begin(), d2.end(), ProjectPolygon(pb, pe, qb, qe)) == d2.end();   
}

bool intersect(const Points2& p1, const Points2& p2)
{
  return intersect(p1.begin(), p1.end(), p2.begin(), p2.end());
}


bool intersect(const Segment2& s, ConstPointIterator2 begin, ConstPointIterator2 end)
{
  ConstPointIterator2 it = begin, prev = begin;
  for (++it; it != end; ++it, ++prev) {
    if (s.intersect(Segment2(*prev, *it)))
      return true;    
  }
  return s.intersect(Segment2(*prev, *begin));    
}

bool intersect(const Rect2& rect, ConstPointIterator2 begin, ConstPointIterator2 end)
{
  Segment2 bottom(rect.bottomLeft(), rect.bottomRight());
  Segment2 top(rect.topLeft(), rect.topRight());
  Segment2 left(rect.bottomLeft(), rect.topLeft());
  Segment2 right(rect.bottomRight(), rect.topRight());
    
  ConstPointIterator2 it;
  for (it = begin+1; it != end; ++it) {
    Segment2 seg(*it, *(it-1));
    if (seg.intersect(bottom)) return true;
    if (seg.intersect(top)) return true;    
    if (seg.intersect(left)) return true;        
    if (seg.intersect(right)) return true;        
  }
  return false;  
}

/**
 * Only works for convex shapes which are counter clockwise
 */
static bool fastInside(ConstPointIterator2 pb, ConstPointIterator2 pe, const Point2& q)
{
  Points2::const_iterator p;
  for (p = pb+1; p != pe; ++p) {
    if ((*p - *(p-1)).cross(q - *(p-1)) <= 0.0)
      return false;
  }
  if ((*pb - *(p-1)).cross(q - *(p-1)) <= 0.0)
    return false;

  return true;
}

/**
 * Only works for convex shapes but we can easily improve it to work for others
 */
/*bool inside(ConstPointIterator2 pb, ConstPointIterator2 pe, const Point2& q)
{
  Ray2 r(q, Vector2(-1.0, 0));
  return r.noIntersections(pb, pe) == 1;  
}*/

/*!
  Calculates a minimum bounding box for all supplied points
*/
Rect2 boundingBox(ConstPointIterator2 pb, ConstPointIterator2 pe)
{
  real min = -numeric_limits<real>::max();
  real max = numeric_limits<real>::max();
    
  Rect2 r(Vector2(max, max), Vector2(min, min));
  Points2::const_iterator p;
  for (p = pb; p != pe; ++p) {
    r = r.surround(*p);
  }
  return r;
}

// Debug
void dumpPoints(ConstPointIterator2 begin, ConstPointIterator2 end) {
  cout << "( ";
  copy(begin, end, ostream_iterator<Vector2>(cout, ", "));  
  cout << " )" << endl;  
}
