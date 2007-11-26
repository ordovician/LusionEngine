/*
 *  Action.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include <Core/SharedObject.hpp>

class Shape;

class Action : public SharedObject
{
public:    
  // Operations
  virtual bool execute(Shape* me, real start_time, real delta_time) = 0;
  
};
 
class CollisionAction : public SharedObject
{
public:    
  // Operations
  virtual bool execute(Shape* me, Shape* other, real start_time, real delta_time) = 0;
  
};

struct lua_State;

class LuaAction : public Action
{
public:
 // Constructors
 LuaAction(lua_State* aL);
 virtual ~LuaAction();

 // Operations
 bool execute(Shape* me, real start_time, real delta_time);
 
private:
  lua_State* L;
  int iActionRef;
};

class LuaCollisionAction : public CollisionAction
{
public:
 // Constructors
 LuaCollisionAction(lua_State* aL);
 virtual ~LuaCollisionAction();

 // Operations
 bool execute(Shape* me, Shape* other, real start_time, real delta_time);
 
private:
  lua_State* L;
  int iActionRef;
};

