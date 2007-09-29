/*
 *  LuaMatrix2.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaMatrix2.h"
#include "Lua/Geometry/LuaVector2.h"
#include "Lua/LuaUtils.h"

#include "Utils/PolygonUtils.h"
#include <Geometry/Matrix2.hpp>

#include <iostream>

#include <lua.hpp>
#include <assert.h>

// Helper functions
Matrix2 *checkMatrix2(lua_State* L, int index)
{
  Matrix2* v;
  pullClassInstance(L, index, "Lusion.Matrix2", v);
  return v;
}

void Matrix2_push(lua_State *L, int index, Matrix2* mstate)
{
  lua_newtable(L);
  lua_getmetatable(L, index);       
  lua_setmetatable(L, -2);

  Matrix2 **s = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));  
  *s = mstate; 
  setUserDataMetatable(L, "Lusion.Matrix2");  
}

void Matrix2_push(lua_State *L, Matrix2* mstate)
{
  lua_newtable(L);
  lua_getglobal(L, "Matrix");
  lua_setmetatable(L, -2);

  Matrix2 **s = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));  
  *s = mstate; 
  setUserDataMetatable(L, "Lusion.Matrix2");  
}

static void checkArguments(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    luaL_error(L, "Got %d arguments expected 2 (self, matrix)", n);
}


// Functions exported to Lua
static int newMatrix2(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (class)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Matrix2 **matrix = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));
  *matrix = new Matrix2;

  setUserDataMetatable(L, "Lusion.Matrix2");

  return 1; 
}

static int zero(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (class)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Matrix2 **matrix = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));
  *matrix = new Matrix2(Matrix2::zero());

  setUserDataMetatable(L, "Lusion.Matrix2");

  return 1; 
}

static int identity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1 (class)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Matrix2 **matrix = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));
  *matrix = new Matrix2(Matrix2::identity());

  setUserDataMetatable(L, "Lusion.Matrix2");

  return 1; 
}

static int translate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2 && n != 3)
    return luaL_error(L, "Got %d arguments expected 2 or 3 (class, x, y)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Matrix2 **matrix = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));
  
  if (n == 3)
    *matrix = new Matrix2(Matrix2::translate(luaL_checknumber(L,2), luaL_checknumber(L,3)));
  else
    *matrix = new Matrix2(Matrix2::translate(Vector2_pull(L, 2)));

  setUserDataMetatable(L, "Lusion.Matrix2");

  return 1; 
}

static int scale(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3)
    return luaL_error(L, "Got %d arguments expected  3 (class, x, y)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Matrix2 **matrix = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));
  *matrix = new Matrix2(Matrix2::scale(luaL_checknumber(L,2), luaL_checknumber(L,3)));

  setUserDataMetatable(L, "Lusion.Matrix2");

  return 1; 
}

static int rotate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected  2 (class, angle)", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 

  pushClassInstance(L);
    
  Matrix2 **matrix = (Matrix2 **)lua_newuserdata(L, sizeof(Matrix2 *));
  real angle = rad(luaL_checknumber(L,2));
  *matrix = new Matrix2(Matrix2::rotate(angle));
  setUserDataMetatable(L, "Lusion.Matrix2");

  return 1; 
}

// Accessors
static int at(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 3 (self, row, col)", n); 
    
  Matrix2* matrix = checkMatrix2(L);    

  // Rows and columns are numbered from 1 to 3 like arrays in lua, so we need to
  // substract 1
  int row = luaL_checkinteger(L,2)-1; 
  int col = luaL_checkinteger(L,3)-1;  
  
  lua_pushnumber(L, matrix->at(row, col));
  return 1;
}

static int setAt(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 4) 
    return luaL_error(L, "Got %d arguments expected 4 (self, row, col, value)", n); 
    
  Matrix2* matrix = checkMatrix2(L);    
  
  // Rows and columns are numbered from 1 to 3 like arrays in lua, so we need to
  // substract 1  
  int row = luaL_checkinteger(L,2)-1;
  int col = luaL_checkinteger(L,3)-1;  
  real value = luaL_checknumber(L,4);
  
  matrix->setAt(row, col, value);

  return 0;
}

static int atRow(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, row)", n); 
    
  Matrix2* matrix = checkMatrix2(L);    

  // Rows and columns are numbered from 1 to 3 like arrays in lua, so we need to
  // substract 1
  int row = luaL_checkinteger(L,2)-1; 
  
  lua_pushnumber(L, matrix->at(row, 0));
  lua_pushnumber(L, matrix->at(row, 1));
  lua_pushnumber(L, matrix->at(row, 2));    
  return 3;
}

static int atCol(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, col)", n); 
    
  Matrix2* matrix = checkMatrix2(L);    

  // Rows and columns are numbered from 1 to 3 like arrays in lua, so we need to
  // substract 1
  int col = luaL_checkinteger(L,2)-1; 
  
  lua_pushnumber(L, matrix->at(0, col));
  lua_pushnumber(L, matrix->at(1, col));
  lua_pushnumber(L, matrix->at(2, col));    
  return 3;
}

// Calculations
static int transpose(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
    
  Matrix2* matrix = checkMatrix2(L);    
  Matrix2_push(L,1, new Matrix2(matrix->getTranspose()));
  return 1;
}

static int transformPoint(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, point)", n); 
    
  Matrix2* matrix = checkMatrix2(L);
  Point2 p = Vector2_pull(L,2);  
  Vector2_push(L, matrix->transformPoint(p));
  return 1;
}

static int transformVector(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, point)", n); 
    
  Matrix2* matrix = checkMatrix2(L);
  Vector2 v = Vector2_pull(L,2);  
  Vector2_push(L, matrix->transformVector(v));
  return 1;
}

static int add(lua_State *L) 
{
  checkArguments(L);
  Matrix2* v = checkMatrix2(L, 1);
  Matrix2* u = checkMatrix2(L, 2); 
  Matrix2_push(L, new Matrix2(*v+*u)); 
  return 1; 
}

static int sub(lua_State *L) 
{
  checkArguments(L);
  Matrix2* v = checkMatrix2(L, 1);
  Matrix2* u = checkMatrix2(L, 2); 
  Matrix2_push(L, new Matrix2(*v-*u)); 
  return 1; 
}

static int multiply(lua_State *L) 
{
  checkArguments(L);
  Matrix2* v = checkMatrix2(L, 1);
  if (lua_isnumber(L,2)) {
    real scale = lua_tonumber(L,2);
    Matrix2_push(L, new Matrix2((*v)*scale));         
  }
  else {
    Matrix2* u = checkMatrix2(L, 2); 
    Matrix2_push(L, new Matrix2((*v)*(*u)));     
  }
  return 1; 
}

// __gc
static int destroyMatrix2(lua_State* L)
{
  Matrix2* matrix = 0;
  checkUserData(L, "Lusion.Matrix2", matrix);
  delete matrix;
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroyMatrix2Funcs[] = {
  {"__gc", destroyMatrix2},  
  {NULL, NULL}  
};

static const luaL_Reg gMatrix2Funcs[] = {
  // Constructors
  {"new", newMatrix2},
  {"zero", zero},
  {"identity", identity},
  {"translate", translate},
  {"scale", scale},
  {"rotate", rotate},

  // Accessors
  {"at", at},
  {"setAt", setAt},
  {"atRow", atRow},
  {"atCol", atCol},
    
  // Calculations
  {"transpose", transpose},
  {"transformPoint", transformPoint},
  {"transformVector", transformVector},
    
  {"__add", add},
  {"__sub", sub},  
  {"__mul", multiply},          

     
  {NULL, NULL}
};

// Initialization
void initLuaMatrix2(lua_State *L)
{    
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Matrix2");
  luaL_register(L, 0, gDestroyMatrix2Funcs);      
  luaL_register(L, 0, gMatrix2Funcs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  

  luaL_register(L, "Matrix", gMatrix2Funcs);  
}