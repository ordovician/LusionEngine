/*
 *  CollisionRect2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/CollisionRect2.h>
#include <Engine.h>
#include <Base/Command.h>
#include <Utils/PolygonUtils.h>

#include <iostream>

using namespace std;

/*!
    \class CollisionRect2 CollisionRect2.h
    \brief Defines a circle shape.

    This class differs from Circle in that it is polymorphic so it
    can be used in collision hierarchies, it can be put in a scenegraph and
    drawn etc.
*/

// Constructors
CollisionRect2::CollisionRect2(const Rect2& rect) : iRect(rect)
{
  
}

CollisionRect2::~CollisionRect2()
{
  
}

// Accessors
Rect2 CollisionRect2::boundingBox() const
{
  return iRect;
}
  
// Request
bool CollisionRect2::collide(CollisionObject* other, real t, real dt, SpriteCommand* command)
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  bool is_colliding = other->intersect(iRect);
  if (is_colliding && command != 0) 
    command->execute(this, other, t, dt);
  return is_colliding;  
}


// Calculations
bool CollisionRect2::intersect(const Circle& c) const
{
  return c.intersect(iRect);
}

bool CollisionRect2::intersect(const Rect2& r) const
{
  return iRect.intersect(r);  
}

bool CollisionRect2::intersect(const Segment2& s) const
{
  return s.intersect(iRect);  
}

bool CollisionRect2::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  return ::intersect(iRect, begin, end);
}

/*!
  Draws circle if it is inside rectangle \a r
*/
void CollisionRect2::draw(const Rect2& r) const
{
  glBegin(GL_LINES);    
    gltVertex(iRect);
  glEnd();
}
