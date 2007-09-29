/*!
	\class LuaCircle  LuaCircle.h
	\brief Brief description of class.
	\author Erik Engheim	
	\date 08/02/2007

	Detailed description of class.
*/

#include "LuaCircle.h"

#include <Lua/LuaUtils.h>

#include <Lua/Geometry/LuaVector2.h>
#include <Lua/Geometry/LuaRect2.h>
#include <Lua/Geometry/LuaSegment2.h>

#include <Geometry/Vector2.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>

#include <Geometry/IO.hpp>


#include <iostream>

#include <lua.hpp>

void Circle_push(lua_State *L, const Circle& s)
{
  lua_newtable(L);
  Vector2_push(L, s.center());
  lua_setfield(L, -2, "center");
  
  lua_pushnumber(L, s.radius());
  lua_setfield(L, -2, "radius");  
    
  lua_getglobal(L, "Circle");
  lua_setmetatable(L, -2);
  lua_getglobal(L, "Circle");
  lua_setfield(L, -2, "__index");      
}

Circle Circle_pull(lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TTABLE);  // Make sure we got a table (a point is a table with pair of values)
  lua_getfield(L, index, "center"); // Get first value from table and put on top of stack
  Vector2 c = Vector2_pull(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, index, "radius"); // Get second value from table and put on top of stack      
  float r = lua_tonumber(L, -1);        
  lua_pop(L, 1);  
    
  return Circle(c,r);  
}
  
static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, circle)", n);
}

// Functions exported to Lua
static int newCircle(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 3 (self, center, radius)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "center");
  lua_pushvalue(L, 3);  
  lua_setfield(L, -2, "radius"); 

  return 1; 
}

// Request
static int inside(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, point)", n);
  Circle c = Circle_pull(L, 1);
  Vector2 v = Vector2_pull(L, 2);
  
  lua_pushboolean(L, c.inside(v));
  return 1; 
}

static int circleIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, circle)", n);
  Circle c1 = Circle_pull(L, 1);
  Circle c2 = Circle_pull(L, 2);
  
  lua_pushboolean(L, c1.intersect(c2));
  return 1; 
}

static int rectIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, rect)", n);
  Circle c = Circle_pull(L, 1);
  Rect2 r = Rect2_pull(L, 2);
  
  lua_pushboolean(L, c.intersect(r));
  return 1; 
}

static int segmentIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, segment)", n);
  Circle c = Circle_pull(L, 1);
  Segment2 s = Segment2_pull(L, 2);
  
  lua_pushboolean(L, c.intersect(s));
  return 1; 
}

// Operators
static int equal(lua_State *L) 
{
  checkArguments(L);
  Circle s = Circle_pull(L, 1);
  Circle t = Circle_pull(L, 2); 
  lua_pushboolean(L, s == t); 
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gCircleFuncs[] = {
  {"new", newCircle},

  // Request
  {"inside", inside},                        
  {"circleIntersect", circleIntersect},                        
  {"rectIntersect", rectIntersect},
  {"segmentIntersect", segmentIntersect},
                    
  // Operators
  {"__eq", equal},      
  {NULL, NULL}
};

// Initialization
void initLuaCircle(lua_State *L)
{    
  luaL_register(L, "Circle", gCircleFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");  
}
