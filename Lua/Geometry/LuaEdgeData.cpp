/*!
	\class LuaEdgeData  LuaEdgeData.h
	\brief Brief description of class.
	\author Erik Engheim	
	\date 08/02/2007

	Detailed description of class.
*/

#include "LuaEdgeData.h"

#include <Lua/LuaUtils.h>
#include <Lua/Geometry/LuaVector2.h>
#include <Lua/Geometry/LuaTrapezoid2.h>

#include <Geometry/Vector2.hpp>
#include <Geometry/IO.hpp>


#include <iostream>

#include <lua.hpp>

EdgePair EdgeData_pull(lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TTABLE);  // Make sure we got a table (a point is a table with pair of values)

  lua_getfield(L, index, "tag"); // Get first value from table and put on top of stack
  int tag = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  
  lua_getfield(L, index, "pos"); // Get second value from table and put on top of stack      
  Vector2 pos = Vector2_pull (L, -1);        
  lua_pop(L, 1);  
  
  lua_getfield(L, index, "u");
  Trapezoid2* u = checkTrapezoid2(L, -1); assert(u != 0);
  lua_pop(L, 1);

  lua_getfield(L, index, "v");
  Trapezoid2* v = checkTrapezoid2(L, -1); assert(v != 0);
  lua_pop(L, 1);
  
  return EdgePair(u, v, tag, pos);  
}
  
static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, segment)", n);
}

// Functions exported to Lua
// EdgeData.new(x, y)
static int newEdgeData(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5)
    return luaL_error(L, "Got %d arguments expected 5 (self, u_trapezoid, v_trapezoid, tag, pos)", n);
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);

  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "u");

  lua_pushvalue(L, 3);
  lua_setfield(L, -2, "v");

  lua_pushvalue(L, 4);
  lua_setfield(L, -2, "tag");

  lua_pushvalue(L, 5);  
  lua_setfield(L, -2, "pos"); 

  return 1; 
}


// Operators
static int equal(lua_State *L) 
{
  checkArguments(L);
  EdgePair e1 = EdgeData_pull(L, 1);
  EdgePair e2 = EdgeData_pull(L, 2); 
  lua_pushboolean(L, e1 == e2); 
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gEdgeDataFuncs[] = {
  {"new", newEdgeData},
        
  // Operators
  {"__eq", equal},      
  {NULL, NULL}
};

// Initialization
void initLuaEdgeData(lua_State *L)
{    
  luaL_register(L, "EdgeData", gEdgeDataFuncs);  
  lua_pushvalue(L,-1);
  lua_setfield(L, -2, "__index");  
}
