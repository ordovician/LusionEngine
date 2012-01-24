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

#include <cmath>
#include <Geometry/Circle.hpp>

/*! 
  Finds the normalized direction between two points dst and src, and
  then finds the vector perpendicular to this. This is a potential candiate
  for separating axis in the case \a dst and \a src are points on a polygon
*/
struct CalcDirection : public std::binary_function<Point2, Point2, Vector2>
{
  Vector2 operator()(const Point2& dst, const Point2 src)
  {
    return (dst-src).unit().normal();
  }
};

struct ProjectPoint : public std::unary_function<Point2, real>
{
  ProjectPoint(const Vector2& v1);  
  real operator()(const Point2& v1);
  
  Vector2 v;
};

/*! Returns true if v is able to separate p1 and p2 */
struct ProjectPolygon : public std::unary_function<Vector2, bool>
{
  ProjectPolygon(
    Points2::const_iterator apb, 
    Points2::const_iterator ape, 
    Points2::const_iterator aqb, 
    Points2::const_iterator aqe
  ) 
    : pb(apb), pe(ape), qb(aqb), qe(aqe), proj1(ape-apb), proj2(aqe-aqb)  { }
 
  bool operator()(const Vector2& v);

 Points2::const_iterator pb, pe, qb, qe;
 std::vector<real> proj1, proj2;
};

struct Longest : public std::unary_function<Vector2, void>
{
  Longest(const Vector2& v = Vector2()) 
  {
    length = v.length();
  }
  
  void operator()(Vector2 v) {
    real l = v.length();
    if (l > length)
      length = l;
  }
  
  real length;
};

inline real rad(real degrees) { return degrees*M_PI/180.0; }
inline real deg(real rad) { return rad*180.0/M_PI; }

bool intersect(
  ConstPointIterator2 pb, 
  ConstPointIterator2 pe, 
  ConstPointIterator2 qb, 
  ConstPointIterator2 qe);
  
bool intersect(const Points2& p1, const Points2& p2);
bool intersect(const Segment2& seg, ConstPointIterator2 begin, ConstPointIterator2 end);
bool intersect(const Rect2& rect, ConstPointIterator2 begin, ConstPointIterator2 end);

//bool inside(ConstPointIterator2 pb, ConstPointIterator2 pe, const Point2& point);
Rect2 boundingBox(ConstPointIterator2 pb, ConstPointIterator2 pe);

// Debug
void dumpPoints(ConstPointIterator2 begin, ConstPointIterator2 end);
