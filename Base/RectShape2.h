/*
 *  RectShape2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include "Base/Shape.h"

#include <Geometry/Rect2.hpp>

class CollisionAction;

class RectShape2 : public Shape
{
public:
  // Constructors
  RectShape2(const Rect2& rect);
  virtual ~RectShape2();

  // Accessors
  std::string typeName() const;
  Rect2 boundingBox() const;  
    
  // Request
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);  
  bool inside(const Point2& p, real t, real dt, Action* command = 0);
  
  // Calculations
  bool intersection(const Circle& c, Points2& points) const;
  bool intersection(const Rect2& r, Points2& points) const;
  bool intersection(const Segment2& s, Points2& points) const;
  bool intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Points2& points) const;
  
  void draw(const Rect2& r) const;
      
private:
  Rect2 iRect;
};