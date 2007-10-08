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
#include "Base/CollisionGroup.h"
#include "Base/Command.h"

#include "Engine.h"

#include <iostream>

#include <lua.hpp>
#include <assert.h>

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

  setUserDataMetatable(L, "Lusion.Group");

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
  // Group* g = checkGroup(L);
  // cout << "Initializing group: " << (int)g << endl;
  return 0;
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

static int sprites(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Group* group = checkGroup(L);      
  ShallowSpriteSet sprites = group->sprites();  
  for_each(sprites.first, sprites.second, PushUserData<Sprite*>(L,1));
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


// Operations
// group:add(sprite)
static int add(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2", n); 
    
  Group* group = checkGroup(L, 1);    
  Sprite* sprite = checkSprite(L, 2);
  group->add(sprite);
  
  // Store sprite in group table using its address as key
  lua_pushlightuserdata(L, sprite);
  lua_pushvalue(L,2);
  lua_settable(L,1);
  
  return 0;
}

static int remove(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2", n); 
    
  Group* group = checkGroup(L, 1);    
  Sprite* sprite = checkSprite(L, 2);
  group->remove(sprite);

  // Remove the sprite from group table by setting it to nil
  lua_pushlightuserdata(L, sprite);
  lua_pushnil(L);
  lua_settable(L,1);

  return 0;
}

static int update(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, start_time, delta_time)", n); 
    
  Group* group = checkGroup(L);      
  assert(group != 0);
  
  group->update(luaL_checknumber(L,2), luaL_checknumber(L,3));
  
  return 0;
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

static int render(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Group* group = checkGroup(L);    
  group->draw(worldView());
  return 0;
}

// clear helper
static int clear(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
      
  Group* group = checkGroup(L);   
  ShallowSpriteSet sprites = group->sprites();
  
  // Remove all sprites from group table by setting them to nil
  SpriteSet::const_iterator sprite;
  for (sprite = sprites.first; sprite != sprites.second; ++sprite) {
    lua_pushlightuserdata(L,*sprite);
    lua_pushnil(L);
    lua_settable(L, 1);
  }
  group->clear();
  
  return 0;
}

static int nextSprite(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
      
  Group* group = checkGroup(L);   
  Sprite* sprite = group->nextSprite();

  if (sprite == 0)  
    lua_pushnil(L);
  else {
    lua_pushlightuserdata(L, sprite);
    lua_gettable(L, 1);    
  }
  return 1;
}

static int collide(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 4 && n != 5) 
    return luaL_error(L, "Got %d arguments expected 4 or 5 (self, collision_obj, start_time, delta_time [,function])", n); 
    
  Group* me = checkGroup(L, 1);    
  CollisionGroup* other = checkCollisionGroup(L, 2);

  assert(me != 0);
  assert(other != 0);
  
  real t = luaL_checknumber(L, 3);
  real dt = luaL_checknumber(L, 4);

  bool ret = false;
  
  if (n == 4) {
    ret = me->collide(other, t, dt);
  }
  else if (n == 5) {
    lua_pushvalue(L, 5);
    LuaCommand cmd(L);
    ret = me->collide(other, t, dt, &cmd);
  }

  lua_pushboolean(L, ret);
  return 1;
}

// __gc
static int destroyGroup(lua_State* L)
{
  Group* group = 0;
  checkUserData(L, "Lusion.Group", group);
  group->release();
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyGroupFuncs[] = {
  {"__gc", destroyGroup},  
  {NULL, NULL}  
};

static const luaL_Reg gGroupFuncs[] = {
  {"new", newGroup},
  {"init", init},

  // Accessors
  {"size", size},
  {"sprites", sprites},
  
  // Request
  {"contains", contains},
  
  // Operations
  {"private_add", add},
  {"private_remove", remove},
  {"update", update},
  {"doPlanning", doPlanning},
  {"render", render},
  {"clear", clear},
  {"nextSprite", nextSprite},  
  {"collide", collide},      
  {NULL, NULL}
};

// Initialization
void initLuaGroup(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Group");
  luaL_register(L, 0, gDestroyGroupFuncs);      
  luaL_register(L, 0, gGroupFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Group", gGroupFuncs);  
}