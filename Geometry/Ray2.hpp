/******************************************************************
Name	: Ray2
Author	: Erik Engheim
Date	: 07/09/2005
Desc	: Describes a 2D ray. A ray has a start point and extends
        infinitly in one direction. In some sense it is a like a line
        segment with only one end point.
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <vector>

#include "Types.h"

class Ray2
{
public:
    // Constructors
    Ray2();    
    Ray2(const Point2& origin, const Vector2& dir);
    
    // Accessors
    Point2 origin() const;
    Vector2 direction() const;    

    // Calculations
    bool intersect(const Segment2& seg) const;
    bool intersection(const Segment2& seg, Vector2& result) const;
    bool intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Vector2& result) const;
    int  noIntersections(ConstPointIterator2 begin, ConstPointIterator2 end) const;
    
    real    squaredDistance(const Point2& p) const;    
    real    distance(const Point2& p) const;
    Point2  nearestPoint(const Point2& p) const;
    
    bool intersect(const Rect2& rect) const;
    
    // Operators
    bool operator==(const Ray2& q) const;
    bool operator!=(const Ray2& q) const;
    
private:
  Vector2 iV[2];
};

