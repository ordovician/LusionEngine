/*
 *  LuaShapeGroup.cpp
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
#include "Base/ShapeGroup.h"
#include "Base/Group.h"
#include "Base/Command.h"

#include <Geometry/Circle.hpp>

#include <iostream>
#include <iterator>

#include <lua.hpp>
#include <assert.h>

// Helper functions
ShapeGroup *checkShapeGroup(lua_State* L, int index)
{
  ShapeGroup* v;
  pullClassInstance(L, index, "Lusion.Shape", v);
  return v;
}

// Functions exported to Lua
static int newShapeGroup(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (class, group)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  ShapeGroup **g = (ShapeGroup **)lua_newuserdata(L, sizeof(ShapeGroup *));
  
  Group* group = checkGroup(L,2);
  assert(group != 0);
  *g = new ShapeGroup(group->shapeIterator());

  setUserDataMetatable(L, "Lusion.Shape");

  // Handle user initialization
  lua_getfield(L, 1, "init"); 
  lua_pushvalue(L, -2);     // Our new instance should be lying on stack right below function 'init'
  lua_call(L, 1, 0);     

  return 1; 
}

static const luaL_Reg gShapeGroupFuncs[] = {
  {"new", newShapeGroup},
  {NULL, NULL}
};

// Initialization
void initLuaShapeGroup(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Shape");
  luaL_register(L, 0, gShapeGroupFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "ShapeGroup", gShapeGroupFuncs);  
}