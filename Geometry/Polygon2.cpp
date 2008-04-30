/*
 *  Polygon2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 30.4.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <Geometry/Polygon2.hpp>
#include <Utils/PolygonUtils.h>

#include <algorithm>

using namespace std;

// Constructors
Polygon2::~Polygon2()
{

}

// Accessors
ConstPointIterator2 Polygon2::begin() const 
{
  return iPoints.begin();
}

ConstPointIterator2 Polygon2::end() const
{
  return iPoints.end();
}

PointIterator2 Polygon2::begin()
{
  return iPoints.begin();
}

PointIterator2 Polygon2::end()
{
  return iPoints.end();
}

int Polygon2::size() const
{
  return iPoints.size();
}

// Request    
bool Polygon2::intersect(const Polygon2& polygon) const
{
  return ::intersect(begin(), end(), polygon.begin(), polygon.end());
}

bool Polygon2::intersect(const Circle& circle) const
{
  return circle.intersect(begin(), end());
}

bool Polygon2::intersect(const Segment2& s) const
{
  return ::intersect(s, begin(), end());  
}

bool Polygon2::intersect(const Rect2& rect) const
{
  return ::intersect(rect, begin(), end());
}

/*!
  True, if point \a q is inside polygon
*/
bool Polygon2::inside(const Point2& q) const
{
  return ::inside(begin(), end(), q);
}

// Calculations
/*!
  Calculates bounding box for polygon
*/
Rect2 Polygon2::boundingBox() const
{
  return ::boundingBox(begin(), end());
}

// Operations
void Polygon2::push_back(const Vector2& p)
{
  iPoints.push_back(p);
}

// Operators
bool Polygon2::operator==(const Polygon2& polygon) const
{
  return polygon.size() == size() && equal(begin(), end(), polygon.begin());
}

bool Polygon2::operator!=(const Polygon2& polygon) const
{
  return polygon.size() != size() || !equal(begin(), end(), polygon.begin());
}