/*
 *  LuaUtils.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 14.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include <lua.hpp>
#include <functional>
#include <string>

#include "Types.h"

class Shape;
class Sprite;
class View;
class SegmentView;
class Group;

void setUserDataMetatable(lua_State *L, const char* tablename);
void *popUserData (lua_State *L, int argnum, const char *tname);

template<class T>
void checkUserData(lua_State* L, const char* classname, T*& data)
{
  void *ud = luaL_checkudata(L, 1, classname);
  std::string msg = std::string("`") + std::string(classname) + std::string("' expected");
  luaL_argcheck(L, ud != 0, 1, msg.c_str());
  data = *((T**)ud);      
}

template<class T>
void pullClassInstance(lua_State* L, int index, const char* classname, T*& data)
{
  void* ud = 0;
  luaL_checktype(L, index, LUA_TTABLE); 
  lua_getfield(L, index, "__self");
  ud = popUserData(L, index, classname);
  std::string msg = std::string("`") + std::string(classname) + std::string("' expected");
  luaL_argcheck(L, ud != 0, index, msg.c_str());  
  
  data = *((T**)ud);      
}

Sprite  *checkSprite(lua_State* L, int index = 1);
Group   *checkGroup(lua_State* L, int index = 1);
View    *checkView(lua_State* L, int index = 1);
SegmentView *checkSegmentView(lua_State* L, int index = 1);

/*!
  This function object assumes that at index 'aIndex' in the lua stack
  'aL' we have a table which contains key-value pairs where the value
  is a table that wraps a C/C++ object. They key is the address of the C++
  object in memory. Thus given the address of a C++ object this functor can
  locate the corresponding lua table.
  
  When the functor is created it will create a table 't' on the stack. Each time
  the functor is called with the address of a C++ object 'c', it will add the coresponding
  lua table 'l' of 'c' to 't'. 
  
  Example of usage. Assume we have an array 'a' of user type T in C++. We want to turn this
  into an array of corresponding lua tables. We assume that a mapping of C++ addreses to lua tables
  is at index 'i': 
  
  // Create an array of lua object from 'a' on the stack
  for_each(a.begin(), a.end(), PushUserData<T*>(L, i));
*/
template<class T>
struct PushUserData : public std::unary_function<T, void>
{
  PushUserData(lua_State *aL, int aIndex) 
    : L(aL), index(aIndex), n(1) 
  {
    lua_newtable(L);     
  }
  
  void operator()(T ud) {
    lua_pushlightuserdata(L, ud);
    lua_gettable(L, index);
    lua_rawseti(L, -2, n++);        
  }
  
  int n, index;
  lua_State *L;  
};

/*! Use this with for_each to push an array of data of type T onto stack. */
template<class T>
struct PushValue : public std::unary_function<T, void>
{
  PushValue(lua_State *aL) 
    : L(aL), n(1) 
  {
    lua_newtable(L);     
  }
  
  void operator()(T v) {
    pushTable(L,v);
    lua_rawseti(L, -2, n++);  // Set index n in array to point table we just created  
  }
  
  int n;
  lua_State *L;  
};

void  getPolygon(lua_State* L, int t, Points2& p);
void  getSegments(lua_State* L, int t, Segments2& s);
void  getStrings(lua_State* L, int t, StringList& s);
void  pushClassInstance(lua_State* L);
void  pushTable(lua_State* L, const Point2& p);
void  pushTable(lua_State* L, real num);
void  pushTable(lua_State* L, const Segment2& p);
void  pushTable(lua_State* L, const Rect2& p);
void  newTable(lua_State* L, const char* metaname);

Vector2   checkVector2(lua_State* L, int index);
Segment2  checkSegment2(lua_State* L, int index);
bool      checkBool(lua_State* L, int index);

void registerPointer(lua_State* L, void* ptr,  const char* tablename);
void retrievePointer(lua_State* L, void* ptr, const char* tablename);
