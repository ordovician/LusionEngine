/*
 *  Command.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"
#include <Geometry/Rect2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Vector2.hpp>
#include <Core/SharedObject.hpp>

class CollisionObject;
class Sprite;
 
/*!
   \class SpriteCommand Command.h
   \brief 


*/
class SpriteCommand : public SharedObject
{
public:    
  // Operations
  virtual bool execute(CollisionObject* me, CollisionObject* other, real start_time, real delta_time) = 0;
  
};

class BinarySpriteCollisionCommand : public SpriteCommand
{
public:
 // Constructors
 BinarySpriteCollisionCommand();
 virtual ~BinarySpriteCollisionCommand();

 // Operations
 bool execute(CollisionObject* me, CollisionObject* other, real start_time, real delta_time);
};

class InsideCommand : public SpriteCommand
{
public:
 // Constructors
 InsideCommand(const Point2& point);
 virtual ~InsideCommand();

 // Operations
 bool execute(CollisionObject* me, CollisionObject*, real start_time, real delta_time);

private:
  Point2 iPoint;
};

class IntersectCommand : public SpriteCommand
{
public:
 // Constructors
 IntersectCommand(Sprite* sprite);
 virtual ~IntersectCommand();

 // Operations
 bool execute(CollisionObject* other, CollisionObject*, real start_time, real delta_time);

private:
  Sprite* me;
};

class CircleIntersectCommand : public SpriteCommand
{
public:
 // Constructors
 CircleIntersectCommand(const Circle &circle);
 virtual ~CircleIntersectCommand();

 // Operations
 bool execute(CollisionObject* me, CollisionObject* other, real start_time, real delta_time);

private:
  Circle iCircle;
};

class RectIntersectCommand : public SpriteCommand
{
public:
 // Constructors
 RectIntersectCommand(const Rect2 &box);
 virtual ~RectIntersectCommand();

 // Operations
 bool execute(CollisionObject* me, CollisionObject* other, real start_time, real delta_time);

private:
  Rect2 iBox;
};

class SegmentIntersectCommand : public SpriteCommand
{
public:
 // Constructors
 SegmentIntersectCommand(const Segment2 &seg);
 virtual ~SegmentIntersectCommand();

 // Operations
 bool execute(CollisionObject* me, CollisionObject* other, real start_time, real delta_time);

private:
  Segment2 iSeg;
};

struct lua_State;

class LuaCommand : public SpriteCommand
{
public:
 // Constructors
 LuaCommand(lua_State* aL);
 virtual ~LuaCommand();

 // Operations
 bool execute(CollisionObject* me, CollisionObject* other, real start_time, real delta_time);
 
private:
  lua_State* L;
  int iCommandRef;
};

