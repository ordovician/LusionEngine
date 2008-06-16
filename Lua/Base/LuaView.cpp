/*
 *  LuaView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 14.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Base/LuaView.h"

#include "LuaEngine.h"
#include "Engine.h"
#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"
#include "Lua/Geometry/LuaTrapezoid2.h"

#include "Base/PolygonView.h"
#include "Base/ImageView.h"
#include "Base/SegmentView.h"
#include "Base/PointsView.h"
#include "Base/TrapezoidView.h"

#include <lua.hpp>
#include <cassert>

#include <iostream>

// Helper functions

// Functions exported to Lua
// Constructors
static int newGenericPolygonView(lua_State *L, GLenum style) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2 && n != 1)
   return luaL_error(L, "Got %d arguments expected 1 or 2", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);

  View **v = (View **)lua_newuserdata(L, sizeof(View *));
  *v = 0;
  if (n == 1) {
    *v = new PolygonView(style);
  }
  else {
    Polygon2 p;
    getPolygon(L, 2, p);
    *v = new PolygonView(p, style);                   
  }

  setUserDataMetatable(L, "Lusion.View");

  return 1; 
}

static int newPolygonView(lua_State *L) 
{
  return newGenericPolygonView(L, GL_POLYGON);
}

static int newPathView(lua_State *L) 
{
  return newGenericPolygonView(L, GL_LINE_STRIP);
}

static int newOutlineView(lua_State *L) 
{
  return newGenericPolygonView(L, GL_LINE_LOOP);
}

static int newImageView(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n < 2 || n > 5)
   return luaL_error(L, "Got %d arguments expected between 2 and 5 arguments", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);

  View **v = (View **)lua_newuserdata(L, sizeof(View *));
  *v = 0;  
  StringList image_files;
  if (lua_isstring(L,2)) {
    image_files.push_back(std::string(luaL_checkstring(L,2)));
  }
  else if (lua_istable(L,2)) {
    getStrings(L, 2, image_files);
  }
  else
    return luaL_error(L, "Argument 2 has to be a string or table of strings", n); 
    
  if (n >= 3 && lua_istable(L,3)) {
    Polygon2 p;
    getPolygon(L, 3, p);
    if (n == 5)
      *v = new ImageView(image_files, p, luaL_checknumber(L,4), luaL_checknumber(L,5));                            
    else
      *v = new ImageView(image_files, p);                    
  }
  else if (n == 4) {
    *v = new ImageView(image_files, luaL_checknumber(L,3), luaL_checknumber(L,4));                            
  }
  else
    *v = new ImageView(image_files);            

  setUserDataMetatable(L, "Lusion.View");

  return 1; 
}

static int newSegmentView(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2 && n != 1)
   return luaL_error(L, "Got %d arguments expected 1 or 2", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);

  View **v = (View **)lua_newuserdata(L, sizeof(View *));
  *v = 0;
  if (n == 1) {
   *v = new SegmentView;
  }
  else {
    Segments2 s;
    getSegments(L, 2, s);
    *v = new SegmentView(s.begin(), s.end());                   
  }

  setUserDataMetatable(L, "Lusion.View");

  return 1; 
}

static int newPointsView(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2 && n != 1)
   return luaL_error(L, "Got %d arguments expected 1 or 2", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);

  View **v = (View **)lua_newuserdata(L, sizeof(View *));
  *v = 0;
  if (n == 1) {
   *v = new PointsView;
  }
  else {
    Polygon2 s;
    getPolygon(L, 2, s);
    *v = new PointsView(s.begin(), s.end());                   
  }

  setUserDataMetatable(L, "Lusion.View");

  return 1; 
}

static int newTrapezoidView(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
   return luaL_error(L, "Got %d arguments expected 2 (class, trapezoid)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);

  View **v = (View **)lua_newuserdata(L, sizeof(View *));
  *v = new TrapezoidView(Trapezoid2_pull(L, 2));

  setUserDataMetatable(L, "Lusion.View");

  return 1; 
}

// Destructors
// __gc
static int destroyView(lua_State* L)
{
  View* view = 0;
  checkUserData(L, "Lusion.View", view);
  view->release();
  return 0;
}	

// Accessors
static int origin(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    View* view = checkView(L);
    Point2 p = view->origin();
    pushTable(L,p);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 2;
}

static int setOrigin(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 3) {
    View* view = checkView(L);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    view->setOrigin(Point2(x,y));
  }
  else if (n == 2) 
    checkView(L)->setOrigin(Vector2_pull(L, 2));          
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

// view:setBoundingBox(xmin,ymin, xmax, ymax)
//static int setBoundingBox(lua_State *L)
//{
//  int n = lua_gettop(L);
//  if (n != 5)
//    return luaL_error(L, "Got %d arguments expected 5", n);
//  View* view = checkView(L);
//  real xmin = luaL_checknumber(L,2);
//  real ymin = luaL_checknumber(L,3);
//  real xmax = luaL_checknumber(L,4);
//  real ymax = luaL_checknumber(L,5);      
//  view->setBoundingBox(Rect2(xmin,ymin,xmax,ymax));
//
//  return 0;
//}

//static int  boundingBox(lua_State *L)
//{
//  int n = lua_gettop(L);
//  if (n != 1)
//    return luaL_error(L, "Got %d arguments expected 1", n);  
//  View* view = checkView(L);
//  Rect2 r = view->boundingBox();
//  pushValue(L, r);
//  return 4;
//}

// view:setCollisionPolygon(points)
static int setPolygon(lua_State *L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2", n);
  View* view = checkView(L);
  Polygon2 p;
  getPolygon(L, 2, p);
  view->setCollisionPolygon(p);
  return 0;
}

static int polygon(lua_State *L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n);  
  View* view = checkView(L);
  Polygon2 p = view->collisionPolygon();
  for_each(p.begin(), p.end(), PushValue<Point2>(L));
  return 1;
}

static int setColor(lua_State *L)
{
  int n = lua_gettop(L);
  if (n != 4)
    return luaL_error(L, "Got %d arguments expected 4", n);
  View* view = checkView(L);
  real r = luaL_checknumber(L,2);
  real g = luaL_checknumber(L,3);
  real b = luaL_checknumber(L,4);    
  view->setColor(r, g, b);
  return 0;
}

static int setArrowVisible(lua_State *L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2", n);
  SegmentView* view = checkSegmentView(L);
  bool is_visible = checkBool(L, 2); 
  view->setArrowVisible(is_visible);
  return 0;
}

static int arrowVisible(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  SegmentView* view = checkSegmentView(L);
  assert(view != 0);  
  lua_pushboolean(L, view->arrowVisible());
  return 1;
}

// view:id()  -- Gets the address of view
static int id(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  View* view = checkView(L);
  assert(view != 0);  
  lua_pushlightuserdata(L, view);
  return 1;
}


// Operations
static int addSegment(lua_State *L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2", n);
  SegmentView* view = checkSegmentView(L);
  view->addSegment(Segment2_pull(L,2));
  return 0;
}


// functions that will show up in our Lua environment

static const luaL_Reg gPolygonViewFuncs[] = {
  {"new", newPolygonView},
  {NULL, NULL}
};

static const luaL_Reg gPathViewFuncs[] = {
  {"new", newPathView},
  {NULL, NULL}
};

static const luaL_Reg gOutlineViewFuncs[] = {
  {"new", newOutlineView},
  {NULL, NULL}
};

static const luaL_Reg gImageViewFuncs[] = {
  {"new", newImageView},
  {NULL, NULL}
};

static const luaL_Reg gSegmentViewFuncs[] = {
  {"new", newSegmentView},
  {"add", addSegment},  
  {"setArrowVisible", setArrowVisible},  
  {"arrowVisible", arrowVisible},    
  {NULL, NULL}
};

static const luaL_Reg gPointsViewFuncs[] = {
  {"new", newPointsView},
  {NULL, NULL}
};

static const luaL_Reg gTrapezoidViewFuncs[] = {
  {"new", newTrapezoidView},
  {NULL, NULL}
};

static const luaL_Reg gDestroyViewFuncs[] = {
  {"__gc", destroyView},  
  {NULL, NULL}  
};

static const luaL_Reg gViewFuncs[] = {
  // Accessors
  {"origin", origin},
  {"setOrigin", setOrigin},  
//  {"boundingBox", boundingBox},
//  {"setBoundingBox", setBoundingBox},    
  {"polygon", polygon},
  {"setPolygon", setPolygon},    
  {"setColor", setColor},      
  {"id", id},      
  
  // Operations
  {NULL, NULL}
};

static void registerFuncs(lua_State* L, const char* name, const luaL_Reg* funcs)
{
  luaL_register(L, name, funcs);
  luaL_register(L, 0, gViewFuncs);      
}

// Initialization
void initLuaView(lua_State *L)
{  
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.View");
  luaL_register(L, 0, gDestroyViewFuncs);      
  luaL_register(L, 0, gViewFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  
    
  registerFuncs(L, "PolygonView", gPolygonViewFuncs);
  registerFuncs(L, "PathView", gPathViewFuncs);
  registerFuncs(L, "OutlineView", gOutlineViewFuncs);
      
  registerFuncs(L, "ImageView", gImageViewFuncs);  
  registerFuncs(L, "SegmentView", gSegmentViewFuncs);    
  registerFuncs(L, "PointsView", gPointsViewFuncs);      
  registerFuncs(L, "TrapezoidView", gTrapezoidViewFuncs);
  
}