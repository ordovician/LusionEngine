/*
 *  LuaShape.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Base/LuaShape.h"

#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaMotionState.h"
#include "Lua/Geometry/LuaRect2.h"
#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"

#include "Base/Sprite.h"
#include "Base/ShapeGroup.h"
#include "Timing.h"
#include "Base/Group.h"
#include "Base/Action.h"

#include "Base/CircleShape.h"
#include "Base/RectShape2.h"
#include "Base/SegmentShape2.h"

#include "Engine.h"

#include <Geometry/Circle.hpp>

#include <iostream>
#include <iterator>

#include <lua.hpp>
#include <assert.h>

// Helper functions
/*!
 Create a table to hold mapping between lua tables and pointers. The table has weak
 references so that lua tables are garbage collected even if there is one reference
 left to table in the this mapping table.
*/
void initShapeBookkeeping(lua_State* L)
{
  luaL_newmetatable(L, "Lusion.Shapes"); // Make new table in registry to hold mapping between sprite pointers and tables
  
  lua_pushstring(L, "v");                 // Make table weak so that sprites can be garbage collected
  lua_setfield(L, -2, "__mode");
  
  lua_pushvalue(L,-1);                    // Let the table be its own metatable
  lua_setmetatable(L, -2);    
}

void registerShapeTable(lua_State* L, Shape* shape)
{
  registerPointer(L, shape, "Lusion.Shapes");
}

void retrieveShapeTable(lua_State* L, Shape* shape)
{
  retrievePointer(L, shape, "Lusion.Shapes");  
}

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

  registerShapeTable(L, *g);

  // Handle user initialization
  lua_getfield(L, 1, "init"); 
  lua_pushvalue(L, -2);     // Our new instance should be lying on stack right below function 'init'
  lua_call(L, 1, 0);     

  return 1; 
}

static int newGroup(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Wrong number of arguments in Group constructor. Should be 0"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Group **g = (Group **)lua_newuserdata(L, sizeof(Group *));
  *g = new Group;

  setUserDataMetatable(L, "Lusion.Shape");

  registerShapeTable(L, *g);

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
  *s = createCircle(Circle(Vector2_pull(L,2), radius));    

  setUserDataMetatable(L, "Lusion.Shape");
  registerShapeTable(L, *s);
  
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
  
  *s = createRect2(Rect2(Vector2_pull(L,2), (Vector2_pull(L,3))));    

  setUserDataMetatable(L, "Lusion.Shape");
  registerShapeTable(L, *s);
  
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
  
  *s = createSegment2(Segment2(Vector2_pull(L,2), (Vector2_pull(L,3))));    

  setUserDataMetatable(L, "Lusion.Shape");
  registerShapeTable(L, *s);
  
  return 1; 
}
static int init(lua_State *L) 
{
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

static int center(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  CircleShape* shape = dynamic_cast<CircleShape*>(checkShape(L)); 

  // This upcast can in theory fail even if we programmed correctl
  // so we don't do an assert. User might not know that center is not
  // allowed for other shapes except by reading docs.
  if (shape == 0)
    return luaL_error(L, "Shape is not of type circle and method 'center' is thus not supported");    
  Vector2_push(L, shape->center());
      
  return 1;
}

static int radius(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
    
  CircleShape* shape = dynamic_cast<CircleShape*>(checkShape(L)); 

  // This upcast can in theory fail even if we programmed correctl
  // so we don't do an assert. User might not know that center is not
  // allowed for other shapes except by reading docs.
  if (shape == 0)
    return luaL_error(L, "Shape is not of type circle and method 'radius' is thus not supported");    
  lua_pushnumber(L, shape->radius());
      
  return 1;
}

// Request
static int collide(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5 && n != 4 && n != 2)
    return luaL_error(L, "Got %d arguments expected 5, 4 or 2 (self, shape [,start_time, delta_time [, function]])", n); 
    
  Shape* shape = checkShape(L,1); 
  Shape* other = checkShape(L,2);      
       
  assert(shape != 0 && other != 0);  

  real t, dt;
  if (n > 2) {
    t = luaL_checknumber(L, 3);
    dt = luaL_checknumber(L, 4);    
  }
  else {
    t = secondsPassed();
    dt = 1.0f;
  }
  
  bool ret = false;
  if (n <= 4)
    ret = shape->collide(other, t, dt);
  else if (n == 5) {
    lua_pushvalue(L,5);
    LuaCollisionAction cmd(L);
    ret = shape->collide(other, t, dt, &cmd);
  }
    
  lua_pushboolean(L, ret);     
  
  return 1;
}

static int inside(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5 && n != 4 && n != 2)
    return luaL_error(L, "Got %d arguments expected 5, 4 or 2 (self, point [,start_time, delta_time [, function]])", n); 
    
  Shape* shape = checkShape(L,1); 
  Point2 point = Vector2_pull(L, 2);
       
  assert(shape != 0);  

  real t, dt;
  if (n > 2) {
    t = luaL_checknumber(L, 3);
    dt = luaL_checknumber(L, 4);    
  }
  else {
    t = secondsPassed();
    dt = 1.0f;
  }
  
  bool ret = false;
  if (n <= 4)
    ret = shape->inside(point, t, dt);
  else if (n == 5) {
    lua_pushvalue(L,5);
    LuaAction cmd(L);
    ret = shape->inside(point, t, dt, &cmd);
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
   
  Points2 points;   
  if (shape->intersection(Circle(Vector2_pull(L, 2), luaL_checknumber(L, 3)), points))
    for_each(points.begin(), points.end(), PushValue<Point2>(L));    
  else
    lua_pushnil(L);
  
  return 1;
}

static int rectIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, rect)", n); 
    
  Shape* shape = checkShape(L);      
  assert(shape != 0);  
   
  Points2 points;
  if (shape->intersection(Rect2(Rect2_pull(L, 2)), points)) 
    for_each(points.begin(), points.end(), PushValue<Point2>(L));    
  else
    lua_pushnil(L);  
  return 1;
}

static int segmentIntersect(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, rect)", n); 
    
  Shape* shape = checkShape(L);      
  assert(shape != 0);  
   
  Points2 points;   
  bool ret = shape->intersection(Segment2(Segment2_pull(L, 2)), points);    
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

static int doPlanning(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, start_time, delta_time)", n); 
    
  Shape* shape = checkShape(L, 1);    

  shape->doPlanning(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  
  return 0;
}

// group:addKid(sprite)
static int addKid(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2", n); 
    
  Shape* shape = checkShape(L, 1);    
  Shape* kid   = checkShape(L, 2);    
  shape->addKid(kid);
  
  // Store sprite in group table using its address as key
  lua_pushlightuserdata(L, kid);
  lua_pushvalue(L,2);
  lua_settable(L,1);
  
  return 0;
}

static int removeKid(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2", n); 
    
  Shape* shape = checkShape(L, 1);    
  Shape* kid   = checkShape(L, 2);    
  shape->addKid(kid);

  // Remove the sprite from group table by setting it to nil
  lua_pushlightuserdata(L, kid);
  lua_pushnil(L);
  lua_settable(L,1);

  return 0;
}

static int kill(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    luaL_error(L, "Got %d arguments expected 1", n);   
    
  Shape* shape = checkShape(L);    
  assert(shape != 0);
  shape->kill();
  
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
  {"newShapeGroup", newShapeGroup},
  {"newGroup", newGroup},  
  {"newCircle", newCircleShape},  
  {"newRect", newRectShape2},  
  {"newSegment", newSegmentShape2},      
  {"init", init},

  // Accessors
  {"boundingBox", boundingBox},
  {"center", center},
  {"radius", radius},
        
  // Request
  {"collide", collide},
  {"inside", inside},  
  {"circleIntersect", circleIntersect},
  {"rectIntersect", rectIntersect},
  {"segmentIntersect", segmentIntersect},
    
  // Calculate     
  {"draw", draw},
    
  // Operations  
  {"update", update},
  {"doPlanning", doPlanning},  
  {"add", addKid},
  {"remove", removeKid},  
  {"kill", kill},      
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
  
  initShapeBookkeeping(L);  
}