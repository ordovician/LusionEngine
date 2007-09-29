/*
 *  LuaGeometry.cpp
 *  LusionGeometry
 *
 *  Created by Erik Engheim on 3.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaGeometry.h"

#include "Lua/LuaUtils.h"

#include <Geometry/Vector2.hpp>

#include <lua.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


#include <CGAL/Cartesian.h> 
// #include <CGAL/ch_graham_andrew.h> 
#include <CGAL/convex_hull_2.h> 

using namespace std;

// typedefs
typedef CGAL::Point_2<CGAL::Cartesian<double> > Point_2; 
typedef vector<Point_2> Points_2;

// Helper functions
Point2 Point2_fromCGAL(const Point_2& p) {
  return Point2(p.x(), p.y());
}

Point_2 Point2_toCGAL(const Point2& p) {
  return Point_2(p.x(), p.y());
}

void pushTable(lua_State* L, const Point_2& p)
{
  pushTable(L, Point2_fromCGAL(p));
}

// Functions exported to Lua
static int convexHull(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
   return luaL_error(L, "Got %d arguments expected 1 (points)", n); 

  Polygon2 poly;
  getPolygon(L, 1, poly);
  
  Points_2 points, hull;
  transform(poly.begin(), poly.end(), back_inserter(points), Point2_toCGAL);
  
  CGAL::convex_hull_2(points.begin(), points.end(), back_inserter(hull));
  for_each(hull.begin(), hull.end(), PushValue<Point_2>(L));
  return 1; 
}

// functions that will show up in our Lua environment
static const luaL_Reg gGeometryFuncs[] = {
  {"convexHull", convexHull},  
  {NULL, NULL}
};


// Initialization
void initLuaGeometry(lua_State *L)
{
  luaL_register(L, "Geometry", gGeometryFuncs);    
}
