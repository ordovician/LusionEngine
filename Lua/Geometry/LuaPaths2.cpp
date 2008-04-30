/*
 *  LuaPaths2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaPaths2.h"
#include "Lua/LuaUtils.h"

#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"

#include <Geometry/Graph2.hpp>

#include <Lua/Geometry/LuaTrapezoid2.h>


#include <iostream>

#include <lua.hpp>
#include <assert.h>

// Helper functions
/*! 
 We don't create paths specifically, so this is the only way to make a path. There is no 
 'new' class method on 'Paths'. Instead Graph:shortestPaths() will call this function to create
 a gc paths object.
*/
void Paths2_push(lua_State *L, Paths2* paths)
{
  if (paths != 0) {
    lua_newtable(L);
    lua_getglobal(L, "Paths");
    lua_setmetatable(L, -2);
    lua_getglobal(L, "Paths");
    lua_setfield(L, -2, "__index");  
       
    Paths2 **p = (Paths2 **)lua_newuserdata(L, sizeof(Paths2 *));
    *p = paths;
        
    setUserDataMetatable(L, "Lusion.Paths2");
  }
  else {
    lua_pushnil(L);
  }
}

Paths2 *checkPaths2(lua_State* L, int index)
{
  Paths2* v;
  pullClassInstance(L, index, "Lusion.Paths2", v);
  return v;
}

void pushTable(lua_State* L, Paths2* t)
{
  Paths2_push(L, t);
}

// Functions exported to Lua
// Accessors
static int pathFrom(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, trapezoid)", n); 
    
  Paths2* paths = checkPaths2(L); assert(paths != 0);      
  Trapezoid2* trap = checkTrapezoid2(L,2); assert(trap != 0);
  
  Points2 poly;
  paths->pathFrom(trap, poly);
  for_each(poly.begin(), poly.end(), PushValue<Point2>(L));
  
  return 1;  
}

static int distanceFrom(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, trapezoid)", n); 
    
  Paths2* paths = checkPaths2(L); assert(paths != 0);      
  Trapezoid2* trap = checkTrapezoid2(L,2); assert(trap != 0);
  
  real d = paths->distanceFrom(trap);
  lua_pushnumber(L, d);
  
  return 1;  
}

static int printGraph(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  Paths2* paths = checkPaths2(L); assert(paths != 0);      
  
  paths->printGraph();
  
  return 0;  
}

static int printPathFrom(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, trapezoid)", n); 
    
  Paths2* paths = checkPaths2(L); assert(paths != 0);      
  Trapezoid2* trap = checkTrapezoid2(L,2);
  paths->printPathFrom(trap);
  
  return 0;  
}

// __gc
static int destroyPaths2(lua_State* L)
{
  Paths2* paths = 0;
  checkUserData(L, "Lusion.Paths2", paths);
  delete paths;
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyPaths2Funcs[] = {
  {"__gc", destroyPaths2},  
  {NULL, NULL}  
};

static const luaL_Reg gPaths2Funcs[] = {
  // Accessors
  {"pathFrom", pathFrom},  
  {"distanceFrom", distanceFrom},    
  {"printPathFrom", printPathFrom},      
  {"printGraph", printGraph},      
  {NULL, NULL}
};

// Initialization
void initLuaPaths2(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Paths2");
  luaL_register(L, 0, gDestroyPaths2Funcs);      
  luaL_register(L, 0, gPaths2Funcs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Paths", gPaths2Funcs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");    
}