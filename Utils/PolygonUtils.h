/*
 *  PolygonUtils.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 13.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include <cmath>
#include <Geometry/Circle.hpp>

using namespace std;

/*! Finds the normalized direction between two points dst and src*/
struct CalcDirection : public binary_function<Point2, Point2, Vector2>
{
  Vector2 operator()(const Point2& dst, const Point2 src)
  {
    return (dst-src).unit();
  }
};

struct ProjectPoint : public unary_function<Point2, real>
{
  ProjectPoint(const Vector2& v1);  
  real operator()(const Point2& v1);
  
  Vector2 v;
};

/*! Returns true if v is able to separate p1 and p2 */
struct ProjectPolygon : public unary_function<Vector2, bool>
{
  ProjectPolygon(
    Polygon2::const_iterator apb, 
    Polygon2::const_iterator ape, 
    Polygon2::const_iterator aqb, 
    Polygon2::const_iterator aqe
  ) 
    : pb(apb), pe(ape), qb(aqb), qe(aqe), proj1(ape-apb), proj2(aqe-aqb)  { }
 
  bool operator()(const Vector2& v);

 Polygon2::const_iterator pb, pe, qb, qe;
 vector<real> proj1, proj2;
};

struct Longest : public unary_function<Vector2, void>
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
  
bool intersect(const Polygon2& p1, const Polygon2& p2);

bool intersect(const Circle& circle, ConstPointIterator2 begin, ConstPointIterator2 end);
bool intersect(const Segment2& seg, ConstPointIterator2 begin, ConstPointIterator2 end);
bool intersect(const Rect2& rect, ConstPointIterator2 begin, ConstPointIterator2 end);

bool inside(ConstPointIterator2 pb, ConstPointIterator2 pe, const Point2& point);
Rect2 boundingBox(ConstPointIterator2 pb, ConstPointIterator2 pe);