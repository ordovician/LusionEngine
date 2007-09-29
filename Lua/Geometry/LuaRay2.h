/******************************************************************
Name   : LuaRay2
Author : Erik Engheim
Date   : 08/02/2007
Desc   : 
Comment: Not tested yet
*******************************************************************/

#pragma once

struct lua_State;

#include <Geometry/Ray2.hpp>

void initLuaRay2(lua_State *L);

void  Ray2_push(lua_State *L, const Ray2& v);
Ray2  Ray2_pull(lua_State *L, int index);