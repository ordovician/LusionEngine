/*
 *  CollisionObject.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"
#include <Geometry/CollisionObject.hpp>

class CollisionCircle : public CollisionObject
{
public:
  // Constructors
  CollisionCircle(const Circle& circle);

  // Accessors
  Rect2 boundingBox() const;  
      
  // Calculations
  bool intersect(CollisionObject* obj) const;
  
private:
  Circle iCircle;
};

