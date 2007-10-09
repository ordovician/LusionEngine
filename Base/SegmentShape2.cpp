/*
 *  SegmentShape2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Base/CollisionSegment2.h>
#include <Engine.h>
#include <Base/Command.h>
#include <Utils/PolygonUtils.h>

#include <iostream>

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
Rect2 SegmentShape2::boundingBox() const
{
  return Rect2(iSeg.left(), iSeg.right());
}
  
// Request
bool SegmentShape2::collide(Shape* other, real t, real dt, SpriteCommand* command)
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  bool is_colliding = other->intersect(iSeg);
  if (is_colliding && command != 0) 
    command->execute(this, other, t, dt);
  return is_colliding;  
}


// Calculations
bool SegmentShape2::intersect(const Circle& c) const
{
  return c.intersect(iSeg);
}

bool SegmentShape2::intersect(const Rect2& r) const
{
  return iSeg.intersect(r);  
}

bool SegmentShape2::intersect(const Segment2& s) const
{
  return iSeg.intersect(s);  
}

bool SegmentShape2::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  return ::intersect(iSeg, begin, end);
}

/*!
  Draws segment if it is inside rectangle \a r
*/
void SegmentShape2::draw(const Rect2& r) const
{
  glBegin(GL_LINES);      
    gltVertex(iSeg);
  glEnd();
}
