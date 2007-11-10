/*
 *  ShapeGroup.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/CollisionGroup.h>
#include <Engine.h>
#include <Base/Command.h>

#include <Utils/Algorithms.h>
#include "Utils/Exception.h"

#include <iostream>

using namespace std;

// Helper functions

/*! 
  Kind of spatially sort Shapes along 'axis' axis and return iterator to the Shape
  placed right before the center of spatial sort. "Kind of" means that it is not fully sorted
  only the first half is sorted well enough to establish pivot iterator.
*/
static vector<Shape*>::iterator qsplit(
  vector<Shape*>::iterator begin, 
  vector<Shape*>::iterator end,
  real pivot, 
  int axis)
{
    Rect2 bbox;
    real centroid;
    vector<Shape*>::iterator ret_value = begin;
    
    // Loop through and make sure bounding boxes are in spatialy ordered along 'axis'
    // such that the ones spatially to the left of pivot comes eariler in list than
    // those spatially to the right of pivot.
    vector<Shape*>::iterator it;     
    for (it = begin; it != end; ++it) {
        bbox = (*it)->boundingBox();
        centroid = bbox.center()[axis];   //  Order rectangles on center line throug rectangle (centroid)
        if (centroid >= pivot)
            continue;
        Shape* temp = *it;
        *it = *ret_value;
        *ret_value = temp;
        ++ret_value;
    }
    if (ret_value == begin || ret_value == end)
        ret_value = begin+(end-begin)/2;

    return ret_value;
}

// Constructors
ShapeGroup::ShapeGroup()
{
  init();
}

ShapeGroup::ShapeGroup(ShapeIterator* source)
{
  vector<Shape*> shapes;
  MutableVectorShapeIterator dest(shapes);
  
  Util::insert(source, &dest); 
  init(shapes.begin(), shapes.end());
}

ShapeGroup::ShapeGroup(vector<Shape*>::iterator begin, vector<Shape*>::iterator end)
{
  init(begin, end);   
}

ShapeGroup::ShapeGroup(Shape *left, Shape *right)
{
  init(left, right);
}

ShapeGroup::ShapeGroup(Shape *left, Shape *right, const Rect2& box)
{
  init(left, right, box);
}

ShapeGroup::~ShapeGroup()
{
  // cout << hex << "0x" << (int)this << " collision group removed" << endl;  // DEBUG    
  iLeft->release();
  iRight->release();
}

// Accessors
Rect2 ShapeGroup::boundingBox() const
{
  return iBox;
}

/*!
  Returns number of shapes in group, but this is not a supported operation on
  ShapeGroup so calling method will cause an exception.
  
  While group might have more than one shape inside it will always return 0 and
  throw an exception because it is only possible to discover the number of shapes
  by traversing the hierarchy, but that requires knowing the types of the 
  child shapes which is not allowed.
  
  \throw UnsuportedMethodException
*/
int ShapeGroup::noShapes() const
{
  throw UnsuportedMethodException();
  return 0; 
}

/*!
  Returns an iterator for group, but this is not a supported operation on
  ShapeGroup so calling method will cause an exception.

  While group might have more than one shape inside it will always return 0 and
  throw an exception because it is only possible to discover the number of shapes
  by traversing the hierarchy, but that requires knowing the types of the 
  child shapes which is not allowed.
  
  \throw UnsuportedMethodException
*/
ShapeIterator* ShapeGroup::shapeIterator() const
{
  throw UnsuportedMethodException();  
  return 0;
}

// Request
bool ShapeGroup::isSimple() const
{
  return false;
}

// Calculations
/*!
  Returns true if there was a collision with \a other Shape. \a command is executed
  on every Shape in ShapeGroup which collided with \a other, and if \a other is also
  a ShapeGroup then every Shape in that group which collided with a Shape in
  this group. If calculations take too long time, meaning more than \a dt time is spent since
  time \a t and now, then false will be returned even if there really was a collision.
*/
bool ShapeGroup::collide(Shape* other, real t, real dt, SpriteCommand* command)
{
  if (!iBox.intersect(other->boundingBox()))
    return false;
    
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->collide(other, t, dt, command);
  if (iRight) hit_right = iRight->collide(other, t, dt, command);
  
  return hit_right || hit_left;  
}

bool ShapeGroup::inside(const Point2& p, real t, real dt, SpriteCommand* command)
{
  if (!iBox.inside(p))
    return false;
    
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->inside(p, t, dt, command);
  if (iRight) hit_right = iRight->inside(p, t, dt, command);
  
  return hit_right || hit_left;    
}

/*!
  Draw all child components which are inside or intersect rectangle \a r.
  It is optimized to do as few intersection tests as possible.
*/
void ShapeGroup::draw(const Rect2& r) const
{
  if (!iBox.intersect(r))
    return;
      
  iLeft->draw(r);
  iRight->draw(r); 
}

// Operations
void ShapeGroup::update(real start_time, real delta_time) 
{
  if (iLeft) iLeft->update(start_time, delta_time);
  if (iRight) iRight->update(start_time, delta_time);
}

void ShapeGroup::init()
{
  iLeft = 0;
  iRight = 0;
}

void ShapeGroup::init(Shape *left, Shape *right)
{
  left->retain();
  right->retain();
  
  iLeft = left; 
  iRight = right;
  iBox = left->boundingBox().surround(right->boundingBox());
}

void ShapeGroup::init(Shape *left, Shape *right, const Rect2& box)
{
  left->retain();
  right->retain();
  
  iLeft = left;
  iRight = right;
  iBox = box;
}

void ShapeGroup::init(vector<Shape*>::iterator begin, vector<Shape*>::iterator end)
{
  init();
    
  int no_nodes = end-begin;      
  if (no_nodes == 0) {
    cerr << "Can't group sprites because none was provided!" << endl;
    return;
  }
  if (no_nodes == 1) 
    init(*begin, *begin);
  else if (no_nodes == 2) 
    init(*begin, *(begin+1));
  
  // Find the midpoint of the bounding box to use as a qsplit pivot
  iBox = (*begin)->boundingBox();
  vector<Shape*>::iterator it = begin;
  while (++it != end) {
      iBox = iBox.surround((*it)->boundingBox());
  }
  Point2 pivot = iBox.center();
  vector<Shape*>::iterator mid_point = qsplit(begin, end, pivot.x(), 0);
  
  // Create a new bounding volume
  iLeft = buildBranch(begin, mid_point, 1);
  iRight = buildBranch(mid_point, end, 1); 
  
  iLeft->retain();
  iRight->retain(); 
}

Shape* ShapeGroup::buildBranch(
  vector<Shape*>::iterator begin,
  vector<Shape*>::iterator end,
  int axis)
{
  Shape* ret_obj = 0;  
  int no_nodes = end-begin;      
  if (no_nodes == 0) {
    cerr << "Can't group sprites because none was provided!" << endl;
  }        
  else if (no_nodes == 1) {
    ret_obj = *begin;
    ret_obj->retain();
  }
  else if (no_nodes == 2) {
    ret_obj = new ShapeGroup(*begin, *(begin+1));    
  }
  else {
    // Find the midpoint of the bounding box to use as a qsplit pivot
    Rect2 box = (*begin)->boundingBox();
    vector<Shape*>::iterator it = begin;
    while (++it != end) {
        box = box.surround((*it)->boundingBox());
    }
    Vector2 pivot = box.center();

    // Now split according to correct axis
    vector<Shape*>::iterator mid_point = qsplit(begin, end, pivot[axis], axis);

    // Create a new bounding volume
    Shape* left = buildBranch(begin, mid_point, (axis+1)%2);
    Shape* right = buildBranch(mid_point, end, (axis+1)%2);
    ret_obj = new ShapeGroup(left, right, box);    
  }
  ret_obj->autorelease();
  return ret_obj;
}