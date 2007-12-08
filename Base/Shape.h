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
#include <Geometry/Rect2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Segment2.hpp>

#include <Core/SharedObject.hpp>

#include <Base/ShapeIterator.h>

class CollisionAction;
class Action;

class Shape : public SharedObject
{
public:
  // Constructors
  Shape();
  virtual ~Shape();

  // Accessors
  virtual Rect2 boundingBox() const = 0;  
  virtual int   noShapes() const;  
  virtual ShapeIterator* shapeIterator() const;
    
  // Request
  virtual bool isSimple() const;
  virtual bool collide(Shape* other, real t, real dt, CollisionAction* command = 0) = 0;  
  virtual bool inside(const Point2& p, real t, real dt, Action* command = 0) = 0;
    
  // Calculations
  virtual bool intersection(const Circle& c, Points2& points) const;
  virtual bool intersection(const Rect2& r, Points2& points) const;
  virtual bool intersection(const Segment2& s, Points2& points) const;
  virtual bool intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Points2& points) const;      
     
  virtual void draw(const Rect2& r) const;
   
  // Operations
  virtual void addKid(Shape* shape);
  virtual void removeKid(Shape* shape);

  virtual void update(real start_time, real delta_time);  
  virtual void handleCollision(Shape* other, Points2& points, real start_time, real delta_time);    
  virtual void doPlanning(real start_time, real delta_time);  
};