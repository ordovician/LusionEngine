/*
 *  Group.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Group.h"
#include "Timing.h"
#include "Base/Command.h"
#include "Base/ShapeIterator.h"

#include <iostream>
#include <assert.h>

using namespace std;

/*!
    \class Group Group.h
    \brief Collection for Shapes. 

    Is in many ways similar to ShapeGroup. However ShapeGroup is not mutable
    Once it is created Shapes can't be added or removed. A Group on the otherhand
    can have Shapes added and removed at any time. The downside of this is that
    the structure can be as efficient in handling collision, because one can't create
    an optimized hierarchical structure.
*/

// Gloal functions
static Group gRenderGroup;

Group* renderGroup()
{
  return &gRenderGroup;
}

// Constructors
Group::Group() : iCurShape(0)
{
  
}

Group::~Group()
{
  cout << hex << "0x" << (int)this << " group removed" << endl;  // DEBUG  
}

// Accessors
int Group::noShapes() const
{
  return iShapes.size();
}

ShapeIterator* Group::shapeIterator() const
{
  ShapeIterator* itr = new SetShapeIterator(iShapes);
  itr->autorelease();
  return itr;
}

Rect2 Group::boundingBox() const
{
  return iBBox;
}

// Request
bool Group::contains(Shape* shape) const
{
  return iShapes.find(shape) != iShapes.end();
}

bool Group::isSimple() const
{
  return false;
}

// Operations
void Group::addKid(Shape* shape)
{
  assert(shape != 0);
  
  if (!contains(shape)) {
    shape->retain();
    iCurShape = iShapes.insert(shape).first;
    if (iShapes.size() == 0)
      iBBox = shape->boundingBox();
    else
      iBBox.surround(shape->boundingBox());
  }
}

void Group::removeKid(Shape* shape)
{
  assert(shape != 0);
  
  if (contains(shape)) {
    if (*iCurShape == shape) nextShape();
    iShapes.erase(shape);
    shape->release();
  }
}

/*!
  Updates all child shapes as well as updateting the boundingbox for group
*/
void Group::update(real start_time, real delta_time)
{
  if (iShapes.size() == 0)
    return;
    
  set<Shape*>::iterator shape = iShapes.begin();
  (*shape)->update(start_time, delta_time);
  Rect2 bbox = (*shape)->boundingBox();
 
  for (++shape; shape != iShapes.end(); ++shape) {
    (*shape)->update(start_time, delta_time);
    bbox.surround((*shape)->boundingBox());
  }
  iBBox = bbox;
}

void Group::doPlanning(real start_time, real delta_time)
{
//  Shape* shape = nextShape();
//  if (shape && shape->planCommand())
//    shape->planCommand()->execute(shape, 0, start_time, delta_time);
}
  
void Group::draw(const Rect2& r) const
{
  set<Shape*>::iterator shape = iShapes.begin();
  for (;shape != iShapes.end(); ++shape) {
    (*shape)->draw(r);
  }  
}

void Group::clear()
{
  iShapes.clear();
}

/*! 
  Does a round robin for shapes in group. That means at each call it will return
  a different shape in the group, until all the shapes have been returned at which
  point it start returning the first shapes again.
*/
Shape* Group::nextShape()
{
  if (iShapes.empty())
    return 0;
  Shape* s = *iCurShape;
  if (++iCurShape == iShapes.end())
    iCurShape = iShapes.begin();
  return s;
}
  

bool Group::collide(Shape* other, real t, real dt, SpriteCommand* command)
{
  bool is_col = false;
  set<Shape*>::iterator it;  
  for (it = iShapes.begin(); it != iShapes.end(); ++it) {
    Shape* shape = *it;
    if (secondsPassed() > t+dt)
      break;
      
    if (shape == other)
      continue;

    if(shape->collide(other, t, dt, command))
      is_col = true;
  }    
  return is_col;
}

bool Group::inside(const Point2& p, real t, real dt, SpriteCommand* command)
{
  bool is_col = false;
  set<Shape*>::iterator it;  
  for (it = iShapes.begin(); it != iShapes.end(); ++it) {
    Shape* shape = *it;

    if (secondsPassed() > t+dt)
      break;
      
    if(shape->inside(p, t, dt, command))
      is_col = true;
  }    
  return is_col;  
}