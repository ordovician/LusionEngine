/*
 *  Polygon2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 30.4.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <Core/Core.h>

#include <Geometry/Vector2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/Ray2.hpp>

class Polygon2
{
public:
  // Constructors
  template<typename ForwardIterator>
  Polygon2(ForwardIterator first, ForwardIterator last);
  Polygon2(const Rect2& r);
  Polygon2();
  ~Polygon2();
  
  // Accessors
  ConstPointIterator2 begin() const;
  ConstPointIterator2 end() const;
  PointIterator2      begin();
  PointIterator2      end();  
  int                 size() const;
  const Vector2&      front() const;
  
  // Request    
  bool intersect(const Polygon2& polygon) const;
  //bool intersection(const Polygon2& polygon, Points2& points) const;  
  bool intersect(const Circle& circle) const;
  bool intersect(const Segment2& seg) const;
  bool intersect(const Rect2& rect) const;

  bool inside(const Point2& q) const;
  
  // Calculations
  Rect2 boundingBox() const;
  void  minkowskiSum(const Polygon2& other, Polygon2& result) const;  
  
  // Operations
  void push_back(const Vector2& p);
  void resize(int size);
  void clear();
  
  // Operators
  bool operator==(const Polygon2& polygon) const;
  bool operator!=(const Polygon2& polygon) const;
  Point2& operator[](int i); 
  const Point2& operator[](int i) const; 
    
private:
  Points2 iPoints;
};

template<typename ForwardIterator>
Polygon2::Polygon2(ForwardIterator first, ForwardIterator last) : iPoints(first, last)
{
  
}
