/*
 *  CollisionObject.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/CollisionObject.h"
#include <iostream>

#include <assert.h>

using namespace std;

/*!
   \class CollisionObject CollisionObject.h
   \brief A node in the scene graph. 

   The scene graph is used for rendering. The node is also used for collision
   detection.

*/
 
// Helper functions

// Constructors
CollisionObject::CollisionObject()
{
  
}

CollisionObject::~CollisionObject()
{
  // cout << hex << "0x" << (int)this << " CollisionObject removed" << endl;  // DEBUG    
}

// Accessors

// Request
/*! Returns true if this object is a simple non recursive collision object */
bool CollisionObject::isSimple() const
{
  return true;
}

// Calculations
/*! 
  True if object intersects circle. For CollisionObject this
  always returns False and produce a warning. Because the method
  is not meant to be called on a collision object. But only
  on subclasses which define a simple shape. Hierarcical structures
  should return false as well as they should not handle this intersection test.
*/
bool CollisionObject::intersect(const Circle& c) const
{
  assert(false);
  cerr << "Error: intersect(const Circle& c) not supported for this class" << endl;
  return false;
}

/*! 
  True if object intersects Rect2. For CollisionObject this
  always returns False and produce a warning. Because the method
  is not meant to be called on a collision object. But only
  on subclasses which define a simple shape. Hierarcical structures
  should return false as well as they should not handle this intersection test.
*/

bool CollisionObject::intersect(const Rect2& r) const
{
  assert(false);  
  cerr << "Error: intersect(const Rect2& r) not supported for this class" << endl;  
  return false;  
}

/*! 
  True if object intersects Segment2. For CollisionObject this
  always returns False and produce a warning. Because the method
  is not meant to be called on a collision object. But only
  on subclasses which define a simple shape. Hierarcical structures
  should return false as well as they should not handle this intersection test.
*/
bool CollisionObject::intersect(const Segment2& s) const
{
  assert(false);  
  cerr << "Error: intersect(const Segment2& s) not supported for this class" << endl;    
  return false;  
}

bool CollisionObject::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
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
void CollisionObject::draw(const Rect2& r) const
{
  
}

// Operations
/*!
  Update position orientation, speed etc of object based on
  time passed. 
  
  Default implementation doesn't do anythning since some objects might
  be static and not moveable.
*/
void CollisionObject::update(real start_time, real delta_time)
{
  // Do nothing
}