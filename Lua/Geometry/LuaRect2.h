/******************************************************************
Name   : LuaRect2
Author : Erik Engheim
Date   : 08/02/2007
Desc   : 
Comment: Not tested yet
*******************************************************************/

#pragma once

struct lua_State;

#include <Geometry/Rect2.hpp>

void initLuaRect2(lua_State *L);

void  Rect2_push(lua_State *L, const Rect2& v);
Rect2 Rect2_pull(lua_State *L, int index);