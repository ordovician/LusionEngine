/*
 *  Command.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Command.h"
#include "Base/Sprite.h"

#include "Lua/LuaUtils.h"
#include "Lua/Base/LuaSprite.h"

#include <lua.hpp>

#include <iostream>

using namespace std;

// helper functions
static bool handleCollision(Sprite* me, Sprite* other, real t, real dt)
{
  if (me != other && me != 0 && other != 0 && me->collision(other)) {
    SpriteCommand* me_cmd = me->collisionCommand();
    SpriteCommand* other_cmd = other->collisionCommand();
    
    bool me_ok = true, other_ok = true;
    
    if (me_cmd) 
      me_ok = me_cmd->execute(me, other, t, dt);
    if (other_cmd) 
      other_ok = other_cmd->execute(other, me, t, dt);
      
    return me_ok || other_ok;
  }
  return false;    
}


/*!
   \class BinarySpriteCollisionCommand Command.h
   \brief 


*/

// Constructors
BinarySpriteCollisionCommand::BinarySpriteCollisionCommand()
{
  
}

BinarySpriteCollisionCommand::~BinarySpriteCollisionCommand()
{
  // cout << hex << "0x" << (int)this << " BinarySpriteCollisionCommand removed" << endl;  // DEBUG    
}

// Operations
bool BinarySpriteCollisionCommand::execute(CollisionObject* me_obj, CollisionObject* other_obj, real t, real dt) 
{  
  Sprite* me = dynamic_cast<Sprite*>(me_obj);
  Sprite* other = dynamic_cast<Sprite*>(other_obj);

  return handleCollision(me, other, t, dt);
}

/*!
   \class InsideCommand Command.h
   \brief 


*/

// Constructors
InsideCommand::InsideCommand(const Point2& point)
  : iPoint(point)
{

}

InsideCommand::~InsideCommand()
{
  // cout << hex << "0x" << (int)this << " InsideCommand removed" << endl;  // DEBUG      
}

// Operations
bool InsideCommand::execute(CollisionObject* me_obj, CollisionObject*, real t, real dt) 
{          
  const Rect2& box = me_obj->boundingBox();
  if (!box.inside(iPoint)) 
    return false;
    
  Sprite* me = dynamic_cast<Sprite*>(me_obj);  
  if (!me)
    return true;
  
  if (me->inside(iPoint)) {
    SpriteCommand* cmd = me->insideCommand();
    if (cmd) return cmd->execute(me, 0, t, dt);
    return true;  
  }
  return false;
}

/*!
   \class IntersectCommand Command.h
   \brief 


*/

// Constructors
IntersectCommand::IntersectCommand(Sprite* sprite)
  : me(sprite)
{
  assert(sprite != 0);
}

IntersectCommand::~IntersectCommand()
{
  // cout << hex << "0x" << (int)this << " IntersectCommand removed" << endl;  // DEBUG        
}

// Operations
bool IntersectCommand::execute(CollisionObject* other_obj, CollisionObject*, real t, real dt) 
{      
  const Rect2& box = me->boundingBox();
  if (!box.intersect(other_obj->boundingBox())) 
    return false;
    
  Sprite* other = dynamic_cast<Sprite*>(other_obj);
  if (!other)
    return true;

  return handleCollision(me, other, t, dt);  
}

/*!
   \class CircleIntersectCommand Command.h
   \brief 


*/

// Constructors
CircleIntersectCommand::CircleIntersectCommand(const Circle &circle)
  : iCircle(circle)
{

}

CircleIntersectCommand::~CircleIntersectCommand()
{
  // cout << hex << "0x" << (int)this << " CircleIntersectCommand removed" << endl;  // DEBUG        
}

// Operations
bool CircleIntersectCommand::execute(CollisionObject* me_obj, CollisionObject* other_obj, real t, real dt) 
{ 
  if (iCircle.intersect(me_obj->boundingBox())) {
    Sprite* me = dynamic_cast<Sprite*>(me_obj);
    if (me) {
      ShallowPoints2 poly = me->collisionPolygon();
      return iCircle.intersect(poly.first, poly.second);
    }
    else
      return true;
  }
  return false;
}

/*!
   \class RectIntersectCommand Command.h
   \brief 


*/

// Constructors
RectIntersectCommand::RectIntersectCommand(const Rect2 &rect)
  : iBox(rect)
{

}

RectIntersectCommand::~RectIntersectCommand()
{
  // cout << hex << "0x" << (int)this << " RectIntersectCommand removed" << endl;  // DEBUG        
}

// Operations
bool RectIntersectCommand::execute(CollisionObject* me_obj, CollisionObject* other_obj, real t, real dt) 
{      
  return iBox.intersect(me_obj->boundingBox()); 
}

/*!
   \class SegmentIntersectCommand Command.h
   \brief 


*/

// Constructors
SegmentIntersectCommand::SegmentIntersectCommand(const Segment2 &seg)
  : iSeg(seg)
{

}

SegmentIntersectCommand::~SegmentIntersectCommand()
{
  // cout << hex << "0x" << (int)this << " SegmentIntersectCommand removed" << endl;  // DEBUG        
}

// Operations
bool SegmentIntersectCommand::execute(CollisionObject* me_obj, CollisionObject* other_obj, real t, real dt) 
{      
  return me_obj->boundingBox().intersect(Rect2(iSeg.left(), iSeg.right()));
}

/*!
   \class LuaCommand Command.h
   \brief 


*/

// Constructors
LuaCommand::LuaCommand(lua_State* aL)
  : L(aL)
{
  if (lua_isfunction(L, -1)) {
    iCommandRef = luaL_ref(L, LUA_REGISTRYINDEX);    
  }
  else {
    iCommandRef = LUA_NOREF;
    luaL_error(L, "Expected a function object but got a '%s'", lua_typename(L, lua_type(L,-1)));
  }
}

LuaCommand::~LuaCommand()
{
  luaL_unref(L, LUA_REGISTRYINDEX, iCommandRef);
  // cout << hex << "0x" << (int)this << " LuaCommand removed" << endl;  // DEBUG         
}

// Operations
bool LuaCommand::execute(CollisionObject* me_obj, CollisionObject* other_obj, real t, real dt) 
{      
  Sprite* me = dynamic_cast<Sprite*>(me_obj);
  Sprite* other = dynamic_cast<Sprite*>(other_obj);
  
  lua_rawgeti(L, LUA_REGISTRYINDEX, iCommandRef);
  if (me)
    retrieveSpriteTable(L, me);
  else
    lua_pushnil(L);
  if (other)
    retrieveSpriteTable(L, other);  
  else
    lua_pushnil(L);
  lua_pushnumber(L, t);
  lua_pushnumber(L, dt);  
  lua_call(L, 4, 1);     
  bool ret = lua_toboolean(L,-1);
  lua_pop(L,1);
  
  return ret;  
}
