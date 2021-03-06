/*
 *  LuaEngine.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 3.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "LuaEngine.h"

#include "Engine.h"
#include "Timing.h"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

#include "Lua/Base/LuaSprite.h"
#include "Lua/Base/LuaView.h"
#include "Lua/Base/LuaShape.h"

#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaSegment2.h"
#include "Lua/Geometry/LuaRay2.h"
#include "Lua/Geometry/LuaRect2.h"
#include "Lua/Geometry/LuaCircle.h"
//#include "Lua/Geometry/LuaTrapezoidalMap.h"
#include "Lua/Geometry/LuaGeometry.h"
//#include "Lua/Geometry/LuaTrapezoid2.h"
//#include "Lua/Geometry/LuaTrapezoidNode2.h"
//#include "Lua/Geometry/LuaEdgeData.h"
// #include "Lua/Geometry/LuaPaths2.h"
// #include "Lua/Geometry/LuaGraph2.h"  // NOTE: Depends on CGAL
#include "Lua/Geometry/LuaMotionState.h"
#include "Lua/Geometry/LuaMatrix2.h"

#include "Lua/LuaUtils.h"

// For testing Sprite Hierarchy test
#include "Base/PolygonView.h"
#include "Base/Sprite.h"
#include "Base/ShapeGroup.h"

#include "Utils/RoadMap.h"

#include <lua.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

#include <cstring>
#include <cctype>

#include <Geometry/IO.hpp>

using namespace std;

static lua_State *gLuaState;
static const char* gEngineScript = "script/engine.lua";
static const char* gStartupScript = "script/startup.lua";
static const char* gGameScript = "script/game.lua";

// Functions exported to Lua
static int renderFrame(lua_State* /*L*/) 
{

  return 0;
}

static int update(lua_State* /*L*/) 
{

  return 0;
}

static int ticks(lua_State* L)
{
  lua_pushnumber(L, getTicks());
  return 1;
}

static int seconds(lua_State* L)
{
  lua_pushnumber(L, secondsPassed());
  return 1;
}

static int ticksPerFrame(lua_State* L)
{
  lua_pushnumber(L, ticksPerFrame());
  return 1;
}

static int setTicksPerFrame(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n);
  setTicksPerFrame(luaL_checkinteger(L,1));
  return 0;
}

static int secondsPerFrame(lua_State* L)
{
  lua_pushnumber(L, secondsPerFrame());
  return 1;
}

static int ticksLeft(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n);       
  lua_pushnumber(L, ticksLeft(luaL_checkinteger(L,1)));
  return 1;
}

static int setViewportWidth(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (width)", n);   
  int width = luaL_checkinteger(L,1);
  setViewportWidth(width);
  
  return 0;
}

static int setViewportHeight(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (height)", n);   
  int width = luaL_checkinteger(L,1);
  setViewportHeight(width);
  
  return 0;
}

static int setViewport(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (width, height)", n);   
  int width = luaL_checkinteger(L,1);
  int height = luaL_checkinteger(L,2);  
  setViewport(width, height);
  
  return 0;
}

static int setWorldView(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (rect)", n);   

  setWorldView(Rect2_pull(L, 1));
  return 0;
}

static int lookAt(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (center)", n);   
  lookAt(Vector2_pull(L,1));
  return 0;
}

static int worldView(lua_State* L)
{
  int n = lua_gettop(L);
  
  if (n > 0 )
    return luaL_error(L, "Got %d argumets expected none", n);
  Rect2_push(L, worldView());
  return 1;
}

// Request
/*!
  Optimized version of ProbablisticRoadMaps nearest obstacle
  It was taking up a lot of time when computing roadmap
*/
static int nearestObstacle(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 2 (obstacles, rect, sample_point)", n);    

  Shape* shape = checkShape(L, 1);    
  Rect2  r = Rect2_pull(L, 2);
  Point2 p = Vector2_pull(L, 3);
  Point2 result;
  ClosestPointFinder finder(shape, r);
  if (finder.nearestObstacle(p, result))
    Vector2_push(L, result);
  else
    lua_pushnil(L);
  return 1;
}

/*!
  Optimized version of ProbablisticRoadMaps equidistanceVertex
  method
*/
static int equidistantVertex(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 4)
    return luaL_error(L, "Got %d arguments expected 2 (obstacles, rect, sample1, sample2)", n);    

  Shape* shape = checkShape(L, 1);
  Rect2  r  = Rect2_pull(L, 2);
  Point2 c1 = Vector2_pull(L, 3);
  Point2 c2 = Vector2_pull(L, 4);  
  Point2 c_v;
  ClosestPointFinder finder(shape, r);
  if (finder.equidistantVertex(c1, c2, c_v))
    Vector2_push(L, c_v);
  else
    lua_pushnil(L);
  return 1;
}

static int retractSample(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 2 (obstacles, rect, sample)", n);    

  Shape* shape = checkShape(L, 1);    
  Rect2  r = Rect2_pull(L, 2);
  Point2 c = Vector2_pull(L, 3);
  Point2 c_v;
  ClosestPointFinder finder(shape, r);
  if (finder.retractSample(c, c_v))
    Vector2_push(L, c_v);
  else
    lua_pushnil(L);
  return 1;
}

// Debug
static int isView(lua_State* L)
{
  bool has_meta = lua_getmetatable(L, 1); 
  if (!has_meta)
    cout << "no view meta table" << endl;

  luaL_getmetatable(L, "Lusion.View"); 
  bool eq = lua_rawequal(L, -1, -2);
  lua_pushboolean(L, has_meta && eq);
  return 1;  
}

static int isSprite(lua_State* L)
{
  bool has_meta = lua_getmetatable(L, 1);
  if (!has_meta)
    cout << "no sprite meta table" << endl;
  luaL_getmetatable(L, "Lusion.Shape"); 
  bool eq = lua_rawequal(L, -1, -2);  
  lua_pushboolean(L, has_meta && eq);
  return 1;
}

// Test the performance of different operations
static int performanceTest(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n);    
  n = luaL_checkinteger(L,1);
  int ticks;
  
  startTimer();
  trigPerformanceTest(n);
  ticks = stopTimer();
  cout << "Time to perform sine " << n << " times is " << ticks << " ms" << endl; 

  startTimer();
  gltPerformanceTest(n);
  ticks = stopTimer();
  cout << "Time to load 16 doubles " << n << " times is " << ticks << " ms" << endl; 
  
  return 0;
}

static int testMisc(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 0)
    return luaL_error(L, "Got %d arguments expected 0", n);    
  
  Vector2 v(0,-1);
  cout << "Angle of (0,-1) " << deg(v.angle()) << endl;
  return 0;
}

static int projectPoint(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 3 (dx, dy, polygon)", n);    
  
  real dx = luaL_checknumber(L, 1);
  real dy = luaL_checknumber(L, 2);    
  Vector2 v(dx, dy);
  
  Polygon2 poly;

  getPolygon(L, 3, poly);
  vector<real> proj(poly.size());  
  
  transform(poly.begin(), poly.end(), proj.begin(), ProjectPoint(v));
  for_each(proj.begin(), proj.end(), PushValue<real>(L));  
  return 1;
}

static int calcDirection(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (polygon)", n);    
    
  Polygon2 p;
  getPolygon(L, 1, p);
  
  Points2::iterator pb = p.begin(), pe = p.end();
  transform(pb+1, pe, pb, pb, CalcDirection());  
  for_each(pb, pe-1, PushValue<Point2>(L));  
  return 1;
}

static int projectPolygon(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected 3 (projections, poly1, poly2)", n);    
    
  Polygon2 d, p, q;
  getPolygon(L, 1, d);
  getPolygon(L, 2, p);
  getPolygon(L, 3, q);  

  Points2::iterator pb = p.begin(), pe = p.end(), qb = q.begin(), qe = q.end();
  
  bool is_col = find_if(d.begin(), d.end(), ProjectPolygon(pb, pe, qb, qe)) != d.end();

  lua_pushboolean(L, is_col);
  return 1;
}

static int testHierarchyIntersect(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 0)
    return luaL_error(L, "Got %d arguments expected 0", n);     
    
  vector<Shape*> sprites;

  View* view = new PolygonView;
  for (int i=0; i<5; ++i) {
    Sprite* sp = createSprite(view);
    sp->setPosition(Vector2(4.0f*i, 4.0f*i));
    sprites.push_back(sp);
  }

  ShapeGroup* group = new ShapeGroup(sprites.begin(), sprites.end());

  Rect2 r1(Vector2(-1.0f, -1.0f), Vector2(17.0f, 17.0f));    

  assert(group->boundingBox() == r1);

  group->release();
  view->release();

  return 0;
}

static int polygonCollision(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (poly1, poly2)", n);   
    
  Polygon2 p1, p2;
  getPolygon(L, 1, p1);
  getPolygon(L, 2, p2);

  bool is_col = intersect(p1.begin(), p1.end(), p2.begin(), p2.end());
  lua_pushboolean(L, is_col);
  return 1;
}



// functions that will show up in our Lua environment
static const luaL_Reg gEngineFuncs[] = {
  {"setViewportHeight", setViewportHeight},  
  {"setViewportWidth", setViewportWidth},    
  {"setViewport", setViewport},      
  {"view", worldView},      
  {"setView", setWorldView},            
  {"lookAt", lookAt},            
  {"renderFrame", renderFrame},      
  {"update", update},        
  {"ticks", ticks},        
  {"seconds", seconds},          
  {"ticksPerFrame", ticksPerFrame},          
  {"setTicksPerFrame", setTicksPerFrame},    
  {"secondsPerFrame", secondsPerFrame},                    
  {"ticksLeft", ticksLeft},          
  {"nearestObstacle", nearestObstacle},
  {"equidistantVertex", equidistantVertex},
  {"retractSample", retractSample},          
  {NULL, NULL}
};

static const luaL_Reg gDebugFuncs[] = {
  {"isView", isView},  
  {"isSprite", isSprite},    
  {"performanceTest", performanceTest},    
  {"testMisc", testMisc},    
  {"projectPoint", projectPoint},    
  {"calcDirection", calcDirection},    
  {"projectPolygon", projectPolygon}, 
  {"polygonCollision", polygonCollision},
  {"testHierarchyIntersect", testHierarchyIntersect},                       
  {NULL, NULL}
};

// Initialization
void initLua()
{
	gLuaState = lua_open();
  luaL_openlibs(gLuaState);
  
  luaL_register(gLuaState, "Engine", gEngineFuncs);
  luaL_register(gLuaState, "Debug", gDebugFuncs);

  initLuaShape(gLuaState);  
  initLuaSprite(gLuaState);
  initLuaView(gLuaState);
    
  initLuaVector2(gLuaState);
  initLuaSegment2(gLuaState);  
  initLuaRay2(gLuaState);  
  initLuaRect2(gLuaState);  
  initLuaCircle(gLuaState);    
  //initLuaTrapezoidalMap(gLuaState);
  initLuaGeometry(gLuaState);
  //initLuaTrapezoid2(gLuaState);
  // initLuaEdgeData(gLuaState);  // NOTE: Depends on CGAL
  // initLuaPaths2(gLuaState);  
  // initLuaGraph2(gLuaState);    // NOTE: Depends on CGAL
  initLuaMotionState(gLuaState);  
  initLuaMatrix2(gLuaState);    
    
  if (luaL_dofile(gLuaState, gEngineScript)) {
    cerr << "Error when executing engine init script " 
         << lua_tostring(gLuaState, -1) << endl;
  }        
  else if (luaL_dofile(gLuaState, gStartupScript)) {
    cerr << "Error when executing startup script " 
         << lua_tostring(gLuaState, -1) << endl;
  }        
}

void initGame()
{
  if (luaL_dofile(gLuaState, gGameScript)) {
    cerr << "Error when executing game init script " 
         << lua_tostring(gLuaState, -1) << endl;
  }          
}


void closeLua() 
{
    lua_close(gLuaState);
}

void debugLua()
{
  lua_getfield(gLuaState, LUA_GLOBALSINDEX, "debug");
  lua_getfield(gLuaState, -1, "debug"); // Get functioned named "debug" from table lying at the top of the stack
  lua_remove(gLuaState, -2);  // Remove element right below top of stack
  lua_call(gLuaState, 0, 0); 
}

/*! Calls lua render callback with number of milliseconds since SDL was initialized */
void luaRenderFrame(real start_time)
{
  lua_State *L = luaState(); 
  lua_getfield(L, LUA_GLOBALSINDEX, "Engine");
  lua_getfield(L, -1, "renderFrame"); // Get functioned named "onCollision" from table lying at the top of the stack
  lua_remove(L, -2);  // Remove element right below top of stack (Sprite table)
  lua_pushnumber(L, start_time);
  if (lua_pcall(L, 1, 0, 0)) {
    cerr << "Error when rendering frame: " 
         << lua_tostring(gLuaState, -1) << endl;
  }    
}

/*! Calls lua update callback with number of milliseconds since SDL was initialized */
void luaUpdate(real start_time)
{
  lua_State *L = luaState(); 
  lua_getglobal(L, "Engine");
    
  lua_getfield(L, -1, "update");
  lua_remove(L, -2);  // Remove element right below top of stack (Sprite table)
  lua_pushnumber(L, start_time);  
  if (lua_pcall(L, 1, 0, 0)) {
    cerr << "Error when calling update: " 
         << lua_tostring(gLuaState, -1) << endl;
  }
}

static bool pcall(int nargs, int nresults) 
{
  int error_code = lua_pcall(gLuaState, nargs, nresults, 0);
  if (error_code) {
    cerr << "Error when calling pcall (set get property): " 
         << lua_tostring(gLuaState, -1) << endl;
  }
  return error_code == 0;
}

static void splitKeyPath(const char* key_path, vector<string> & keys)
{
  const char* begin = key_path;
  const char* end   = key_path + strlen(key_path);
  
  const char *it = begin;
  const char *it_prev = begin;

  while (it != end) {
    it = find(it_prev, end, '.');
    string key(it_prev, it);
    keys.push_back(key);
    it_prev = it+1;
  }  
}

/*!
  Will put value found by 
*/
static void fetchValue(vector<string> & keys)
{
  lua_State *L = luaState(); 
  vector<string>::const_iterator it = keys.begin();
  lua_getglobal(L, it->c_str());
  ++it;
  lua_getfield(L, -1, it->c_str());
  ++it;
  pcall(0, 1);
  
  while (it != keys.end()) {
    lua_getfield(L, -1, it->c_str());
    lua_insert(L, -2); // swaps function and 'this' on stack, so function objects is at bottom
    pcall(1, 1);
    ++it;
  }  
}

/*!
  Sets a property on a lua object in the global lua state. The name of the property
  can be given as a property path similar to keypaths in Cocoa. 
  \code
  luaSetNumberProperty("Engine.player.rotation", 180);
  \endcode
  is the same as calling:
  
  Engine.player():setRotation(180)
  
  In lua code
*/
void luaSetNumberProperty(const char* key_path, double value)
{
  lua_State *L = luaState(); 

  vector<string> keys;
  splitKeyPath(key_path, keys);
  string key = keys.back();
  keys.pop_back();
  fetchValue(keys);

  key = "set" + key;
  key[3] = (char)toupper(key[3]);
  lua_getfield(L, -1, key.c_str());
  lua_insert(L, -2); // swaps function and 'this' on stack, so function objects is at bottom
  lua_pushnumber(L, value);  
  pcall(2, 0);      
}

/*!
  Gets a property on a lua object in the global lua state. The name of the property
  can be given as a property path similar to keypaths in Cocoa.
  
  \code
  luaGetNumberProperty("Engine.player.rotation");
  \endcode
  is the same as calling:
  
  Engine.player():rotation(180)
  
  In lua code    
*/
double luaGetNumberProperty(const char* key_path)
{
  lua_State *L = luaState(); 

  vector<string> keys;
  splitKeyPath(key_path, keys);
  fetchValue(keys);
  real number = luaL_checknumber(L, -1);
  return number;
}

/*!
  Similar to luaSetNumberProperty but will not work for key paths.
  Instead it it works only on boolean properties found on the Engine
  global table in lua code.
  
  \code
  luaSetEngineBoolean("Engine.keystate", 12, true);
  \endcode
  is the same as calling:
  
  Engine.setKeystate(12, true)
  
  In lua code    
*/
void luaSetEngineBoolean(const char* key, int int_key, bool value)
{
  lua_State *L = luaState(); 
  lua_getglobal(L, "Engine");
  string s(key);
  s[0] = (char)toupper(s[0]);
  s = "set"+s; 
  lua_getfield(L, -1, s.c_str());
  lua_remove(L, -2);  
  lua_pushnumber(L, int_key);
  lua_pushboolean(L, value);    
  pcall(2, 0);      
}

/*!
  Similar to luaGetNumberProperty but will not work for key paths.
  Instead it it works only on boolean properties found on the Engine
  global table in lua code.
  
  \code
  luaGetEngineBoolean("Engine.keystate", 12);
  \endcode
  is the same as calling:
  
  Engine.keystate(12)
  
  In lua code
*/
bool luaGetEngineBoolean(const char* key, int int_key)
{
  lua_State *L = luaState(); 
  lua_getglobal(L, "Engine");
  lua_getfield(L, -1, key);
  lua_remove(L, -2);  
  lua_pushnumber(L, int_key);
  pcall(1, 1);      
  bool value = lua_toboolean(L, -1);
  return value;
}


// Accessors
lua_State* luaState()
{
  return gLuaState;
}