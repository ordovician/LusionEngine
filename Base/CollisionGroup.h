/*
 *  CollisionGroup.h
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


class CollisionGroup : public CollisionObject
{
public:
  // Constructors
  CollisionGroup();
  CollisionGroup(CollisionObjectIterator begin, CollisionObjectIterator end);
  CollisionGroup(CollisionObject *left, CollisionObject *right);  
  CollisionGroup(CollisionObject *left, CollisionObject *right, const Rect2& box);
  
  virtual ~CollisionGroup();

  // Accessors
  Rect2 boundingBox() const;
  
  // Request
  bool  intersect(const Circle& c) const;
  bool  intersect(const Rect2& r) const;
  bool  intersect(const Segment2& s) const;
  
  // Calculations
  bool traverse(real t, real dt, SpriteCommand* command);
        
  // Operations
  void update(real start_time, real delta_time);  
  
  void init();
  void init(CollisionObjectIterator begin, CollisionObjectIterator end);
  void init(CollisionObject *left, CollisionObject *right);
  void init(CollisionObject *left, CollisionObject *right, const Rect2& box);
  CollisionObject* buildBranch(CollisionObjectIterator begin, CollisionObjectIterator end, int axis);
  
private:
  CollisionObject *iLeft, *iRight;
  Rect2 iBox;                 // Bounding box
};