/*
 *  CollisionGroup.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/CollisionGroup.h"
#include "Engine.h"
#include "Base/Command.h"

#include <iostream>

using namespace std;

// Helper functions

/*! 
  Kind of spatially sort CollisionObjects along 'axis' axis and return iterator to the CollisionObject
  placed right before the center of spatial sort. "Kind of" means that it is not fully sorted
  only the first half is sorted well enough to establish pivot iterator.
*/
static CollisionObjectIterator qsplit(CollisionObjectIterator begin, 
                                             CollisionObjectIterator end,
                                             real pivot, int axis)
{
    Rect2 bbox;
    real centroid;
    CollisionObjectIterator ret_value = begin;
    
    // Loop through and make sure bounding boxes are in spatialy ordered along 'axis'
    // such that the ones spatially to the left of pivot comes eariler in list than
    // those spatially to the right of pivot.
    CollisionObjectIterator it;     
    for (it = begin; it != end; ++it) {
        bbox = (*it)->boundingBox();
        centroid = bbox.center()[axis];   //  Order rectangles on center line throug rectangle (centroid)
        if (centroid >= pivot)
            continue;
        CollisionObject* temp = *it;
        *it = *ret_value;
        *ret_value = temp;
        ++ret_value;
    }
    if (ret_value == begin || ret_value == end)
        ret_value = begin+(end-begin)/2;

    return ret_value;
}

// Constructors
CollisionGroup::CollisionGroup()
{
  init();
}

CollisionGroup::CollisionGroup(CollisionObjectIterator begin, CollisionObjectIterator end)
{
  init(begin, end);   
}

CollisionGroup::CollisionGroup(CollisionObject *left, CollisionObject *right)
{
  init(left, right);
}

CollisionGroup::CollisionGroup(CollisionObject *left, CollisionObject *right, const Rect2& box)
{
  init(left, right, box);
}

CollisionGroup::~CollisionGroup()
{
  // cout << hex << "0x" << (int)this << " collision group removed" << endl;  // DEBUG    
  iLeft->release();
  iRight->release();
}

// Accessors
Rect2 CollisionGroup::boundingBox() const
{
  return iBox;
}

// Request
bool CollisionGroup::intersect(const Circle& circle) const
{
  if (!circle.intersect(boundingBox())) return false;
  
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->intersect(circle);
  if (iRight) hit_right = iRight->intersect(circle);
  
  return hit_right || hit_left;  
}

bool CollisionGroup::intersect(const Rect2& rect) const
{
  if (!rect.intersect(boundingBox())) return false;
  
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->intersect(rect);
  if (iRight) hit_right = iRight->intersect(rect);
  
  return hit_right || hit_left;    
}

bool CollisionGroup::intersect(const Segment2& seg) const
{
  if (!boundingBox().intersect(Rect2(seg.left(), seg.right()))) return false;
  
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->intersect(seg);
  if (iRight) hit_right = iRight->intersect(seg);
  
  return hit_right || hit_left;      
}

// Calculations
bool CollisionGroup::traverse(real t, real dt, SpriteCommand* command)
{
  assert(command != 0);  
  if (!command->execute(this, 0, t, dt)) return false;
  
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->traverse(t, dt, command);
  if (iRight) hit_right = iRight->traverse(t, dt, command);
  
  return hit_right || hit_left;
}

// Operations
void CollisionGroup::update(real start_time, real delta_time) 
{
  if (iLeft) iLeft->update(start_time, delta_time);
  if (iRight) iRight->update(start_time, delta_time);
}

void CollisionGroup::init()
{
  iLeft = 0;
  iRight = 0;
}

void CollisionGroup::init(CollisionObject *left, CollisionObject *right)
{
  left->retain();
  right->retain();
  
  iLeft = left; 
  iRight = right;
  iBox = left->boundingBox().surround(right->boundingBox());
}

void CollisionGroup::init(CollisionObject *left, CollisionObject *right, const Rect2& box)
{
  left->retain();
  right->retain();
  
  iLeft = left;
  iRight = right;
  iBox = box;
}

void CollisionGroup::init(CollisionObjectIterator begin, CollisionObjectIterator end)
{
  init();
  
  // CollisionObjectIterator i;
  // for (i = begin; i != end; ++i) (*i)->retain();
  
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
  CollisionObjectIterator it = begin;
  while (++it != end) {
      iBox = iBox.surround((*it)->boundingBox());
  }
  Point2 pivot = iBox.center();
  CollisionObjectIterator mid_point = qsplit(begin, end, pivot.x(), 0);
  
  // Create a new bounding volume
  iLeft = buildBranch(begin, mid_point, 1);
  iRight = buildBranch(mid_point, end, 1); 
  
  iLeft->retain();
  iRight->retain(); 
}

CollisionObject* CollisionGroup::buildBranch(CollisionObjectIterator begin, 
                              CollisionObjectIterator end, 
                              int axis)
{
  CollisionObject* ret_obj = 0;  
  int no_nodes = end-begin;      
  if (no_nodes == 0) {
    cerr << "Can't group sprites because none was provided!" << endl;
  }        
  else if (no_nodes == 1) {
    ret_obj = *begin;
    ret_obj->retain();
  }
  else if (no_nodes == 2) {
    ret_obj = new CollisionGroup(*begin, *(begin+1));    
  }
  else {
    // Find the midpoint of the bounding box to use as a qsplit pivot
    Rect2 box = (*begin)->boundingBox();
    CollisionObjectIterator it = begin;
    while (++it != end) {
        box = box.surround((*it)->boundingBox());
    }
    Vector2 pivot = box.center();

    // Now split according to correct axis
    CollisionObjectIterator mid_point = qsplit(begin, end, pivot[axis], axis);

    // Create a new bounding volume
    CollisionObject* left = buildBranch(begin, mid_point, (axis+1)%2);
    CollisionObject* right = buildBranch(mid_point, end, (axis+1)%2);
    ret_obj = new CollisionGroup(left, right, box);    
  }
  ret_obj->autorelease();
  return ret_obj;
}