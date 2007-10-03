/******************************************************************
Name	: Segment2
Author	: Erik Engheim
Date	: 07/09/2005
Desc	: Describes a 2d line segment. 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>

#include <Geometry/Vector2.hpp>
#include <Geometry/Rect2.hpp>

#include <vector>

class Segment2
{
public:
    // Constructors
    Segment2();    
    Segment2(const Point2& p, const Point2& q, void *data=0);
    
    // Accessors
    Point2 source() const;
    Point2 target() const;    
    Point2 vertex(int i) const;
    Point2 side(int i) const;
    Point2 left() const;
    Point2 right() const;    
        
    real xmin() const;
    real ymin() const;
    real xmax() const;
    real ymax() const;
    
    int   tag() const;
    void  setTag(int tag);
    
    void  setData(void *data);
    void* data() const;
    
    // Request
    bool isDegenerate() const;
    bool isAbove(const Point2& p) const;
    bool isBelow(const Point2& p) const;    
    bool isOn(const Point2& p) const;    
    bool isOnEnd(const Point2& p) const;    
    bool intersect(const Segment2& seg) const;
    bool intersect(const Rect2& rect) const;
    
    // Calculations
    Segment2 opposite() const;
    Vector2 toVector() const;
    real    slope() const;
    real    distance(const Point2& p) const;
    Point2  nearestPoint(const Point2& p) const;
    bool    intersection(const Segment2& seg, Vector2& result) const;
                
    // Operators
    bool operator==(const Segment2& q) const;
    bool operator!=(const Segment2& q) const;
    
private:
  Vector2 iV[2];
  void *iData;
};

// bool intersect(const Segment2& s1, const Vector2& l2, Vector2& result);


typedef std::vector<Segment2> Segments2;