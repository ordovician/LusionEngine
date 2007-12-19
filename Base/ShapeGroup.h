/*
 *  ShapeGroup.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include <Base/Shape.h>

class ShapeGroup : public Shape
{
public:
  // Constructors
  ShapeGroup();
  ShapeGroup(ShapeIterator* iterator);
  ShapeGroup(vector<Shape*>::iterator begin, vector<Shape*>::iterator end);
  ShapeGroup(Shape *left, Shape *right);  
  ShapeGroup(Shape *left, Shape *right, const Rect2& box);
  
  virtual ~ShapeGroup();

  // Accessors
  std::string typeName() const;
  Rect2 boundingBox() const;
  int   noShapes() const;  
  ShapeIterator* shapeIterator() const;
    
  // Request
  bool isSimple() const;
  
  // Calculations
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);  
  bool inside(const Point2& p, real t, real dt, Action* command = 0);
  void draw(const Rect2& r) const;
  
  // Operations
  void update(real start_time, real delta_time);  
  
  void init();
  void init(vector<Shape*>::iterator begin, vector<Shape*>::iterator end);
  void init(Shape *left, Shape *right);
  void init(Shape *left, Shape *right, const Rect2& box);
  Shape* buildBranch(vector<Shape*>::iterator begin, vector<Shape*>::iterator end, int axis);
  
private:
  Shape *iLeft, *iRight;
  Rect2 iBox;                 // Bounding box
};