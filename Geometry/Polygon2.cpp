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
Polygon2::Polygon2()
{

}

Polygon2::Polygon2(const Rect2& r)
{
  real h = r.height();
  real w = r.width();

  iPoints.resize(4);
  iPoints[0] = r.min();
  iPoints[1] = r.min() + Vector2(w,0.0f);
  iPoints[2] = r.max();
  iPoints[3] = r.min() + Vector2(0.0f,h);  
}

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

const Vector2& Polygon2::front() const
{
  return iPoints.front();
}



// Request    
bool Polygon2::intersect(const Polygon2& polygon) const
{
  return ::intersect(begin(), end(), polygon.begin(), polygon.end());
}
/*
bool Polygon2::intersection(const Polygon2& polygon, Points2& points) const
{
  return ::intersection(begin(), end(), polygon.begin(), polygon.end(), points);
}
*/
bool Polygon2::intersect(const Circle& circle) const
{
  return circle.intersect(*this);
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
  
  \note Only works for convex shapes but we can easily improve it to work for others  
*/
bool Polygon2::inside(const Point2& q) const
{
  Ray2 r(q, Vector2(-1.0, 0));
  return r.noIntersections(*this) == 1;
}

// Calculations
/*!
  Calculates bounding box for polygon
*/
Rect2 Polygon2::boundingBox() const
{
  return ::boundingBox(begin(), end());
}

/*!
  Used by minkowski sum to calculate angle between vector pq and positive x axis
*/
static real angle(const Point2& p, const Point2& q) 
{
  // if not p or not q then print(debug.traceback("angle")); debug.debug() end  
  Vector2 v = q - p;
  real  ang = atan2(v.y(), v.x());
  return ang < 0 ? 2.0 * M_PI + ang : ang;
}


/*!
  minkowski sum algorithm from page 279 in Computational Geometry
  
  \note there is a bug in the book, which is described in the code.
*/
void Polygon2::minkowskiSum(const Polygon2& other, Polygon2& result) const
{
  Points2 v(size() + 2);
  Points2 w(other.size() + 2);
  copy(begin(), end(), v.begin());
  copy(other.begin(), other.end(), w.begin());
      
  int i = 0;
  int j = 0;
  int n = size() - 1;
  int m = other.size() - 1;
  v[n+1] = v[1];
  w[m+1] = w[1];
    
  v[n+2] = v[2]; // Book bug: Must cover second since when j=m+1 or i=n+1, 
  w[m+2] = w[2]; // the other one might not have been reached.
                 // j+1 and i+1 will be accesses which might
                 // potentially be m+2 or n+2.
    
  real ang   = -1.0;
  real ang_v = -1.0;
  real ang_w = -1.0;
  
  do {
    result.push_back(v[i]+w[j]);

    // We need to do these two following checks to make sure
    // that angle always increases in value as we trail around polygon.
    ang = angle(v[i], v[i+1]);
    if (ang < ang_v)
      ang = 2.0 * M_PI + ang;
    ang_v = ang;
    
    ang = angle(w[j], w[j+1]);
    if (ang < ang_w)
      ang = 2.0 * M_PI + ang;
    ang_w = ang;
        
    if  (ang_v < ang_w)
      i = i+1;
    else if (ang_v > ang_w)
      j = j+1;
    else {
      i = i+1;
      j = j+1;     
    }
  }
  while (i != n + 1 || j != m + 1);
}

// Operations
void Polygon2::push_back(const Vector2& p)
{
  iPoints.push_back(p);
}

void Polygon2::resize(int size) 
{
  iPoints.resize(size);
}

void Polygon2::clear() 
{
  iPoints.clear();
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

Point2& Polygon2::operator[](int i)
{
  return iPoints[i];
}

const Point2& Polygon2::operator[](int i) const
{
  return iPoints[i];
}