/*
 *  LuaGroup.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Base/LuaGroup.h"
#include "Lua/Base/LuaCollisionGroup.h"
#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaMotionState.h"

#include "Base/Sprite.h"
#include "Base/Group.h"
#include "Base/ShapeGroup.h"
#include "Base/Command.h"

#include "Engine.h"

#include <iostream>

#include <lua.hpp>
#include <cassert>

// Helper functions

// Functions exported to Lua
// Group.new(points)
// E.g. v = Group.new({{1,2}, {1,1}, {4,4}})
static int newGroup(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Wrong number of arguments in Group constructor. Should be 0"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Group **g = (Group **)lua_newuserdata(L, sizeof(Group *));
  *g = new Group;

  setUserDataMetatable(L, "Lusion.Shape");

  // Handle user initialization
  lua_getfield(L, 1, "init"); 
  lua_pushvalue(L, -2);     // Our new instance should be lying on stack right below function 'init'
  lua_call(L, 1, 0);     

  return 1; 
}

// Accessors
static int size(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Group* group = checkGroup(L);    
  lua_pushnumber(L, group->size());
      
  return 1;
}

// Request
static int contains(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2", n); 
    
  Group* group = checkGroup(L, 1);    
  Sprite* sprite = checkSprite(L, 2);
  
  lua_pushboolean(L, group->contains(sprite));    
  return 1;
}



static int doPlanning(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, start_time, delta_time)", n); 
    
  Group* group = checkGroup(L);      
  assert(group != 0);
  
  group->doPlanning(luaL_checknumber(L,2), luaL_checknumber(L,3));
  
  return 0;  
}

// clear helper
static int clear(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
      
  Group* group = checkGroup(L);   

  // Remove all sprites from group table by setting them to nil  
  ShapeIterator* it = group->shapeIterator();
  for (it->first(); !it->done(); it->next()) {
    lua_pushlightuserdata(L, it->value());
    lua_pushnil(L);
    lua_settable(L, 1);  
  }  
  group->clear();
  
  return 0;
}

static int nextShape(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
      
  Group* group = checkGroup(L);   
  Shape* shape = group->nextShape();

  if (shape == 0)  
    lua_pushnil(L);
  else {
    lua_pushlightuserdata(L, shape);
    lua_gettable(L, 1);    
  }
  return 1;
}

// functions that will show up in our Lua environment
static const luaL_Reg gGroupClassFuncs[] = {
  {"new", newGroup},
  {NULL, NULL}
};

static const luaL_Reg gGroupFuncs[] = {
  // Accessors
  {"size", size},
//  {"sprites", sprites}, TODO: reintroduce later
  
  // Request
  {"contains", contains},
  
  // Operations
  {"doPlanning", doPlanning},
  {"clear", clear},
  {"nextShape", nextShape},  
  {NULL, NULL}
};

// Initialization
void initLuaGroup(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Shape");
  luaL_register(L, 0, gGroupFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Group", gGroupClassFuncs);  
}