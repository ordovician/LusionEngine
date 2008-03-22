/*
 *  RoadMap.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 6.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include <Geometry/TrapezoidalMap2.hpp>
#include <Base/Sprite.hpp>

class RoadMap
{
public:
  // Constructors
  RoadMap( SpriteList::const_iterator begin, SpriteList::const_iterator end );
  virtual ~RoadMap();

  // Accessors

  // Request

  // Operations

  // Calculations
  Polygon2::iterator findPath(
    const Point2& start, 
    const Point2& target, 
    int start_ticks
    Polygon2::iterator output) const;
  
private:
  TrapezoidalMap2 iMap;
};