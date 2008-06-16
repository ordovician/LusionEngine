/*
 *  CircleShape.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include "Base/Shape.h"

#include <Geometry/Circle.hpp>
#include <Geometry/Polygon2.hpp>

class CollisionAction;

class CircleShape : public Shape
{
public:
  // Constructors
  CircleShape(const Circle& circle);
  virtual ~CircleShape();

  // Accessors
  std::string typeName() const;
  Rect2 boundingBox() const;  
  const Point2& center() const;
  real          radius() const;
    
  // Request
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);  
  bool inside(const Point2& p, real t, real dt, Action* command = 0);  
  
  // Calculations
  bool intersection(const Circle& c, Points2& points) const;
  bool intersection(const Rect2& r, Points2& points) const;
  bool intersection(const Segment2& s, Points2& points) const;
  bool intersection(const Polygon2& poly, Points2& points) const;
  
  void draw(const Rect2& r) const;
      
private:
  Circle iCircle;
};