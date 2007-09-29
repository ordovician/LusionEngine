/******************************************************************
Name   : LuaVector2
Author : Erik Engheim
Date   : 08/02/2007
Desc   : 
Comment: Not tested yet
*******************************************************************/

#pragma once

struct lua_State;

#include <Geometry/Vector2.hpp>

void initLuaVector2(lua_State *L);

void    Vector2_push(lua_State *L, const Vector2& v);
Vector2 Vector2_pull(lua_State *L, int index);