/*
 *  LuaShape.cpp
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
#include "Lua/Geometry/LuaSegment2.h"

#include "Base/Sprite.h"
#include "Base/ShapeGroup.h"
#include "Base/Group.h"
#include "Base/Command.h"

#include "Base/CircleShape.h"
#include "Base/RectShape2.h"
#include "Base/SegmentShape2.h"

#include <Geometry/Circle.hpp>

#include <iostream>
#include <iterator>

#include <lua.hpp>
#include <assert.h>

// Helper functions
Shape *checkShape(lua_State* L, int index)
{
  Shape* v;
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
    
  Shape **g = (Shape **)lua_newuserdata(L, sizeof(Shape *));
  
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

static int newCircleShape(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (class, center, radius)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 
  
  // Create a table and set its metatable to first argument of 'new' function
  // This way we can mimic inheritance  
  pushClassInstance(L);
  
  CircleShape **s = (CircleShape **)lua_newuserdata(L, sizeof(Sprite *));  
  
  real radius = luaL_checknumber (L, 3);      
  *s = new CircleShape(Circle(Vector2_pull(L,2), radius));    

  setUserDataMetatable(L, "Lusion.Shape");
  
  return 1; 
}

static int newRectShape2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (class, min, max)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 
  
  // Create a table and set its metatable to first argument of 'new' function
  // This way we can mimic inheritance  
  pushClassInstance(L);
  
  RectShape2 **s = (RectShape2 **)lua_newuserdata(L, sizeof(Sprite *));  
  
  *s = new RectShape2(Rect2(Vector2_pull(L,2), (Vector2_pull(L,3))));    

  setUserDataMetatable(L, "Lusion.Shape");
  
  return 1; 
}

static int newSegmentShape2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (class, min, max)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 
  
  // Create a table and set its metatable to first argument of 'new' function
  // This way we can mimic inheritance  
  pushClassInstance(L);
  
  SegmentShape2 **s = (SegmentShape2 **)lua_newuserdata(L, sizeof(Sprite *));  
  
  *s = new SegmentShape2(Segment2(Vector2_pull(L,2), (Vector2_pull(L,3))));    

  setUserDataMetatable(L, "Lusion.Shape");
  
  return 1; 
}
static int init(lua_State *L) 
{
  // int n = lua_gettop(L);  // Number of arguments
  // if (n != 1) 
  //   return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
  // Shape* g = checkShape(L);
  // cout << "Initializing group: " << (int)g << endl;
  return 0;
}

// Accessors
static int boundingBox(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  Shape* shape = checkShape(L); 
  assert(shape != 0);   
  Rect2_push(L, shape->boundingBox());
      
  return 1;
}

// Request
static int collide(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5 && n != 4)
    return luaL_error(L, "Got %d arguments expected 5 or 4 (self, shape, start_time, delta_time [, function])", n); 
    
  Shape* shape = checkShape(L,1); 
  Shape* other = checkShape(L,2);      
       
  assert(shape != 0 && other != 0);  
   
  real t = luaL_checknumber(L, 3);
  real dt = luaL_checknumber(L, 4);
  
  bool ret = false;
  if (n == 4)
    ret = shape->collide(other, t, dt);
  else if (n == 5) {
    lua_pushvalue(L,5);
    LuaCommand cmd(L);
    ret = shape->collide(other, t, dt, &cmd);
  }
    
  lua_pushboolean(L, ret);     
  
  return 1;
}

static int circleIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, center, radius)", n); 
    
  Shape* shape = checkShape(L);      
  assert(shape != 0);  
   
  bool ret = shape->intersect(Circle(Vector2_pull(L, 2), luaL_checknumber(L, 3)));    
  lua_pushboolean(L, ret);
  
  return 1;
}

static int rectIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, rect)", n); 
    
  Shape* shape = checkShape(L);      
  assert(shape != 0);  
   
  bool ret = shape->intersect(Rect2(Rect2_pull(L, 2)));    
  lua_pushboolean(L, ret);
  
  return 1;
}

static int segmentIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, rect)", n); 
    
  Shape* shape = checkShape(L);      
  assert(shape != 0);  
   
  bool ret = shape->intersect(Segment2(Segment2_pull(L, 2)));    
  lua_pushboolean(L, ret);
  
  return 1;
}

// Calculate
static int draw(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 3 (self, rect)", n); 
    
  Shape* shape = checkShape(L, 1);    

  shape->draw(Rect2_pull(L, 2));
  
  return 0;
}

// Operations
static int update(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, start_time, delta_time)", n); 
    
  Shape* shape = checkShape(L, 1);    

  shape->update(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  
  return 0;
}

// __gc
static int destroyShape(lua_State* L)
{
  Shape* shape = 0;
  checkUserData(L, "Lusion.Shape", shape);
  shape->release();
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyShapeFuncs[] = {
  {"__gc", destroyShape},  
  {NULL, NULL}  
};

static const luaL_Reg gShapeFuncs[] = {
  {"newGroup", newShapeGroup},
  {"newCircle", newCircleShape},  
  {"newRect", newRectShape2},  
  {"newSegment", newSegmentShape2},      
  {"init", init},

  // Accessors
  {"boundingBox", boundingBox},
    
  // Request
  {"collide", collide},
  {"circleIntersect", circleIntersect},
  {"rectIntersect", rectIntersect},
  {"segmentIntersect", segmentIntersect},
    
  // Calculate     
  {"draw", draw},
    
  // Operations  
  {"update", update},
  {NULL, NULL}
};

// Initialization
void initLuaShape(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Shape");
  luaL_register(L, 0, gDestroyShapeFuncs);      
  luaL_register(L, 0, gShapeFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Shape", gShapeFuncs);  
}