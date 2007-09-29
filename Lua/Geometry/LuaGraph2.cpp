/*
 *  LuaGraph2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaGraph2.h"
#include "Lua/LuaUtils.h"
#include "Lua/Geometry/LuaTrapezoid2.h"
#include "Lua/Geometry/LuaPaths2.h"
#include "Lua/Geometry/LuaEdgeData.h"
#include "Geometry/Graph2.hpp"

#include <iostream>

#include <lua.hpp>
#include <assert.h>

// Helper functions
Graph2 *checkGraph2(lua_State* L, int index)
{
 Graph2* v;
 pullClassInstance(L, index, "Lusion.Graph2", v);
 return v;
}

/*! 
  Gets edge data from array of edge data stored at index t in 
  lua stack. 
*/
void getEdgeDatas(lua_State* L, int t, EdgePairs& d) 
{
  luaL_checktype(L, t, LUA_TTABLE); // Make sure we get a table with points as first argument
  lua_pushnil(L); // first key (ready traversal of table)
  while (lua_next(L, t) != 0) { 
    // ‘key’ is at index -2 and ‘value’ at index -1 
    d.push_back(EdgeData_pull(L,-1));
    lua_pop(L, 1); // removes ‘value’; keeps ‘key’ for next iteration   
  }
}


// Functions exported to Lua
// Graph2.new(points)
// E.g. v = Graph2.new({{1,2}, {1,1}, {4,4}})
static int newGraph2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3)
    return luaL_error(L, "Wrong number of arguments in Graph2 constructor. Should be 3 (self, #verticies, edges)"); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Graph2 **g = (Graph2 **)lua_newuserdata(L, sizeof(Graph2 *));
  
  int noVertices = luaL_checkinteger(L,2);
  EdgePairs edges;
  getEdgeDatas(L, 3, edges);
  
  *g = Graph2::create(noVertices, edges.begin(), edges.end());

  setUserDataMetatable(L, "Lusion.Graph2");

  return 1; 
}

// Calculations
static int shortestPaths(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, trapezoid)", n); 
    
  Graph2* graph = checkGraph2(L);    
  Trapezoid2* trap = checkTrapezoid2(L, 2);
  
  Paths2* paths = graph->shortestPaths(trap);
  
  if (paths == 0)
    lua_pushnil(L);
  else {
    Paths2_push(L, paths);
  }
      
  return 1;
}

static int shortestPath(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 2 (self, trapezoid, trapezoid)", n); 
    
  Graph2* graph = checkGraph2(L);    
  Trapezoid2* source = checkTrapezoid2(L, 2); assert(source != 0);
  Trapezoid2* target = checkTrapezoid2(L, 3); assert(target != 0); 
  
  Polygon2 path;
  if (graph->shortestPath(source, target, path))
    for_each(path.begin(), path.end(), PushValue<Point2>(L));  
  else
    lua_pushnil(L);
      
  return 1;
}

static int printGraph(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  Graph2* graph = checkGraph2(L);    
  graph->printGraph();
        
  return 0;
}

// __gc
static int destroyGraph2(lua_State* L)
{
  Graph2* graph = 0;
  checkUserData(L, "Lusion.Graph2", graph);
  delete graph;
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyGraph2Funcs[] = {
  {"__gc", destroyGraph2},  
  {NULL, NULL}  
};

static const luaL_Reg gGraph2Funcs[] = {
  {"new", newGraph2},
  // Calculations
  {"shortestPaths", shortestPaths},
  {"shortestPath", shortestPath},  
  {"printGraph", printGraph},  
  {NULL, NULL}
};

// Initialization
void initLuaGraph2(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Graph2");
  luaL_register(L, 0, gDestroyGraph2Funcs);      
  luaL_register(L, 0, gGraph2Funcs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Graph", gGraph2Funcs);  
}