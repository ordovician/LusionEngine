/*!
	\class LuaSegment2  LuaSegment2.h
	\brief Brief description of class.
	\author Erik Engheim	
	\date 08/02/2007

	Detailed description of class.
*/

#include "LuaSegment2.h"

#include <Lua/LuaUtils.h>
#include <Lua/Geometry/LuaVector2.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/IO.hpp>


#include <iostream>

#include <lua.hpp>

void Segment2_push(lua_State *L, const Segment2& s)
{
  lua_newtable(L);
  Vector2_push(L, s.source());
  lua_setfield(L, -2, "source");
  Vector2_push(L, s.target());
  lua_setfield(L, -2, "target");  
  
  lua_pushlightuserdata(L, s.data());
  lua_setfield(L, -2, "data");
  
  lua_getglobal(L, "Segment");
  lua_setmetatable(L, -2);
  lua_getglobal(L, "Segment");
  lua_setfield(L, -2, "__index");      
}

Segment2 Segment2_pull(lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TTABLE);  // Make sure we got a table (a point is a table with pair of values)
  lua_getfield(L, index, "source"); // Get first value from table and put on top of stack
  Vector2 s = Vector2_pull(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, index, "target"); // Get second value from table and put on top of stack      
  Vector2 t = Vector2_pull (L, -1);        
  lua_pop(L, 1);  
  
  lua_getfield(L, index, "data");
  void *data = lua_touserdata(L, -1);
  lua_pop(L, 1);
  
  return Segment2(s,t, data);  
}
  
static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, segment)", n);
}

// Functions exported to Lua
// Segment.new(x, y)
static int newSegment2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3 && n != 4)
    return luaL_error(L, "Got %d arguments expected 3 (self, source, target)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "source");
  lua_pushvalue(L, 3);  
  lua_setfield(L, -2, "target"); 

  if (n == 4)
    lua_pushvalue(L, 4);
  else
    lua_pushlightuserdata(L, 0); // Set  user data to 0/NULL

  lua_setfield(L, -2, "data");

  return 1; 
}

// Accessors
static int left(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  Segment2 s = Segment2_pull(L, 1);
  Vector2_push(L, s.left());
  return 1; 
}

static int right(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  Segment2 s = Segment2_pull(L, 1);
  Vector2_push(L, s.right());
  return 1; 
}

static int vertex(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, index)", n);
  Segment2 s = Segment2_pull(L, 1);
  int index = luaL_checkinteger(L,2);
  Vector2_push(L, s.vertex(index));
  return 1; 
}

static int tag(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  Segment2 s = Segment2_pull(L, 1);
  lua_pushnumber(L, s.tag());
  return 1; 
}

// Request
static int isDegenerate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);

  Segment2 s = Segment2_pull(L, 1);
  lua_pushboolean(L, s.isDegenerate()); 
  return 1; 
}

static int above(lua_State *L) 
{
  checkArguments(L);
  Segment2 s = Segment2_pull(L, 1);
  Vector2 v = Vector2_pull(L, 2); 
  lua_pushboolean(L, s.isAbove(v)); 
  return 1; 
}

static int below(lua_State *L) 
{
  checkArguments(L);
  Segment2 s = Segment2_pull(L, 1);
  Vector2 v = Vector2_pull(L, 2); 
  lua_pushboolean(L, s.isBelow(v)); 
  return 1; 
}

static int on(lua_State *L) 
{
  checkArguments(L);
  Segment2 s = Segment2_pull(L, 1);
  Vector2 v = Vector2_pull(L, 2); 
  lua_pushboolean(L, s.isOn(v)); 
  return 1; 
}

// Calculations
static int opposite(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  Segment2 s = Segment2_pull(L, 1);
  Segment2_push(L, s.opposite());
  return 1; 
}

static int toVector(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  Segment2 s = Segment2_pull(L, 1);
  Vector2_push(L, s.toVector());
  return 1; 
}

static int intersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, segment)", n);
  Segment2 s1 = Segment2_pull(L, 1);
  Segment2 s2 = Segment2_pull(L, 2);
  
  Vector2 v;
  if (s1.intersection(s2, v)) {
    Vector2_push(L, v);    
  }  
  else
    lua_pushnil(L);
  return 1; 
}

static int nearestPoint(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, point)", n);
  Segment2 seg   = Segment2_pull(L, 1);
  Point2 point = Vector2_pull(L, 2);
  
  Vector2_push(L, seg.nearestPoint(point));    
  return 1; 
}

// Operators
static int equal(lua_State *L) 
{
  checkArguments(L);
  Segment2 s = Segment2_pull(L, 1);
  Segment2 t = Segment2_pull(L, 2); 
  lua_pushboolean(L, s == t); 
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gSegmentFuncs[] = {
  {"new", newSegment2},

  // Accessors
  {"left", left},                  
  {"right", right},                  
  {"vertex", vertex},                      
  {"tag", tag},                        

  // Request
  {"isDegenerate", isDegenerate},                        
  {"above", above},                        
  {"below", below},                        
  {"on", on},                            

  // Calculations
  {"opposite", opposite},              
  {"toVector", toVector},                
  {"intersect", intersect},  
  {"nearestPoint", nearestPoint},    
                  
  // Operators
  {"__eq", equal},      
  {NULL, NULL}
};

// Initialization
void initLuaSegment2(lua_State *L)
{    
  luaL_register(L, "Segment", gSegmentFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");  
}
