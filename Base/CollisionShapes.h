/*
 *  Shape.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"
#include <Geometry/Shape.hpp>

class CircleShape : public Shape
{
public:
  // Constructors
  CircleShape(const Circle& circle);

  // Accessors
  Rect2 boundingBox() const;  
      
  // Calculations
  bool intersect(Shape* obj) const;
  
private:
  Circle iCircle;
};

