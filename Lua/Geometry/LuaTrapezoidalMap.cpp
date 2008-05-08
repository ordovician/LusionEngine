/*
 *  LuaTrapezoidalMap.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaTrapezoidalMap.h"
#include "Lua/LuaUtils.h"

#include "Geometry/Trapezoid2.hpp"
#include "Geometry/TrapezoidalMap2.hpp"

#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaRect2.h"
#include "Lua/Geometry/LuaTrapezoid2.h"

#include <iostream>

#include <lua.hpp>
#include <cassert>

// Helper functions
TrapezoidalMap2 *checkTrapezoidalMap(lua_State* L, int index=1)
{
  TrapezoidalMap2* v;
  pullClassInstance(L, index, "Lusion.TrapezoidalMap", v);
  return v;
}

// Functions exported to Lua
static int newTrapezoidalMap(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2 && n != 3)
   return luaL_error(L, "Got %d arguments expected 2 or 3 (class, segments [,boundingbox])", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 
  Segments2 segs;
  getSegments(L, 2, segs);

  pushClassInstance(L);

  TrapezoidalMap2 **g = (TrapezoidalMap2 **)lua_newuserdata(L, sizeof(TrapezoidalMap2 *));

  if (n == 2)
    *g = new TrapezoidalMap2(segs.begin(), segs.end());
  else if (n == 3)  {
    *g = new TrapezoidalMap2(segs.begin(), segs.end(), Rect2_pull(L, 3));    
  }
  setUserDataMetatable(L, "Lusion.TrapezoidalMap");
  
  return 1; 
}

static int locate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, point)", n);     
  TrapezoidalMap2* tmap = checkTrapezoidalMap(L);
  assert(tmap != 0);
  
  Trapezoid2 *t = tmap->locate(Vector2_pull(L, 2));  
  Trapezoid2_push(L, t);
  // lua_pushlightuserdata(L, t);
  
  return 1;
}

static int calcBoundingBox(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (segments)", n);     

  Segments2 segs;
  getSegments(L, 1, segs);
  pushTable(L, calcBoundingBox(segs.begin(), segs.end()));
  
  return 1;
}

static int trapezoids(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);     
  
  TrapezoidalMap2* tmap = checkTrapezoidalMap(L);
  assert(tmap != 0);
  
  Trapezoids2 ts;
  tmap->getTrapezoids(ts);
  
  for_each(ts.begin(), ts.end(), PushValue<Trapezoid2*>(L));
  
  return 1;
}

// Operations
static int remove(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, trapezoid)", n);     
  
  TrapezoidalMap2* tmap = checkTrapezoidalMap(L);
  assert(tmap != 0);
  
  Trapezoid2 *trap = checkTrapezoid2(L, 2);
  assert(trap != 0);
  tmap->remove(trap);

  return 0;
}

static int assignUniqueTags(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
    
  TrapezoidalMap2* tmap = checkTrapezoidalMap(L);
  assert(tmap != 0);
  
  lua_pushnumber(L, tmap->assignUniqueTags());
  
  return 1;
}
// __gc
static int destroyTrapezoidalMap(lua_State* L)
{
  TrapezoidalMap2* tmap = 0;
  checkUserData(L, "Lusion.TrapezoidalMap", tmap);
  // cout << "Releasing trapezoidal map: " << (int)tmap << endl;
  delete tmap;
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyTrapezoidalMapFuncs[] = {
  {"__gc", destroyTrapezoidalMap},  
  {NULL, NULL}  
};

static const luaL_Reg gTrapezoidalMapFuncs[] = {
  {"new", newTrapezoidalMap},
  // Calculations
  {"locate", locate},
  {"calcBoundingBox", calcBoundingBox},   
  {"trapezoids", trapezoids},       
  // Operations
  {"remove", remove},         
  {"assignUniqueTags", assignUniqueTags},           
  {NULL, NULL}
};

// Initialization
void initLuaTrapezoidalMap(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.TrapezoidalMap");
  luaL_register(L, 0, gDestroyTrapezoidalMapFuncs);      
  luaL_register(L, 0, gTrapezoidalMapFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "TrapezoidalMap", gTrapezoidalMapFuncs);  
}