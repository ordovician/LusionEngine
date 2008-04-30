/*!
	\class LuaRay2  LuaRay2.h
	\brief Brief description of class.
	\author Erik Engheim	
	\date 08/02/2007

	Detailed description of class.
*/

#include "LuaRay2.h"
#include "LuaSegment2.h"

#include <Lua/LuaUtils.h>
#include <Lua/Geometry/LuaVector2.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/IO.hpp>


#include <iostream>

#include <lua.hpp>

void Ray2_push(lua_State *L, const Ray2& r)
{
  lua_newtable(L);
  Vector2_push(L, r.origin());
  lua_setfield(L, -2, "origin");
  Vector2_push(L, r.direction());
  lua_setfield(L, -2, "direction");  
    
  lua_getglobal(L, "Ray");
  lua_setmetatable(L, -2);
  lua_getglobal(L, "Ray");
  lua_setfield(L, -2, "__index");      
}

Ray2 Ray2_pull(lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TTABLE); 
  lua_getfield(L, index, "origin"); 
  Vector2 o = Vector2_pull(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, index, "direction");
  Vector2 d = Vector2_pull (L, -1);        
  lua_pop(L, 1);  
    
  return Ray2(o,d);  
}
  
static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, ray)", n);
}

// Functions exported to Lua
// Ray.new(x, y)
static int newRay2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 3 (self, origin, direction)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "origin");
  lua_pushvalue(L, 3);  
  lua_setfield(L, -2, "direction"); 

  return 1; 
}

// Accessors
static int segmentIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, ray)", n);
  Ray2 r = Ray2_pull(L, 1);
  Segment2 s = Segment2_pull(L, 2);
  
  Vector2 v;
  if (r.intersection(s, v)) {
    Vector2_push(L, v);    
  }  
  else
    lua_pushnil(L);
  return 1; 
}

static int polygonIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, ray)", n);
  Ray2 r = Ray2_pull(L, 1);
  
  Points2 poly;
  getPolygon(L, 2, poly);
  
  Vector2 v;
  if (r.intersection(poly.begin(), poly.end(), v)) {
    Vector2_push(L, v);    
  }  
  else
    lua_pushnil(L);
  return 1; 
}

// Operators
static int equal(lua_State *L) 
{
  checkArguments(L);
  Ray2 s = Ray2_pull(L, 1);
  Ray2 t = Ray2_pull(L, 2); 
  lua_pushboolean(L, s == t); 
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gRayFuncs[] = {
  {"new", newRay2},
  // Calculations               
  {"segmentIntersect", segmentIntersect},                  
  {"polygonIntersect", polygonIntersect},  
                    
  // Operators
  {"__eq", equal},      
  {NULL, NULL}
};

// Initialization
void initLuaRay2(lua_State *L)
{    
  luaL_register(L, "Ray", gRayFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");  
}
