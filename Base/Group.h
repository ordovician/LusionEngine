/*
 *  Group.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"
#include "Base/Sprite.h"
#include "Base/MotionState.h"
#include <Core/SharedObject.hpp>
#include <Base/CollisionObject.h>


// Group used for rendering
Group* renderGroup(); 

class Group : public CollisionObject
{
public:
  // Constructors
  Group();
  virtual ~Group();

  // Accessors
  int size() const;
  ShallowSpriteSet sprites() const;
  MutableShallowSpriteSet sprites();
  Rect2 boundingBox() const;  

    
  // Request
  bool contains(Sprite* sprite) const;
  bool isSimple() const;
  
  // Calculations
  bool collide(CollisionObject* other, real t, real dt, SpriteCommand* command = 0);
  bool inside(const Point2& p, real t, real dt, SpriteCommand* command);
  void draw(const Rect2& r) const;
        
  // Operations
  void add(Sprite* sprite);
  void remove(Sprite* sprite);
  void update(real start_time, real delta_time);
  void doPlanning(real start_time, real delta_time);
  void clear();
    
public: 
  Sprite* nextSprite();  // NOTE: consider making private
  
private:
  SpriteSet iSprites;
  SpriteSet::iterator iCurSprite;
  
  Rect2 iBBox;
};