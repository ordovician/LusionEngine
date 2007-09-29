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
#include <Geometry/Rect2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Segment2.hpp>

#include <Core/SharedObject.hpp>

class SpriteCommand;

class CollisionObject : public SharedObject
{
public:
  // Constructors
  CollisionObject();
  virtual ~CollisionObject();

  // Accessors
  virtual Rect2 boundingBox() const = 0;  
    
  // Calculations
  virtual bool intersect(const Circle& c) const = 0;
  virtual bool intersect(const Rect2& r) const = 0;
  virtual bool intersect(const Segment2& s) const = 0;
      
  virtual bool traverse(real t, real dt, SpriteCommand* command) = 0;
    
  // Operations
  virtual void update(real start_time, real delta_time) = 0;  
};

typedef vector<CollisionObject*> CollisionObjects;
typedef CollisionObjects::iterator CollisionObjectIterator;
typedef CollisionObjects::const_iterator ConstCollisionObjectIterator;