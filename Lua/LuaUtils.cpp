/*
 *  LuaUtils.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 14.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/LuaUtils.h"

#include "Base/Shape.h"
#include "Base/Sprite.h"
#include "Base/View.h"
#include "Base/Group.h"

#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"
#include "Lua/Geometry/LuaRect2.h"

#include <assert.h>

using namespace std;

void setUserDataMetatable(lua_State *L, const char* tablename)
{
  luaL_getmetatable(L, tablename);
  lua_setmetatable(L, -2);       
  lua_setfield(L, -2, "__self");  
}

/*! Pops user data of the stack and checks that it is of correct type */
void *popUserData (lua_State *L, int argnum, const char *tname) {
  void *p = lua_touserdata(L, -1);
  if (p != 0) {  /* value is a userdata? */
   if (lua_getmetatable(L, -1)) {  /* does it have a metatable? */
     lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
     if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
       lua_pop(L, 3);  /* remove both metatables  and user data*/
       return p;
     }
   }
  }
  luaL_typerror(L, argnum, tname);  /* else error */
  return 0;  /* to avoid warnings */
}
 
Sprite *checkSprite(lua_State* L, int index)
{
  Sprite* v;
  pullClassInstance(L, index, "Lusion.Shape", v);
  return v;
}

Group *checkGroup(lua_State* L, int index)
{
  Group* v;
  pullClassInstance(L, index, "Lusion.Shape", v);
  return v;
}

View *checkView(lua_State* L, int index)
{
  View* v;
  pullClassInstance(L, index, "Lusion.View", v);
  return v;
}

SegmentView *checkSegmentView(lua_State* L, int index)
{
  SegmentView* v;
  pullClassInstance(L, index, "Lusion.View", v);
  return v;
}
 
/*! 
  Gets polygon from array of points stored at index t in 
  lua stack. We assume array is a table of the form
  {{x = 1, y = 2}, {x = 3, y = 4}}. Polygon is returned in p. 
*/
void getPolygon(lua_State* L, int t, Points2& p) 
{
  luaL_checktype(L, t, LUA_TTABLE); // Make sure we get a table with points as first argument
  lua_pushnil(L); // first key (ready traversal of table)
  while (lua_next(L, t) != 0) { 
    // ‘key’ is at index -2 and ‘value’ at index -1 
    p.push_back(Vector2_pull(L,-1));
    lua_pop(L, 1); // removes ‘value’; keeps ‘key’ for next iteration   
  }
}

void getSegments(lua_State* L, int t, Segments2& s) 
{
  luaL_checktype(L, t, LUA_TTABLE); // Make sure we get a table with points as first argument
  lua_pushnil(L); // first key (ready traversal of table)
  while (lua_next(L, t) != 0) { 
    // ‘key’ is at index -2 and ‘value’ at index -1 
    s.push_back(Segment2_pull(L,-1));
    lua_pop(L, 1); // removes ‘value’; keeps ‘key’ for next iteration   
  }
}

void getStrings(lua_State* L, int t, StringList& s) 
{
  luaL_checktype(L, t, LUA_TTABLE); // Make sure we get a table with points as first argument
  lua_pushnil(L); // first key (ready traversal of table)
  while (lua_next(L, t) != 0) { 
    // ‘key’ is at index -2 and ‘value’ at index -1 
    s.push_back(string(luaL_checkstring(L,-1)));
    lua_pop(L, 1); // removes ‘value’; keeps ‘key’ for next iteration   
  }
}

/*! 
 push a new table on the stack which is meant to inherit the table 
 at position 1 on stack. This typically argument 1 one the call to the
 'new' function which creates an instance of a class. 
 
 This function makes sure that the new table has the meta table set to 
 the table given as first argument to new. If new is called like this:
 class:new(), then 1 argument will be class.
*/
void pushClassInstance(lua_State* L)
{
  lua_newtable(L);
  lua_pushvalue(L,1);       
  lua_setmetatable(L, -2);
  lua_pushvalue(L,1);
  lua_setfield(L, 1, "__index");  
}

/*!
  Push a table representing a point onto the top of the stack
  This table will have coordinates of the point at keys 'x' and 'y'.
*/
void pushTable(lua_State* L, const Point2& p)
{
  Vector2_push(L,p);
}

void pushTable(lua_State* L, const Segment2& s)
{  
  Segment2_push(L, s);
}

void pushTable(lua_State* L, const Rect2& r)
{  
  Rect2_push(L, r);
}

void pushTable(lua_State* L, real num)
{
  lua_pushnumber(L, num);      
}

void newTable(lua_State* L, const char* metaname)
{
  lua_newtable(L);
  luaL_getmetatable(L, metaname); // metatable is now at index -1 and userdata at -2
  lua_setmetatable(L, -2);  // pops the metatable from the top and sets it on userdata    
}

bool checkBool(lua_State* L, int index)
{
 luaL_argcheck(L, lua_isboolean(L,index), 1, "`Boolean' expected");
 return lua_toboolean(L, index);
}

/*! 
  Register a relationship between lua value on top of stack and pointer \a ptr.
  Relation is registered under table with name \a tablename
*/
void registerPointer(lua_State* L, void* ptr,  const char* tablename)
{
  lua_getfield(L, LUA_REGISTRYINDEX, tablename);
  lua_pushlightuserdata(L, ptr);  // key
  lua_pushvalue(L, -3);           // value
  lua_settable(L, -3);
  lua_pop(L,1);
}

/*!
  Get lua table that was registered for pointer \a ptr in table \a tablename.
  The table is put on top of stack.
*/
void retrievePointer(lua_State* L, void* ptr, const char* tablename)
{
  lua_getfield(L, LUA_REGISTRYINDEX, tablename);
  lua_pushlightuserdata(L, ptr);  // key
  lua_gettable(L, -2);
  lua_remove(L, -2);
}