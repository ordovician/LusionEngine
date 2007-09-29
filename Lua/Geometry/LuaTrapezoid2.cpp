/*
 *  LuaTrapezoid2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaTrapezoid2.h"
#include "Lua/LuaUtils.h"

#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"

#include <Geometry/Trapezoid2.hpp>

#include <iostream>

#include <lua.hpp>
#include <assert.h>

// Helper functions
void Trapezoid2_push(lua_State *L, Trapezoid2* t)
{
  if (t != 0) {
    lua_newtable(L);
    lua_pushlightuserdata(L, t);
    lua_setfield(L, -2, "__self");

    lua_getglobal(L, "Trapezoid");
    lua_setmetatable(L, -2);
    lua_getglobal(L, "Trapezoid");
    lua_setfield(L, -2, "__index");          
  }
  else {
    lua_pushnil(L);
  }
}

Trapezoid2* Trapezoid2_pull(lua_State *L, int index)
{
  // TODO: Add check for metatable so we don't make type mistakes
  luaL_checktype(L, index, LUA_TTABLE);  
  lua_getfield(L, index, "__self");
  Trapezoid2* t = (Trapezoid2*)lua_touserdata(L, -1);
  lua_pop(L, 1);  
  return t;  
}

Trapezoid2 *checkTrapezoid2(lua_State* L, int index)
{
  // Trapezoid2* v;
  // pullClassInstance(L, index, "Lusion.Trapezoid2", v);
  // return v;
  return Trapezoid2_pull(L, index);
}

void pushTable(lua_State* L, Trapezoid2* t)
{
  Trapezoid2_push(L, t);
}

// Functions exported to Lua
// NOTE: We need to define checkTrapezoid2 to use pullClassInstance() if we create
// trapezoids with this function
static int newTrapezoid2(lua_State *L) 
{  
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5)
    return luaL_error(L, "Wrong number of arguments in Trapezoid2 constructor. Should be 5"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Trapezoid2 **g = (Trapezoid2 **)lua_newuserdata(L, sizeof(Trapezoid2 *));
  *g = new Trapezoid2(Vector2_pull(L,2), Vector2_pull(L,3), Segment2_pull(L,4), Segment2_pull(L,5));

  setUserDataMetatable(L, "Lusion.Trapezoid2");

  return 1; 
}

// Accessors
static int setNeighbors(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5) 
    return luaL_error(L, "Got %d arguments expected 5 (self, ll, ul, lr, ur)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L, 1); assert(trap != 0);      
  Trapezoid2* ll = checkTrapezoid2(L, 2);
  Trapezoid2* ul = checkTrapezoid2(L, 3);
  Trapezoid2* lr = checkTrapezoid2(L, 4);
  Trapezoid2* ur = checkTrapezoid2(L, 5);
        
  trap->setNeighbors(ll, ul, lr, ur);
  
  return 0;  
}

static int setLeftNeighbors(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, ll, ul)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L, 1); assert(trap != 0);      
  Trapezoid2* ll = checkTrapezoid2(L, 2);
  Trapezoid2* ul = checkTrapezoid2(L, 3);
        
  trap->setLeftNeighbors(ll, ul);
  
  return 0;  
}

static int setRightNeighbors(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5) 
    return luaL_error(L, "Got %d arguments expected 5 (self, lr, ur)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L, 1); assert(trap != 0);      
  Trapezoid2* lr = checkTrapezoid2(L, 2);
  Trapezoid2* ur = checkTrapezoid2(L, 3);
        
  trap->setRightNeighbors(lr, ur);
  
  return 0;  
}

static int setLeftNeighbor(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, left)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L, 1); assert(trap != 0);      
  Trapezoid2* left = checkTrapezoid2(L, 2);
        
  trap->setLeftNeighbor(left);
  
  return 0;  
}

static int setRightNeighbor(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, right)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L, 1); assert(trap != 0);      
  Trapezoid2* right = checkTrapezoid2(L, 2);
        
  trap->setRightNeighbor(right);
  
  return 0;  
}

static int left(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->left());
      
  return 1;
}

static int right(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->right());
      
  return 1;
}

static int bottom(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->bottom());
      
  return 1;
}

static int top(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->top());
      
  return 1;
}

static int center(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->center());
      
  return 1;
}

static int centerLeft(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->centerLeft());
      
  return 1;
}

static int centerRight(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->centerRight());
      
  return 1;
}



static int lowerLeft(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->lowerLeft());
      
  return 1;  
}

static int upperLeft(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->upperLeft());
      
  return 1;  
}

static int lowerRight(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);    
  pushTable(L, trap->lowerRight());
      
  return 1;  
}

static int upperRight(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);      
  pushTable(L, trap->upperRight());
      
  return 1;  
}

static int neighbors(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);      
  Trapezoids2 ts = trap->neighbors();
  
  for_each(ts.begin(), ts.end(), PushValue<Trapezoid2*>(L));
      
  return 1;  
}

static int leftNeighbors(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);      
  Trapezoids2 ts = trap->leftNeighbors();
  
  for_each(ts.begin(), ts.end(), PushValue<Trapezoid2*>(L));
      
  return 1;  
}

static int rightNeighbors(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);      
  Trapezoids2 ts = trap->rightNeighbors();
  
  for_each(ts.begin(), ts.end(), PushValue<Trapezoid2*>(L));
      
  return 1;  
}

static int setNeighbor(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, index, trapezoid)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);      
  trap->setNeighbor(luaL_checkinteger(L, 2), checkTrapezoid2(L, 3));
  
  return 0;  
}

static int neighbor(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, index)", n); 
    
  Trapezoid2* trap = checkTrapezoid2(L); assert(trap != 0);      
  pushTable(L, trap->neighbor(luaL_checkinteger(L, 2)));
      
  return 1;  
}

static int id(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Trapezoid2* trap = checkTrapezoid2(L);
  assert(trap != 0);  
  lua_pushlightuserdata(L, trap);
  return 1;
}

// Request
// __eq
static int compareTrapezoids(lua_State* L)
{
  Trapezoid2* v1 = checkTrapezoid2(L,1); assert( v1 != 0 );
  Trapezoid2* v2 = checkTrapezoid2(L,2); assert( v2 != 0 ); 
  lua_pushboolean(L, v1 == v2);
  return 1;
}

static int isBordering(lua_State* L)
{
  Trapezoid2* v1 = checkTrapezoid2(L,1); assert( v1 != 0 );
  Trapezoid2* v2 = checkTrapezoid2(L,2); assert( v2 != 0 ); 
  lua_pushboolean(L, v1->isBordering(v2));
  return 1;
}



// Calculations
static int distance(lua_State* L)
{
  Trapezoid2* v1 = checkTrapezoid2(L,1); assert( v1 != 0 );
  Trapezoid2* v2 = checkTrapezoid2(L,2); assert( v2 != 0 ); 
  lua_pushnumber(L, v1->distance(v2));
  return 1;
}

// Debug
static int tag(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Trapezoid2* trap = checkTrapezoid2(L);
  assert(trap != 0);  
  lua_pushnumber(L, trap->tag());
  
  return 1;
}

// __gc
static int destroyTrapezoid2(lua_State* L)
{
  Trapezoid2* trap = 0;
  checkUserData(L, "Lusion.Trapezoid2", trap);
  delete trap;
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyTrapezoid2Funcs[] = {
  {"__gc", destroyTrapezoid2},  
  {NULL, NULL}  
};

static const luaL_Reg gTrapezoid2Funcs[] = {
  {"new", newTrapezoid2},
  // Accessors
  {"left", left},
  {"right", right},
  {"bottom", bottom},
  {"top", top},
  {"center", center},  
  {"centerLeft", centerLeft},  
  {"centerRight", centerRight},      
  {"lowerLeft", lowerLeft},  
  {"upperLeft", upperLeft},  
  {"lowerRight", lowerRight},  
  {"upperRight", upperRight},      
  {"neighbors", neighbors},      
  {"leftNeighbors", leftNeighbors},        
  {"rightNeighbors", rightNeighbors},          
  {"setNeighbor", setNeighbor},          
  {"neighbor", neighbor},            
  {"id", id},

  // Request
  {"__eq", compareTrapezoids},  
  {"isBordering", isBordering},        
  
  // Calculations
  {"distance", distance},        
  
  // Debug
  {"tag", tag},        
  
  {NULL, NULL}
};

// Initialization
void initLuaTrapezoid2(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Trapezoid2");
  luaL_register(L, 0, gDestroyTrapezoid2Funcs);      
  luaL_register(L, 0, gTrapezoid2Funcs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Trapezoid", gTrapezoid2Funcs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");    
}