/*
 *  Shape.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Shape.h"
#include <iostream>

#include <assert.h>

using namespace std;

/*!
   \class Shape Shape.h
   \brief A node in the scene graph. 

   The scene graph is used for rendering. The node is also used for collision
   detection.

*/
 
// Helper functions

// Constructors
Shape::Shape()
{
  
}

Shape::~Shape()
{
  // cout << hex << "0x" << (int)this << " Shape removed" << endl;  // DEBUG    
}

// Accessors
int Shape::noShapes() const
{
  return 0;
} 
 
ShapeIterator* Shape::shapeIterator() const
{
  static NullIterator<Shape*> nullIterator;
  return &nullIterator;
}

// Request
/*! Returns true if this object is a simple non recursive collision object */
bool Shape::isSimple() const
{
  return true;
}

// Calculations
/*! 
  True if object intersects circle. For Shape this
  always returns False and produce a warning. Because the method
  is not meant to be called on a collision object. But only
  on subclasses which define a simple shape. Hierarcical structures
  should return false as well as they should not handle this intersection test.
*/
bool Shape::intersect(const Circle& c) const
{
  assert(false);
  cerr << "Error: intersect(const Circle& c) not supported for this class" << endl;
  return false;
}

/*! 
  True if object intersects Rect2. For Shape this
  always returns False and produce a warning. Because the method
  is not meant to be called on a collision object. But only
  on subclasses which define a simple shape. Hierarcical structures
  should return false as well as they should not handle this intersection test.
*/

bool Shape::intersect(const Rect2& r) const
{
  assert(false);  
  cerr << "Error: intersect(const Rect2& r) not supported for this class" << endl;  
  return false;  
}

/*! 
  True if object intersects Segment2. For Shape this
  always returns False and produce a warning. Because the method
  is not meant to be called on a collision object. But only
  on subclasses which define a simple shape. Hierarcical structures
  should return false as well as they should not handle this intersection test.
*/
bool Shape::intersect(const Segment2& s) const
{
  assert(false);  
  cerr << "Error: intersect(const Segment2& s) not supported for this class" << endl;    
  return false;  
}

bool Shape::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  assert(false);  
  cerr << "Error: intersect(ConstPointIterator2 begin, ConstPointIterator2 end) not supported for this class" << endl;    
  return false;    
}

/*!
  Draw collision object inside rectangle \a r. It means that if object happens
  to be outside \a r then code does not need to draw object. This is a performance measure.
  Default implementation is to do nothing (draw nothing).
*/
void Shape::draw(const Rect2& r) const
{
  
}

// Operations
void Shape::addKid(Shape* shape)
{
  // Do nothing
}

void Shape::removeKid(Shape* shape)
{
  // Do nothing  
}

/*!
  Update position orientation, speed etc of object based on
  time passed. 
  
  Default implementation doesn't do anythning since some objects might
  be static and not moveable.
*/
void Shape::update(real start_time, real delta_time)
{
  // Do nothing
}