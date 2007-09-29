/*!
	\class LuaRect2  LuaRect2.h
	\brief Brief description of class.
	\author Erik Engheim	
	\date 08/02/2007

	Detailed description of class.
*/

#include "LuaRect2.h"

#include <Lua/LuaUtils.h>
#include <Lua/Geometry/LuaVector2.h>
#include <Geometry/Rect2.hpp>
#include <Geometry/IO.hpp>


#include <iostream>

#include <lua.hpp>

void Rect2_push(lua_State *L, const Rect2& r)
{
  lua_newtable(L);
  Vector2_push(L, r.min());
  lua_setfield(L, -2, "min");
  Vector2_push(L, r.max());
  lua_setfield(L, -2, "max");  

  lua_getglobal(L, "Rect");
  lua_setmetatable(L, -2);
  lua_getglobal(L, "Rect");
  lua_setfield(L, -2, "__index");      
}

Rect2 Rect2_pull(lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TTABLE);  

  lua_getfield(L, index, "min");
  Vector2 min = Vector2_pull(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, index, "max");
  Vector2 max = Vector2_pull (L, -1);        
  lua_pop(L, 1);  

  return Rect2(min, max);  
}
  
static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, segment)", n);
}

// Functions exported to Lua
// Rect.new(x, y)
static int newRect2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5 && n != 3)
    return luaL_error(L, "Got %d arguments expected 3 (self, min, max) or 5 (self, left, bottom, right, top)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
  
  if (n == 3) {
    lua_pushvalue(L, 2);
    lua_setfield(L, -2, "min");
    lua_pushvalue(L, 3);  
    lua_setfield(L, -2, "max");     
  }
  else if (n == 5) {
    Vector2_push(L, Vector2(luaL_checknumber(L,2), luaL_checknumber(L,3)));
    lua_setfield(L, -2, "min");
    Vector2_push(L, Vector2(luaL_checknumber(L,4), luaL_checknumber(L,5)));
    lua_setfield(L, -2, "max");  
  }

  return 1; 
}

// Accessors
static int x_coord(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.x());
  return 1; 
}

static int y_coord(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.y());
  return 1; 
}

static int width(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.width());
  return 1; 
}

static int height(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.height());
  return 1; 
}


static int xmin(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.xmin());
  return 1; 
}

static int xmax(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.xmax());
  return 1; 
}

static int ymin(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.ymin());
  return 1; 
}

static int ymax(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  lua_pushnumber(L, s.ymax());
  return 1; 
}

static int bottomLeft(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  Vector2_push(L, s.bottomLeft());
  return 1; 
}

static int topRight(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  Vector2_push(L, s.topRight());
  return 1; 
}

static int bottomRight(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  Vector2_push(L, s.bottomRight());
  return 1; 
}

static int topLeft(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 s = Rect2_pull(L, 1);
  Vector2_push(L, s.topLeft());
  return 1; 
}

static int halfSize(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 
  Rect2 r = Rect2_pull(L, 1);
  Vector2 hsize = r.halfSize();
  Vector2_push(L, hsize);
  return 1; 
}

// Request
static int isNull(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);

  Rect2 r = Rect2_pull(L, 1);
  lua_pushboolean(L, r.isNull()); 
  return 1; 
}

static int isEmpty(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);

  Rect2 r = Rect2_pull(L, 1);
  lua_pushboolean(L, r.isEmpty()); 
  return 1; 
}

static int isValid(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (self)", n);

  Rect2 r = Rect2_pull(L, 1);
  lua_pushboolean(L, r.isValid()); 
  return 1; 
}

// Calculations

// Operators

// functions that will show up in our Lua environment
static const luaL_Reg gRectFuncs[] = {
  {"new", newRect2},
  // Accessors
  {"x", x_coord},
  {"y", y_coord},

  {"width", width},
  {"height", height},
                      
  {"xmin", xmin},                  
  {"ymin", ymin},                  
  {"xmax", xmax},                  
  {"ymax", ymax},                  

  {"bottomLeft", bottomLeft},                  
  {"topRight", topRight},                  
  {"bottomRight", bottomRight},                  
  {"topLeft", topLeft},                  

  {"halfSize", halfSize},                  

  // Request
  {"isNull", isNull},                  
  {"isEmpty", isEmpty},                  
  {"isValid", isValid},                  
  
  // Calculations
  // Operators

  {NULL, NULL}
};

// Initialization
void initLuaRect2(lua_State *L)
{    
  luaL_register(L, "Rect", gRectFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");  
}
