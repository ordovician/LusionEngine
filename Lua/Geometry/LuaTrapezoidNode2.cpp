/*
 *  LuaTrapezoidNode2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaTrapezoidNode2.h"
#include "Lua/Geometry/LuaTrapezoid2.h"
#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"

#include "Geometry/TrapezoidNode2.hpp"

#include <iostream>

#include <lua.hpp>
#include <cassert>

// Helper functions
TrapezoidNode2 *checkTrapezoidNode2(lua_State* L, int index)
{
 TrapezoidNode2* v;
 pullClassInstance(L, index, "Lusion.TrapezoidNode2", v);
 return v;
}


// Functions exported to Lua
// Constructors
static int newSegmentNode2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Wrong number of arguments in TrapezoidNode2 constructor. Should be 0"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  TrapezoidNode2 **g = (TrapezoidNode2 **)lua_newuserdata(L, sizeof(TrapezoidNode2 *));
  *g = newNode(Segment2_pull(L,2));

  setUserDataMetatable(L, "Lusion.TrapezoidNode2");

  return 1; 
}

static int newPointNode2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Wrong number of arguments in TrapezoidNode2 constructor. Should be 0"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  TrapezoidNode2 **g = (TrapezoidNode2 **)lua_newuserdata(L, sizeof(TrapezoidNode2 *));
  *g = newNode(Vector2_pull(L,2));

  setUserDataMetatable(L, "Lusion.TrapezoidNode2");

  return 1; 
}

static int newLeafNode2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Wrong number of arguments in TrapezoidNode2 constructor. Should be 0"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  TrapezoidNode2 **g = (TrapezoidNode2 **)lua_newuserdata(L, sizeof(TrapezoidNode2 *));
  *g = newNode(Trapezoid2_pull(L,2));

  setUserDataMetatable(L, "Lusion.TrapezoidNode2");

  return 1; 
}

static int newHeadNode2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Wrong number of arguments in TrapezoidNode2 constructor. Should be 0"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  TrapezoidNode2 **g = (TrapezoidNode2 **)lua_newuserdata(L, sizeof(TrapezoidNode2 *));
  *g = newNode(checkTrapezoidNode2(L,2));

  setUserDataMetatable(L, "Lusion.TrapezoidNode2");

  return 1; 
}


// __gc
static int destroyTrapezoidNode2(lua_State* L)
{
  TrapezoidNode2* node = 0;
  checkUserData(L, "Lusion.TrapezoidNode2", node);
  delete node;
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyTrapezoidNode2Funcs[] = {
  {"__gc", destroyTrapezoidNode2},  
  {NULL, NULL}  
};

static const luaL_Reg gTrapezoidNode2Funcs[] = {
  {"newSegmentNode", newSegmentNode2},
  {"newPointNode", newPointNode2},  
  {"newLeaf", newLeafNode2},
  {"newHead", newHeadNode2},        
  // Accessors
  {NULL, NULL}
};

// Initialization
void initLuaTrapezoidNode2(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.TrapezoidNode2");
  luaL_register(L, 0, gDestroyTrapezoidNode2Funcs);      
  luaL_register(L, 0, gTrapezoidNode2Funcs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "TrapezoidNode2", gTrapezoidNode2Funcs);  
}