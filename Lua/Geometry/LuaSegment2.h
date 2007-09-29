/******************************************************************
Name   : LuaSegment2
Author : Erik Engheim
Date   : 08/02/2007
Desc   : 
Comment: Not tested yet
*******************************************************************/

#pragma once

struct lua_State;

#include <Geometry/Segment2.hpp>

void initLuaSegment2(lua_State *L);

void    Segment2_push(lua_State *L, const Segment2& v);
Segment2 Segment2_pull(lua_State *L, int index);