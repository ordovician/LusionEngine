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
#include <Base/Shape.h>
#include <Base/ShapeListener.h>

// Group used for rendering
Group* renderGroup(); 

class Group : public Shape, public ShapeListener
{
public:
  // Constructors
  Group();
  virtual ~Group();

  // Accessors
  std::string typeName() const;
  Rect2 boundingBox() const;  
  int   noShapes() const;  
  ShapeIterator* iterator() const;
  
  // Request
  bool contains(Shape* shape) const;
  bool isSimple() const;
  
  // Calculations
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);
  bool inside(const Point2& p, real t, real dt, Action* command);
  void draw(const Rect2& r) const;
        
  // Operations
  void addKid(Shape* shape);
  void removeKid(Shape* shape);
  void update(real start_time, real delta_time);
  void doPlanning(real start_time, real delta_time);
  void clear();

  Shape* nextShape();
  
  // Even handling
  void shapeDestroyed(Shape* shape);
  void shapeKilled(Shape* shape);
        
private:
  std::set<Shape*> iShapes;
  std::set<Shape*>::iterator iCurShape;
  
  Rect2 iBBox;
};