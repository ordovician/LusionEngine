/*
 *  RectShape2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/RectShape2.h>
#include <Engine.h>
#include <Base/Action.h>
#include <Utils/PolygonUtils.h>
#include <Core/Core.h>

#include <iostream>

#ifndef UNIT_TEST
#include "Utils/GLUtils.h"

#include <OpenGL/gl.h>
#endif

using namespace std;

/*!
    \class RectShape2 RectShape2.h
    \brief Defines a circle shape.

    This class differs from Circle in that it is polymorphic so it
    can be used in collision hierarchies, it can be put in a scenegraph and
    drawn etc.
*/

// Constructors
RectShape2::RectShape2(const Rect2& rect) : iRect(rect)
{
  
}

RectShape2::~RectShape2()
{
  
}

// Accessors
std::string
RectShape2::typeName() const  
{ 
  return "RectShape2"; 
}

Rect2 RectShape2::boundingBox() const
{
  return iRect;
}
  
// Request
bool RectShape2::collide(Shape* other, real t, real dt, CollisionAction* command)
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  Points2 points;
  bool is_colliding = other->intersection(iRect, points);
  if (is_colliding && command != 0) 
    command->execute(this, other, points, t, dt);
  return is_colliding;  
}

bool RectShape2::inside(const Point2& p, real t, real dt, Action* command)
{
  if (iRect.inside(p)) {
    command->execute(this, t, dt);
    return true;      
  }
  return false;
}


// Calculations
bool RectShape2::intersection(const Circle& c, Points2& ) const
{
  return c.intersect(iRect);
}

bool RectShape2::intersection(const Rect2& r, Points2& ) const
{
  return iRect.intersect(r);  
}

bool RectShape2::intersection(const Segment2& s, Points2& ) const
{
  return s.intersect(iRect);  
}

bool RectShape2::intersection(const Polygon2& poly, Points2& ) const
{
  return poly.intersect(iRect);
}

/*!
  Draws circle if it is inside rectangle \a r
*/
void RectShape2::draw(const Rect2& ) const
{
#ifndef UNIT_TEST
  glBegin(GL_LINES);    
    gltVertex(iRect);
  glEnd();
#endif
}
