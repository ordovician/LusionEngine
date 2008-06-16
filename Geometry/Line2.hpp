/******************************************************************
Name	: Line2
Author: Erik Engheim
Date	: 07/09/2005
Desc	: Describes a 2D line. The line definition has an origin point, which
        is a point on the line and direction is the direction of the line.
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <vector>

#include "Types.h"

class Polygon2;

class Line2
{
public:
    // Constructors
    Line2();    
    Line2(const Point2& origin, const Vector2& dir);
    
    // Accessors
    Point2 origin() const;
    Vector2 direction() const;    

    // Calculations
    bool intersect(const Segment2& seg) const;
    bool intersection(const Segment2& seg, Vector2& result) const;
    bool intersection(const Polygon2& poly, Vector2& result) const;
    int  noIntersections(const Polygon2& poly) const;
    
    real    squaredDistance(const Point2& p) const;    
    real    distance(const Point2& p) const;
    Point2  nearestPoint(const Point2& p) const;
    
    bool intersect(const Rect2& rect) const;
    
    // Operators
    bool operator==(const Line2& q) const;
    bool operator!=(const Line2& q) const;
    
private:
  Vector2 iV[2];
};

