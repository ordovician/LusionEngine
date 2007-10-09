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

class Shape;
class Sprite;
 
class SpriteCommand : public SharedObject
{
public:    
  // Operations
  virtual bool execute(Shape* me, Shape* other, real start_time, real delta_time) = 0;
  
};

struct lua_State;

class LuaCommand : public SpriteCommand
{
public:
 // Constructors
 LuaCommand(lua_State* aL);
 virtual ~LuaCommand();

 // Operations
 bool execute(Shape* me, Shape* other, real start_time, real delta_time);
 
private:
  lua_State* L;
  int iCommandRef;
};

