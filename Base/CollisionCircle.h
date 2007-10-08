/*
 *  CollisionCircle.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include "Base/CollisionObject.h"

#include <Geometry/Circle.hpp>

class SpriteCommand;

class CollisionCircle : public CollisionObject
{
public:
  // Constructors
  CollisionCircle();
  virtual ~CollisionCircle();

  // Accessors
  virtual Rect2 boundingBox() const = 0;  
    
  // Request
  bool isSimple() const;
  bool collide(CollisionObject* other, real t, real dt, SpriteCommand* command = 0);  
  
  // Calculations
  bool intersect(const Circle& c) const;
  bool intersect(const Rect2& r) const;
  bool intersect(const Segment2& s) const;
  bool intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const;
    
  // Operations
  virtual void update(real start_time, real delta_time) = 0;  
};