/*
 *  LuaMotionState.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 13.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaMotionState.h"
#include "Base/MotionState.h"
#include "LuaEngine.h"
#include "Engine.h"
#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaVector2.h"

#include <lua.hpp>
#include <cassert>

#include <iostream>

// Helper functions
MotionState  *checkMotionState(lua_State* L, int index)
{
  MotionState* v;
  pullClassInstance(L, index, "Lusion.MotionState", v);
  return v;
}

/*!
  Not called directly by Lua but by other functions that need
  to create a 'MotionState' object.
  
  Assumes that another MotionState object is at 'index'.
  So the metatable of that object is set to the metatable of the new
  pushed object
*/
static MotionState* MotionState_create(lua_State *L, int index)
{
  lua_newtable(L);
  lua_getmetatable(L, index);       
  lua_setmetatable(L, -2);
    
  MotionState **s = (MotionState **)lua_newuserdata(L, sizeof(MotionState *));  
  *s = new MotionState; 
  setUserDataMetatable(L, "Lusion.MotionState");  
  return *s;
}

void MotionState_push(lua_State *L, MotionState* mstate)
{
  lua_newtable(L);
  lua_getglobal(L, "MotionState");
  lua_setmetatable(L, -2);
    
  MotionState **s = (MotionState **)lua_newuserdata(L, sizeof(MotionState *));  
  *s = mstate;
  mstate->retain(); 
  setUserDataMetatable(L, "Lusion.MotionState");  
}

// Functions exported to Lua
// MotionState.new(x,y,dir,speed) or MotionState.new(pos,dir,speed)
// pos = {x,y}
static int newMotionState(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5 && n != 1 && n != 4) 
    return luaL_error(L, "Got %d arguments expected 5,4 or 1", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 
  
  // Create a table and set its metatable to first argument of 'new' function
  // This way we can mimic inheritance  
  pushClassInstance(L);
  
  MotionState **s = (MotionState **)lua_newuserdata(L, sizeof(MotionState *));  
  
  if (n == 5) {
    real x = luaL_checknumber (L, 2);      
    real y = luaL_checknumber (L, 3); 
    real dir = luaL_checknumber (L, 4);      
    real speed = luaL_checknumber (L, 5); 
    *s = new MotionState(Point2(x, y), dir, speed);
  }
  else if (n == 4) {
    real dir = luaL_checknumber (L, 3);      
    real speed = luaL_checknumber (L, 4); 
    *s = new MotionState(Vector2_pull(L,2), dir, speed);    
  }  
  else
    *s = new MotionState; 
       
  setUserDataMetatable(L, "Lusion.MotionState");

  return 1; 
}

// __gc for MotionState
static int destroyMotionState(lua_State* L)
{
  MotionState* mstate = 0;
  checkUserData(L, "Lusion.MotionState", mstate);
  mstate->release();
  return 0;
}

// Accessors

// mstate:setPosition(x,y)
static int setPosition(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 3) {
    MotionState* mstate = checkMotionState(L);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    mstate->setPosition(Point2(x,y));
  }
  else if (n == 2) 
    checkMotionState(L)->setPosition(Vector2_pull(L, 2));    
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

// mstate:position()
static int position(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  MotionState* mstate = checkMotionState(L);
  Point2 p = mstate->position();
  pushTable(L, p);
  
  return 1;
}

// mstate:velocity()
static int velocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    Vector2 v = mstate->velocity();
    pushTable(L,v);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int speed(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    real x = mstate->speed();
    lua_pushnumber(L, x);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

// mstate:setVelocity(x,y)
static int setVelocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 3) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    mstate->setVelocity(Vector2(x,y));
  }
  else if (n == 2) 
    checkMotionState(L)->setVelocity(Vector2_pull(L, 2));      
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

static int setSpeed(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 2) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    real speed = luaL_checknumber (L, 2); 
    mstate->setSpeed(speed);
  }
  else
    luaL_error(L, "Got %d arguments expected 2 (self, speed)", n); 
  
  return 0;
}

static int setDirection(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 3) {
    MotionState* mstate = checkMotionState(L);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    mstate->setDirection(Vector2(x,y));
  }
  else if (n == 2) 
    checkMotionState(L)->setDirection(Vector2_pull(L, 2));        
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

// mstate:direction()
static int direction(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    pushTable(L, mstate->direction());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setRotation(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    MotionState* mstate = checkMotionState(L);
    real deg = luaL_checknumber (L, 2); 
    mstate->setRotation(deg);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int rotation(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    lua_pushnumber(L, mstate->rotation());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setAngularVelocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    MotionState* mstate = checkMotionState(L);
    real deg = luaL_checknumber (L, 2); 
    mstate->setAngularVelocity(deg);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int angularVelocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    lua_pushnumber(L, mstate->angularVelocity());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setAngularAcceleration(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    MotionState* mstate = checkMotionState(L);
    real deg = luaL_checknumber (L, 2); 
    mstate->setAngularAcceleration(deg);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int angularAcceleration(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    MotionState* mstate = checkMotionState(L);
    assert(mstate != 0);
    lua_pushnumber(L, mstate->angularAcceleration());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

// Calculations
static int copy(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
  MotionState* mstate = checkMotionState(L, 1);
  mstate = new MotionState(*mstate);
  MotionState_push(L, mstate);
  mstate->release();

  return 1;
}

// Operations
static int integrate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3", n); 
  MotionState* mstate = checkMotionState(L);

  mstate->integrate(luaL_checknumber(L,2), luaL_checkint(L,3));

  return 0;
}

static int integratePath(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3", n); 
  MotionState* mstate = checkMotionState(L);
  Points2 path;
  mstate->integratePath(luaL_checknumber(L,2), luaL_checkint(L,3), path);
  
  if (path.empty())
    lua_pushnil(L);
  else
    for_each(path.begin(), path.end(), PushValue<Point2>(L));
  return 1;
}

static int advance(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2", n); 
  MotionState* mstate = checkMotionState(L);
  mstate->advance(luaL_checknumber(L,2));

  return 0;
}

static int move(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3 || n != 2) 
    return luaL_error(L, "Got %d arguments expected 3", n); 
  MotionState* mstate = checkMotionState(L);

  if (n == 2)
    mstate->move(Vector2_pull(L, 2));        
  else {
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    mstate->move(Vector2(x,y));    
  }

  return 0;
}

static int accelerate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    MotionState* mstate = checkMotionState(L);    
    assert(mstate != 0);
    real a = luaL_checknumber (L, 2); 
    mstate->accelerate(a);
  }
  else
    luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int rotate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments

  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 or 3", n); 

  MotionState* mstate = checkMotionState(L);    
  assert(mstate != 0);
  real angle = luaL_checknumber (L, 2); 
  mstate->rotate(angle);

  return 0;
}

static int reverse(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
  MotionState* mstate = checkMotionState(L);
  mstate->reverse();

  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyMotionStateFuncs[] = {
  // Destruction
  {"__gc", destroyMotionState},
  {NULL, NULL}
};

static const luaL_Reg gMotionStateFuncs[] = {
  // Creation
  {"new", newMotionState},

  // Accessors
  {"position", position},
  {"setPosition", setPosition},  
  {"setVelocity", setVelocity},
  {"velocity", velocity},    
  {"setSpeed", setSpeed},      
  {"speed", speed},      
  {"direction", direction},
  {"setDirection", setDirection},  
  {"setRotation", setRotation},  
  {"rotation", rotation},  
  {"setAngularVelocity", setAngularVelocity},  
  {"angularVelocity", angularVelocity},  
  {"setAngularAcceleration", setAngularAcceleration},  
  {"angularAcceleration", angularAcceleration},  
       
  // Calculations
  {"copy", copy},  

  
  // Operations
  {"integrate", integrate},      
  {"integratePath", integratePath},        
  {"advance", advance},          
  {"move", move},    
  {"accelerate", accelerate},      
  {"rotate", rotate},  
  {"reverse", reverse},    
  
  {NULL, NULL}
};

// Initialization
void initLuaMotionState(lua_State *L)
{  
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.MotionState");
  luaL_register(L, 0, gDestroyMotionStateFuncs);      
  luaL_register(L, 0, gMotionStateFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  
  
  luaL_register(L, "MotionState", gMotionStateFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");    
}