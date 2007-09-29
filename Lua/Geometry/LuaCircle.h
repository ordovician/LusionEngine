/******************************************************************
Name   : LuaCircle
Author : Erik Engheim
Date   : 08/02/2007
Desc   : 
Comment: Not tested yet
*******************************************************************/

#pragma once

struct lua_State;

#include <Geometry/Circle.hpp>

void initLuaCircle(lua_State *L);

void    Circle_push(lua_State *L, const Circle& v);
Circle  Circle_pull(lua_State *L, int index);