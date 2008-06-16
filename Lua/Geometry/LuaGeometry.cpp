/*
 *  LuaGeometry.cpp
 *  LusionGeometry
 *
 *  Created by Erik Engheim on 3.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaGeometry.h"

#include "Lua/LuaUtils.h"

#include <Geometry/Vector2.hpp>
#include <Utils/PolygonUtils.h>
#include <Lua/Geometry/LuaVector2.h>

#include <Geometry/Polygon2.hpp>


#include <lua.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

// Functions exported to Lua
static int inside(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
   return luaL_error(L, "Got %d arguments expected 2 (polygon, query_point)", n); 

  Polygon2 poly;
  getPolygon(L, 1, poly);
  Vector2 q = Vector2_pull(L, 2);
  lua_pushboolean(L, inside(poly.begin(), poly.end(), q));  
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gGeometryFuncs[] = {
  {"inside", inside},  
  {NULL, NULL}
};


// Initialization
void initLuaGeometry(lua_State *L)
{
  luaL_register(L, "Geometry", gGeometryFuncs);    
}
