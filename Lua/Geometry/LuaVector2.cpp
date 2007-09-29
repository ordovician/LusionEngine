/*!
	\class LuaVector2  LuaVector2.h
	\brief Brief description of class.
	\author Erik Engheim	
	\date 08/02/2007

	Detailed description of class.
*/

#include "LuaVector2.h"

#include <Lua/LuaUtils.h>
#include <Geometry/IO.hpp>

#include <iostream>

#include <lua.hpp>

void Vector2_push(lua_State *L, const Vector2& p)
{
  lua_newtable(L);            // a point structure
  lua_pushstring(L, "x");     // value first and then key
  lua_pushnumber(L, p.x());    
  lua_settable(L, -3);        // value and key on top of stack so table is on 3rd slot below top
  lua_pushstring(L, "y");
  lua_pushnumber(L, p.y());             
  lua_settable(L, -3);    

  lua_getglobal(L, "Vector");
  lua_setmetatable(L, -2);
  lua_getglobal(L, "Vector");
  lua_setfield(L, -2, "__index");      
}

Vector2 Vector2_pull(lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TTABLE);  // Make sure we got a table (a point is a table with pair of values)
  lua_getfield(L, index, "x"); // Get first value from table and put on top of stack
  real x = luaL_checknumber (L, -1);
  lua_pop(L, 1);
  lua_getfield(L, index, "y"); // Get second value from table and put on top of stack      
  real y = luaL_checknumber (L, -1);        
  lua_pop(L, 1);  
  return Vector2(x,y);  
}
  
static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, vector)", n);
}

// Functions exported to Lua
// Vector.new(x, y)
static int newVector2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 3 (self, x, y)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "x");
  lua_pushvalue(L, 3);  
  lua_setfield(L, -2, "y"); 

  return 1; 
}

static int unit(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Vector2 v = Vector2_pull(L, 1);
  Vector2_push(L, v.unit());
  return 1; 
}

static int length(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Vector2 v = Vector2_pull(L, 1);
  lua_pushnumber(L, v.length());
  return 1; 
}

static int squaredLength(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Vector2 v = Vector2_pull(L, 1);
  lua_pushnumber(L, v.squaredLength());
  return 1; 
}

static int add(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  Vector2_push(L, v+u); 
  return 1; 
}

static int sub(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  Vector2_push(L, v-u); 
  return 1; 
}

static int multiply(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  if (lua_isnumber(L,2)) {
    Vector2_push(L, v*lua_tonumber(L,2));
  }
  else {
    Vector2 u = Vector2_pull(L, 2); 
    lua_pushnumber(L, v.dot(u));     
  }
  return 1; 
}

static int cross(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  lua_pushnumber(L, v.cross(u)); 
  return 1; 
}


static int equal(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  lua_pushboolean(L, v == u); 
  return 1; 
}

static int lessThan(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  lua_pushboolean(L, v.isMin(u)); 
  return 1; 
}

static int lessOrEqual(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  lua_pushboolean(L, v.isMin(u) || v == u); 
  return 1; 
}

static int greaterThan(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  lua_pushboolean(L, v.isMax(u)); 
  return 1; 
}

static int greaterOrEqual(lua_State *L) 
{
  checkArguments(L);
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2); 
  lua_pushboolean(L, v.isMax(u) || v == u); 
  return 1; 
}

static int unaryMinus(lua_State *L) 
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L,"Got %d arguments expected 1 (self)", n);
    
  Vector2 v = Vector2_pull(L, 1);
  Vector2 u = Vector2_pull(L, 2);  

  Vector2_push(L, -v);
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gVectorFuncs[] = {
  {"new", newVector2},
  // Calculations
  {"__add", add},
  {"__sub", sub},  
  {"__eq", equal},    
 {"__lt", lessThan},      
 {"__le", lessOrEqual},        
 {"__gt", greaterThan},      
 {"__ge", greaterOrEqual},      
 {"__unm", unaryMinus},       
  {"__mul", multiply},          
  {"cross", cross},            
  {"unit", unit},              
  {"length", length},                
  {"squaredLength", squaredLength},                  
  {NULL, NULL}
};

// Initialization
void initLuaVector2(lua_State *L)
{    
  luaL_register(L, "Vector", gVectorFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");  
}
