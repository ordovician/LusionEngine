/*
 *  Command.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Command.h"
#include "Base/Sprite.h"

#include "Lua/LuaUtils.h"
#include "Lua/Base/LuaSprite.h"

#include <lua.hpp>

#include <iostream>

using namespace std;
/*!
   \class SpriteCommand Command.h
   \brief Used to mimic a function object for C++

   SpriteCommand is used through out the application to specify actions to happen when
   certain events happen, like e.g. collision between two sprites. 
*/

/*!
   \class LuaCommand Command.h
   \brief 

   Wrapper for a Lua function object. This is lua functions can be called from 
   C++ code. It allows one to specify custome behaviour for when e.g. Sprites collide.
*/

// Constructors
LuaCommand::LuaCommand(lua_State* aL)
  : L(aL)
{
  if (lua_isfunction(L, -1)) {
    iCommandRef = luaL_ref(L, LUA_REGISTRYINDEX);    
  }
  else {
    iCommandRef = LUA_NOREF;
    luaL_error(L, "Expected a function object but got a '%s'", lua_typename(L, lua_type(L,-1)));
  }
}

LuaCommand::~LuaCommand()
{
  luaL_unref(L, LUA_REGISTRYINDEX, iCommandRef);
  // cout << hex << "0x" << (int)this << " LuaCommand removed" << endl;  // DEBUG         
}

// Operations
bool LuaCommand::execute(Shape* me_obj, Shape* other_obj, real t, real dt) 
{      
  Sprite* me = dynamic_cast<Sprite*>(me_obj);
  Sprite* other = dynamic_cast<Sprite*>(other_obj);
  
  lua_rawgeti(L, LUA_REGISTRYINDEX, iCommandRef);
  if (me)
    retrieveSpriteTable(L, me);
  else
    lua_pushnil(L);
  if (other)
    retrieveSpriteTable(L, other);  
  else
    lua_pushnil(L);
  lua_pushnumber(L, t);
  lua_pushnumber(L, dt);  
  lua_call(L, 4, 1);     
  bool ret = lua_toboolean(L,-1);
  lua_pop(L,1);
  
  return ret;  
}
