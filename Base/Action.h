/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
  virtual bool execute(Shape* me, Shape* other, Points2& points, real start_time, real delta_time) = 0;
  
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
 bool execute(Shape* me, Shape* other, Points2& points, real start_time, real delta_time);
 
private:
  lua_State* L;
  int iActionRef;
};

