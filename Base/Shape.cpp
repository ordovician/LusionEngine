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

#include <cassert>

#include <algorithm>
#include <functional>

using namespace std;

/*!
   \class Shape Shape.h
   \brief A node in a 2D scene graph. 

   The scene graph is used for rendering. The node is also used for collision
   detection. Collision tests and intersection tests have interfaces that
   indicate that all the points in the intersection\collision is reported.
   
   However that is only the case for certain subclasses and combinations.
   Only Sprite and CircleShape collisions will produce intersection points.
   And it that case only one of the intersection points. Not all.
   
   However in the future it will be possible for all shapes.
*/

static int  gNextDepth = 0;
 
// Helper functions

// Constructors
Shape::Shape()
{
  // This way the last created shapes is always drawn  on top of the
  // first created ones
  iDepth = gNextDepth--;  
}

Shape::~Shape()
{
  // Notify all listeners that shape will no longer exist
  for_each(iListeners.begin(), iListeners.end(),
    bind2nd(
      mem_fun(&ShapeListener::shapeDestroyed),
      this));
  // cout << hex << "0x" << (long)this << " Shape removed" << endl;  // DEBUG
}

// Accessors
std::string
Shape::typeName() const  
{ 
  return "Shape"; 
}

int Shape::noShapes() const
{
  return 0;
} 
 
ShapeIterator* Shape::iterator() const
{
  static NullIterator<Shape*> nullIterator;
  return &nullIterator;
}

void Shape::setDepth(int aDepth)
{
	iDepth = aDepth;
}

int Shape::depth() const
{
	return iDepth;
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
bool Shape::intersection(const Circle& c, Points2& points) const
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

bool Shape::intersection(const Rect2& r, Points2& points) const
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
bool Shape::intersection(const Segment2& s, Points2& points) const
{
  assert(false);  
  cerr << "Error: intersect(const Segment2& s) not supported for this class" << endl;    
  return false;  
}

bool Shape::intersection(const Polygon2& poly, Points2& points) const
{
  assert(false);  
  cerr << "Error: intersect(const Polygon2& poly, points) not supported for this class" << endl;    
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


/*!  
  Should be overriden by subclasses which represent simple shapes. A simple shape
  is a shape that is not a composite (consiting of serval other shapes).
  
  This method will typically be called in collide method of subclasses to handle
  collision once it has been determined that a collision has occured. 
  
  If you implement your own collide method in a subclass, you should call 
  handleCollision on each involved shape to let the shape handle the collision. 
*/
void Shape::handleCollision(Shape* other, Points2& points, real start_time, real delta_time)
{
  // Do nothing
}

/*!
  Should be overriden by subclasses which wants particular actions to be performed with
  a certain regularity but which is not time critical enough to be performed each frame
  of game or step in simulation.
  
  Default implementation is to do nothing. 
*/
void Shape::doPlanning(real start_time, real delta_time)
{
  // Do nothing
}

void Shape::addListener(ShapeListener* listener) 
{
  iListeners.insert(listener);
}

void Shape::removeListener(ShapeListener* listener)
{
  iListeners.erase(listener);
}

void Shape::removeAllListeners()
{
  iListeners.clear();
}

/*!
  Killing a shape should delete a shape from the system
  Since a shape might be contained in several groups
  it is not possible to just call release on it an expect
  it to disappear because it might be retained in many different
  Groups or Shape groups. 
  
  kill will notify all listeners that we wish to delete shape.
  Listeners will typically be containers which will remove shape
  from their list and release it.
*/
void Shape::kill()
{
  // Notify all listeners that shape is killed
  for_each(iListeners.begin(), iListeners.end(),
    bind2nd(
      mem_fun(&ShapeListener::shapeKilled),
      this));
}
