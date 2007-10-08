/*
 *  CollisionCircle.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/CollisionCircle.h>
#include <Engine.h>
#include <Base/Command.h>
#include <Utils/PolygonUtils.h>

#include <iostream>

using namespace std;

/*!
    \class CollisionCircle CollisionCircle.h
    \brief Defines a circle shape.

    This class differs from Circle in that it is polymorphic so it
    can be used in collision hierarchies, it can be put in a scenegraph and
    drawn etc.
*/

// Constructors
CollisionCircle::CollisionCircle(const Circle& circle) : iCircle(circle)
{
  
}

CollisionCircle::~CollisionCircle()
{
  
}

// Accessors
Rect2 CollisionCircle::boundingBox() const
{
  Rect2 r(Vector2(0.0f, 0.0f), Vector2(iCircle.radius()*2.0f, iCircle.radius()*2.0f));
  r.moveCenter(iCircle.center());
  return r;
}
  
// Request
bool CollisionCircle::collide(CollisionObject* other, real t, real dt, SpriteCommand* command)
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  bool is_colliding = other->intersect(iCircle);
  if (is_colliding && command != 0) 
    command->execute(this, other, t, dt);
  return is_colliding;  
}


// Calculations
bool CollisionCircle::intersect(const Circle& c) const
{
  return iCircle.intersect(c);
}

bool CollisionCircle::intersect(const Rect2& r) const
{
  return iCircle.intersect(r);  
}

bool CollisionCircle::intersect(const Segment2& s) const
{
  return iCircle.intersect(s);  
}

bool CollisionCircle::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  return iCircle.intersect(begin, end);
}

/*!
  Draws circle if it is inside rectangle \a r
*/
void CollisionCircle::draw(const Rect2& r) const
{
  glPushMatrix();
    gltTranslate(iCircle.center());
    drawCircle(iCircle.radius());
  glPopMatrix();  
}
