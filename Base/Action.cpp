/*
 *  Action.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Action.h"
#include "Base/Sprite.h"

#include "Lua/LuaUtils.h"
#include "Lua/Base/LuaSprite.h"

#include <lua.hpp>

#include <iostream>

using namespace std;
/*!
  \class Action Action.h
  \brief Used to mimic a unary function object for C++

  Implementation of the command pattern for performing actions on shape
  objects. Typically the action object is passed to a function that 
  traverses or iterates over a collection of shape objects in a particular way.
  
  These function will then call execute() for each shape object and provide
  the shape object as first argument. An Action object is basically and alternative
  to the code you would put between the curcly braces of a for loop iterating over
  a collection of shape objects. 
  
  The reason for using this approach over an iterator is that it is quite difficult
  to provide an iterator for shapes across all kinds of collection classes.
*/

/*!
  \class CollisionAction Action.h
  \brief Used to mimic a binary function object for C++

  CollisionAction is used to represent function objects specifying what
  happens when two shapes collide. To create an action just subclass this class.
*/

/*!
  \class LuaAction Action.h
  \brief 

  Wrapper for a Lua function object. This is lua functions can be called from 
  C++ code. It allows one to specify custome behaviour for when e.g. Sprites collide.
*/

// Constructors
LuaAction::LuaAction(lua_State* aL)
  : L(aL)
{
  if (lua_isfunction(L, -1)) {
    iActionRef = luaL_ref(L, LUA_REGISTRYINDEX);    
  }
  else {
    iActionRef = LUA_NOREF;
    luaL_error(L, "Expected a function object but got a '%s'", lua_typename(L, lua_type(L,-1)));
  }
}

LuaAction::~LuaAction()
{
  luaL_unref(L, LUA_REGISTRYINDEX, iActionRef);
  // cout << hex << "0x" << (int)this << " LuaCollisionAction removed" << endl;  // DEBUG         
}

// Operations
bool LuaAction::execute(Shape* me_obj, real t, real dt) 
{      
  Sprite* me = dynamic_cast<Sprite*>(me_obj);
  
  lua_rawgeti(L, LUA_REGISTRYINDEX, iActionRef);
  if (me)
    retrieveSpriteTable(L, me);
  else
    lua_pushnil(L);

  lua_pushnumber(L, t);
  lua_pushnumber(L, dt);  
  lua_call(L, 3, 1);     
  bool ret = lua_toboolean(L,-1);
  lua_pop(L,1);
  
  return ret;  
}

/*!
  \class LuaCollisionAction Action.h
  \brief 

  Wrapper for a Lua function object. This is lua functions can be called from 
  C++ code. It allows one to specify custome behaviour for when e.g. Sprites collide.
*/

// Constructors
LuaCollisionAction::LuaCollisionAction(lua_State* aL)
  : L(aL)
{
  if (lua_isfunction(L, -1)) {
    iActionRef = luaL_ref(L, LUA_REGISTRYINDEX);    
  }
  else {
    iActionRef = LUA_NOREF;
    luaL_error(L, "Expected a function object but got a '%s'", lua_typename(L, lua_type(L,-1)));
  }
}

LuaCollisionAction::~LuaCollisionAction()
{
  luaL_unref(L, LUA_REGISTRYINDEX, iActionRef);
  // cout << hex << "0x" << (int)this << " LuaCollisionAction removed" << endl;  // DEBUG         
}

// Operations
bool LuaCollisionAction::execute(Shape* me_obj, Shape* other_obj, real t, real dt) 
{      
  Sprite* me = dynamic_cast<Sprite*>(me_obj);
  Sprite* other = dynamic_cast<Sprite*>(other_obj);
  
  lua_rawgeti(L, LUA_REGISTRYINDEX, iActionRef);
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
