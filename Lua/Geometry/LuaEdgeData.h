/******************************************************************
Name   : LuaEdgeData
Author : Erik Engheim
Date   : 08/02/2007
Desc   : 
Comment: Not tested yet
*******************************************************************/

#pragma once

struct lua_State;

#include <Geometry/Graph2.hpp>

void initLuaEdgeData(lua_State *L);

EdgePair EdgeData_pull(lua_State *L, int index);