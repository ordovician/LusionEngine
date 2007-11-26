/*
 *  SegmentShape2.h
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

class CollisionAction;

class SegmentShape2 : public Shape
{
public:
  // Constructors
  SegmentShape2(const Segment2& seg);
  virtual ~SegmentShape2();

  // Accessors
  Rect2 boundingBox() const;  
    
  // Request
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);  
  bool inside(const Point2& p, real t, real dt, Action* command = 0);
  
  // Calculations
  bool intersect(const Circle& c) const;
  bool intersect(const Rect2& r) const;
  bool intersect(const Segment2& s) const;
  bool intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const;
  
  void draw(const Rect2& r) const;
      
private:
  Segment2 iSeg;
};