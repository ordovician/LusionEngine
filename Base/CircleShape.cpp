/*
 *  CircleShape.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/CircleShape.h>
#include <Engine.h>
#include <Base/Command.h>
#include <Utils/PolygonUtils.h>

#include <iostream>

using namespace std;

/*!
    \class CircleShape CircleShape.h
    \brief Defines a circle shape.

    This class differs from Circle in that it is polymorphic so it
    can be used in collision hierarchies, it can be put in a scenegraph and
    drawn etc.
*/

// Constructors
CircleShape::CircleShape(const Circle& circle) : iCircle(circle)
{
  
}

CircleShape::~CircleShape()
{
  
}

// Accessors
Rect2 CircleShape::boundingBox() const
{
  Rect2 r(Vector2(0.0f, 0.0f), Vector2(iCircle.radius()*2.0f, iCircle.radius()*2.0f));
  r.moveCenter(iCircle.center());
  return r;
}
  
// Request
bool CircleShape::collide(Shape* other, real t, real dt, SpriteCommand* command)
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

bool CircleShape::inside(const Point2& p, real t, real dt, SpriteCommand* command)
{
  if (iCircle.inside(p)) {
    if (command)
      command->execute(this, 0, t, dt);
    return true;      
  }
  return false;
}

// Calculations
bool CircleShape::intersect(const Circle& c) const
{
  return iCircle.intersect(c);
}

bool CircleShape::intersect(const Rect2& r) const
{
  return iCircle.intersect(r);  
}

bool CircleShape::intersect(const Segment2& s) const
{
  return iCircle.intersect(s);  
}

bool CircleShape::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  return iCircle.intersect(begin, end);
}

/*!
  Draws circle if it is inside rectangle \a r
*/
void CircleShape::draw(const Rect2& r) const
{
  // glPushMatrix();
  //   gltTranslate(iCircle.center());
  //   drawCircle(iCircle.radius());
  // glPopMatrix();  
}
