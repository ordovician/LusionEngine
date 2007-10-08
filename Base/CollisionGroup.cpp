/*
 *  CollisionGroup.cpp
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
bool CollisionGroup::isSimple() const
{
  return false;
}

// Calculations
/*!
  Returns true if there was a collision with \a other CollisionObject. \a command is executed
  on every CollisionObject in CollisionGroup which collided with \a other, and if \a other is also
  a CollisionGroup then every CollisionObject in that group which collided with a CollisionObject in
  this group. If calculations take too long time, meaning more than \a dt time is spent since
  time \a t and now, then false will be returned even if there really was a collision.
*/
bool CollisionGroup::collide(CollisionObject* other, real t, real dt, SpriteCommand* command)
{
  if (!iBox.intersect(other->boundingBox()))
    return false;
    
  bool hit_left = false;
  bool hit_right = false;
  
  if (iLeft) hit_left = iLeft->collide(other, t, dt, command);
  if (iRight) hit_right = iRight->collide(other, t, dt, command);
  
  return hit_right || hit_left;  
}

bool CollisionGroup::inside(const Point2& p, real t, real dt, SpriteCommand* command)
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
void CollisionGroup::draw(const Rect2& r) const
{
  if (!iBox.intersect(r))
    return;
      
  iLeft->draw(r);
  iRight->draw(r); 
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