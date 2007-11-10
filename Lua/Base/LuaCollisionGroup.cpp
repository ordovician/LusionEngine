/*
 *  LuaShapeGroup.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Base/LuaCollisionGroup.h"
#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaMotionState.h"
#include "Lua/Geometry/LuaRect2.h"
#include "Lua/Geometry/LuaVector2.h"

#include "Base/Sprite.h"
#include "Base/ShapeGroup.h"
#include "Base/Group.h"
#include "Base/Command.h"

#include <Geometry/Circle.hpp>

#include <iostream>
#include <iterator>

#include <lua.hpp>
#include <assert.h>

// Helper functions
ShapeGroup *checkShapeGroup(lua_State* L, int index)
{
  ShapeGroup* v;
  pullClassInstance(L, index, "Lusion.Shape", v);
  return v;
}

// Functions exported to Lua
static int newShapeGroup(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (class, group)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  ShapeGroup **g = (ShapeGroup **)lua_newuserdata(L, sizeof(ShapeGroup *));
  
  Group* group = checkGroup(L,2);
  assert(group != 0);
  *g = new ShapeGroup(group->shapeIterator());

  setUserDataMetatable(L, "Lusion.Shape");

  // Handle user initialization
  lua_getfield(L, 1, "init"); 
  lua_pushvalue(L, -2);     // Our new instance should be lying on stack right below function 'init'
  lua_call(L, 1, 0);     

  return 1; 
}

static int init(lua_State *L) 
{
  // int n = lua_gettop(L);  // Number of arguments
  // if (n != 1) 
  //   return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
  // ShapeGroup* g = checkShapeGroup(L);
  // cout << "Initializing group: " << (int)g << endl;
  return 0;
}

// Accessors
static int boundingBox(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  ShapeGroup* group = checkShapeGroup(L); 
  assert(group != 0);   
  Rect2_push(L, group->boundingBox());
      
  return 1;
}

// Request
static int circleIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, center, radius)", n); 
    
  ShapeGroup* group = checkShapeGroup(L);      
  assert(group != 0);  
   
  bool ret = group->intersect(Circle(Vector2_pull(L, 2), luaL_checknumber(L, 3)));    
  lua_pushboolean(L, ret);
  
  return 1;
}

static int rectIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, rect)", n); 
    
  ShapeGroup* group = checkShapeGroup(L);      
  assert(group != 0);  
   
  bool ret = group->intersect(Rect2(Rect2_pull(L, 2)));    
  lua_pushboolean(L, ret);
  
  return 1;
}

// Operations
static int update(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, start_time, delta_time)", n); 
    
  ShapeGroup* group = checkShapeGroup(L, 1);    

  group->update(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  
  return 0;
}

// __gc
static int destroyShapeGroup(lua_State* L)
{
  ShapeGroup* group = 0;
  checkUserData(L, "Lusion.Shape", group);
  group->release();
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyShapeGroupFuncs[] = {
  {"__gc", destroyShapeGroup},  
  {NULL, NULL}  
};

static const luaL_Reg gShapeGroupFuncs[] = {
  {"new", newShapeGroup},
  {"init", init},

  // Accessors
  {"boundingBox", boundingBox},
    
  // Request
  {"collide", circleIntersect},
  {"rectIntersect", rectIntersect},
    
  // Operations  
  {"update", update},
  {NULL, NULL}
};

// Initialization
void initLuaShapeGroup(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Shape");
  luaL_register(L, 0, gDestroyShapeGroupFuncs);      
  luaL_register(L, 0, gShapeGroupFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "ShapeGroup", gShapeGroupFuncs);  
}