/*
 *  SegmentShape2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/SegmentShape2.h>
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
    \class SegmentShape2 SegmentShape2.h
    \brief Defines a segment2 shape.

    This class differs from Segment2 in that it is polymorphic so it
    can be used in collision hierarchies, it can be put in a scenegraph and
    drawn etc.
*/

// Constructors
SegmentShape2::SegmentShape2(const Segment2& seg) : iSeg(seg)
{
  
}

SegmentShape2::~SegmentShape2()
{
  
}

// Accessors
std::string
SegmentShape2::typeName() const  
{ 
  return "SegmentShape2"; 
}

Rect2 SegmentShape2::boundingBox() const
{
  return Rect2(iSeg.left(), iSeg.right());
}
  
// Request
bool SegmentShape2::collide(Shape* other, real t, real dt, CollisionAction* command)
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  Points2 points;
  bool is_colliding = other->intersection(iSeg, points);
  if (is_colliding && command != 0) 
    command->execute(this, other, points, t, dt);
  return is_colliding;  
}

bool SegmentShape2::inside(const Point2& p, real t, real dt, Action* command)
{
  if (iSeg.isOn(p)) {
    command->execute(this, t, dt);
    return true;      
  }
  return false;
}


// Calculations
bool SegmentShape2::intersection(const Circle& c, Points2& points) const
{
  return c.intersect(iSeg);
}

bool SegmentShape2::intersection(const Rect2& r, Points2& points) const
{
  return iSeg.intersect(r);  
}

bool SegmentShape2::intersection(const Segment2& s, Points2& points) const
{
  return iSeg.intersect(s);  
}

bool SegmentShape2::intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Points2& points) const
{
  return ::intersect(iSeg, begin, end);
}

/*!
  Draws segment if it is inside rectangle \a r
*/
void SegmentShape2::draw(const Rect2& r) const
{
#ifndef UNIT_TEST
  glBegin(GL_LINES);      
    gltVertex(iSeg);
  glEnd();
#endif
}
