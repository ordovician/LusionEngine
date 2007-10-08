/*
 *  CollisionRect2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include "Base/CollisionObject.h"

#include <Geometry/Rect2.hpp>

class SpriteCommand;

class CollisionRect2 : public CollisionObject
{
public:
  // Constructors
  CollisionRect2(const Rect2& rect);
  virtual ~CollisionRect2();

  // Accessors
  Rect2 boundingBox() const;  
    
  // Request
  bool collide(CollisionObject* other, real t, real dt, SpriteCommand* command = 0);  
  
  // Calculations
  bool intersect(const Circle& c) const;
  bool intersect(const Rect2& r) const;
  bool intersect(const Segment2& s) const;
  bool intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const;
  
  void draw(const Rect2& r) const;
      
private:
  Rect2 iRect;
};