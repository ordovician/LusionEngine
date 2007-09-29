/*
 *  LuaCollisionGroup.cpp
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
#include "Base/CollisionGroup.h"
#include "Base/Group.h"
#include "Base/Command.h"

#include <Geometry/Circle.hpp>

#include <iostream>
#include <iterator>

#include <lua.hpp>
#include <assert.h>

// Helper functions
CollisionGroup *checkCollisionGroup(lua_State* L, int index)
{
  CollisionGroup* v;
  pullClassInstance(L, index, "Lusion.CollisionGroup", v);
  return v;
}

// Functions exported to Lua
static int newCollisionGroup(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (class, group)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  CollisionGroup **g = (CollisionGroup **)lua_newuserdata(L, sizeof(CollisionGroup *));
  
  Group* group = checkGroup(L,2);
  assert(group != 0);
  ShallowSpriteSet sprites = group->sprites();
  CollisionObjects objs(distance(sprites.first, sprites.second));
  copy(sprites.first, sprites.second, objs.begin());
  *g = new CollisionGroup(objs.begin(), objs.end());

  setUserDataMetatable(L, "Lusion.CollisionGroup");

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
  // CollisionGroup* g = checkCollisionGroup(L);
  // cout << "Initializing group: " << (int)g << endl;
  return 0;
}

// Accessors
static int boundingBox(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  CollisionGroup* group = checkCollisionGroup(L); 
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
    
  CollisionGroup* group = checkCollisionGroup(L);      
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
    
  CollisionGroup* group = checkCollisionGroup(L);      
  assert(group != 0);  
   
  bool ret = group->intersect(Rect2(Rect2_pull(L, 2)));    
  lua_pushboolean(L, ret);
  
  return 1;
}

// Calculations
static int traverse(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 4) 
    return luaL_error(L, "Got %d arguments expected 4 (self, start_time, delta_time function)", n); 
    
  CollisionGroup* group = checkCollisionGroup(L);      
  assert(group != 0);  
   
  real t = luaL_checknumber(L, 2);
  real dt = luaL_checknumber(L, 3);  
  
  bool ret = false;
  lua_pushvalue(L, 4);
  LuaCommand cmd(L);
  ret = group->traverse(t, dt, &cmd);
    
  lua_pushboolean(L, ret);
  
  return 1;
}

static int boundingBoxes(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  CollisionGroup* group = checkCollisionGroup(L);      
  assert(group != 0);  

  struct BoundingBoxesCommand : public SpriteCommand {
    bool execute(CollisionObject* me, CollisionObject*, real, real) {
      boxes.push_back(me->boundingBox());
      return true;      
    }
    vector<Rect2> boxes;
  } cmd;

  group->traverse(0.0, 0.0, &cmd);
  
  for_each(cmd.boxes.begin(), cmd.boxes.end(), PushValue<Rect2>(L));
  
  return 1;
}

// Operations
static int update(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, start_time, delta_time)", n); 
    
  CollisionGroup* group = checkCollisionGroup(L, 1);    

  group->update(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  
  return 0;
}

// __gc
static int destroyCollisionGroup(lua_State* L)
{
  CollisionGroup* group = 0;
  checkUserData(L, "Lusion.CollisionGroup", group);
  group->release();
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyCollisionGroupFuncs[] = {
  {"__gc", destroyCollisionGroup},  
  {NULL, NULL}  
};

static const luaL_Reg gCollisionGroupFuncs[] = {
  {"new", newCollisionGroup},
  {"init", init},

  // Accessors
  {"boundingBox", boundingBox},
    
  // Request
  {"circleIntersect", circleIntersect},
  {"rectIntersect", rectIntersect},
  
  // Calculations
  {"traverse", traverse},
  {"boundingBoxes", boundingBoxes},
  
  // Operations  
  {"update", update},
  {NULL, NULL}
};

// Initialization
void initLuaCollisionGroup(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.CollisionGroup");
  luaL_register(L, 0, gDestroyCollisionGroupFuncs);      
  luaL_register(L, 0, gCollisionGroupFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "CollisionGroup", gCollisionGroupFuncs);  
}