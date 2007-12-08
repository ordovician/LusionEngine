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
#include <Base/Action.h>
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
  
const Point2& CircleShape::center() const
{
  return iCircle.center();
}

real CircleShape::radius() const 
{
  return iCircle.radius();
} 
 
// Request
bool CircleShape::collide(Shape* other, real t, real dt, CollisionAction* command)
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  Points2 points;
  bool is_colliding = other->intersection(iCircle, points);
  if (is_colliding && command != 0) 
    command->execute(this, other, points, t, dt);
  return is_colliding;  
}

bool CircleShape::inside(const Point2& p, real t, real dt, Action* command)
{
  if (iCircle.inside(p)) {
    if (command)
      command->execute(this, t, dt);
    return true;      
  }
  return false;
}

// Calculations
bool CircleShape::intersection(const Circle& c, Points2& points) const
{
  return iCircle.intersect(c);
}

bool CircleShape::intersection(const Rect2& r, Points2& points) const
{
  return iCircle.intersect(r);  
}

bool CircleShape::intersection(const Segment2& s, Points2& points) const
{
  return iCircle.intersect(s);  
}

bool CircleShape::intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Points2& points) const
{
  return iCircle.intersection(begin, end, points);
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
