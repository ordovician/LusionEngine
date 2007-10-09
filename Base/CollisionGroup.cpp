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

#include <iostream>

using namespace std;

// Helper functions

/*! 
  Kind of spatially sort Shapes along 'axis' axis and return iterator to the Shape
  placed right before the center of spatial sort. "Kind of" means that it is not fully sorted
  only the first half is sorted well enough to establish pivot iterator.
*/
static ShapeIterator qsplit(ShapeIterator begin, 
                                             ShapeIterator end,
                                             real pivot, int axis)
{
    Rect2 bbox;
    real centroid;
    ShapeIterator ret_value = begin;
    
    // Loop through and make sure bounding boxes are in spatialy ordered along 'axis'
    // such that the ones spatially to the left of pivot comes eariler in list than
    // those spatially to the right of pivot.
    ShapeIterator it;     
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

ShapeGroup::ShapeGroup(ShapeIterator begin, ShapeIterator end)
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

void ShapeGroup::init(ShapeIterator begin, ShapeIterator end)
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
  ShapeIterator it = begin;
  while (++it != end) {
      iBox = iBox.surround((*it)->boundingBox());
  }
  Point2 pivot = iBox.center();
  ShapeIterator mid_point = qsplit(begin, end, pivot.x(), 0);
  
  // Create a new bounding volume
  iLeft = buildBranch(begin, mid_point, 1);
  iRight = buildBranch(mid_point, end, 1); 
  
  iLeft->retain();
  iRight->retain(); 
}

Shape* ShapeGroup::buildBranch(ShapeIterator begin, 
                              ShapeIterator end, 
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
    ShapeIterator it = begin;
    while (++it != end) {
        box = box.surround((*it)->boundingBox());
    }
    Vector2 pivot = box.center();

    // Now split according to correct axis
    ShapeIterator mid_point = qsplit(begin, end, pivot[axis], axis);

    // Create a new bounding volume
    Shape* left = buildBranch(begin, mid_point, (axis+1)%2);
    Shape* right = buildBranch(mid_point, end, (axis+1)%2);
    ret_obj = new ShapeGroup(left, right, box);    
  }
  ret_obj->autorelease();
  return ret_obj;
}