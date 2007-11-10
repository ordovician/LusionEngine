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
#include <Base/Shape.h>


// Group used for rendering
Group* renderGroup(); 

class Group : public Shape
{
public:
  // Constructors
  Group();
  virtual ~Group();

  // Accessors
  Rect2 boundingBox() const;  
  int   noShapes() const;  
  ShapeIterator* shapeIterator() const;
  int   size() const;
  
  // Request
  bool contains(Shape* shape) const;
  bool isSimple() const;
  
  // Calculations
  bool collide(Shape* other, real t, real dt, SpriteCommand* command = 0);
  bool inside(const Point2& p, real t, real dt, SpriteCommand* command);
  void draw(const Rect2& r) const;
        
  // Operations
  void add(Shape* shape);
  void remove(Shape* shape);
  void update(real start_time, real delta_time);
  void doPlanning(real start_time, real delta_time);
  void clear();

  Shape* nextShape();
      
private:
  std::set<Shape*> iShapes;
  std::set<Shape*>::iterator iCurShape;
  
  Rect2 iBBox;
};