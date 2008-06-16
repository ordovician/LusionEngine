/******************************************************************
Name	: Circle
Author	: Erik Engheim
Date	: 07/09/2005
Desc	: Describes a 2d line segment. 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>

#include <Geometry/Vector2.hpp>

#include <vector>

// Forward declarations
class Rect2;
class Segment2;
class Ray2;
class Polygon2;

class Circle
{     
public:
    // Constructors
    Circle();    
    Circle(const Point2& center, real radius);
    
    // Accessors
    const Point2& center() const;
    real   radius() const;    
    void   setCenter(const Point2& pos);
    void   setRadius(real radius);
            
    // Request    
    bool inside(const Point2& p) const;
    bool intersect(const Circle& c) const;
    bool intersect(const Rect2& rect) const;
    bool intersect(const Segment2& seg) const;
    bool intersection(const Segment2& seg, Points2& points) const;        
    bool intersect(const Ray2& ray) const;
    bool intersect(const Polygon2& poly) const;
    bool intersection(const Polygon2& poly, Points2& points, bool first = false) const;        
    
    // Calculations
                
    // Operators
    bool operator==(const Circle& circle) const;
    bool operator!=(const Circle& circle) const;
    
private:
  Point2 iCenter;
  real   iRadius;
};