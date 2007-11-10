/*
 *  LuaSprite.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 13.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Lua/Geometry/LuaMotionState.h"
#include "Lua/Geometry/LuaVector2.h"
#include "Lua/Geometry/LuaRect2.h"
#include "Lua/Base/LuaSprite.h"
#include "Lua/Base/LuaCollisionGroup.h"
#include "Lua/LuaUtils.h"
#include "LuaEngine.h"
#include "Engine.h"
#include "Base/Group.h"
#include "Base/Command.h"
#include "Base/Sprite.h"
#include "Base/ShapeGroup.h"

#include "Engine.h"


#include <lua.hpp>
#include <assert.h>

#include <iostream>

// Helper functions
/*!
 Create a table to hold mapping between lua tables and pointers. The table has weak
 references so that lua tables are garbage collected even if there is one reference
 left to table in the this mapping table.
*/
void initSpriteBookkeeping(lua_State* L)
{
  luaL_newmetatable(L, "Lusion.Sprites"); // Make new table in registry to hold mapping between sprite pointers and tables
  
  lua_pushstring(L, "v");                 // Make table weak so that sprites can be garbage collected
  lua_setfield(L, -2, "__mode");
  
  lua_pushvalue(L,-1);                    // Let the table be its own metatable
  lua_setmetatable(L, -2);    
}

void registerSpriteTable(lua_State* L, Sprite* sprite)
{
  registerPointer(L, sprite, "Lusion.Sprites");
}

void retrieveSpriteTable(lua_State* L, Sprite* sprite)
{
  retrievePointer(L, sprite, "Lusion.Sprites");  
}
 
/*! 
 Sets the sprite to point to the same view as the parent class, if parents has any view
 Assumes new instance table is at the top of stack.
*/
static void cloneView(lua_State *L, Sprite* sprite)
{
  lua_getfield(L, 1, "__view");

  // Make sure there is actually view table there
  if (lua_istable(L, -1)) {
    View* view = checkView(L, -1);
    assert(view != 0);
    sprite->setView(view);
    lua_setfield(L,-2, "__view");
  }
  lua_pop(L,1);
}

// Functions exported to Lua
// Sprite.new(x,y,dir,speed) or Sprite.new(pos,dir,speed)
// pos = {x,y}
static int newSprite(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 5 && n != 1 && n != 2 && n != 4) 
    return luaL_error(L, "Got %d arguments expected 5,4 or 1", n); 
  luaL_checktype(L, 1, LUA_TTABLE); 
  
  // Create a table and set its metatable to first argument of 'new' function
  // This way we can mimic inheritance  
  pushClassInstance(L);
  
  Sprite **s = (Sprite **)lua_newuserdata(L, sizeof(Sprite *));  
  
  if (n == 5) {
    real x = luaL_checknumber (L, 2);      
    real y = luaL_checknumber (L, 3); 
    real dir = luaL_checknumber (L, 4);      
    real speed = luaL_checknumber (L, 5); 
    *s = new Sprite(Point2(x, y), dir, speed);        
  }
  else if (n == 4) {
    real dir = luaL_checknumber (L, 3);      
    real speed = luaL_checknumber (L, 4); 
    *s = new Sprite(Vector2_pull(L,2), dir, speed);    
  }
  else if (n == 2){
    View* view = checkView(L,2);
    *s = new Sprite(view);  
    lua_pushvalue(L, 2);
    lua_setfield(L, -3, "__view");    
    
  }
  else
    *s = new Sprite; 

  setUserDataMetatable(L, "Lusion.Sprite");

  // Inherit view from parent
  if (n != 2)
    cloneView(L, *s);

  // Register sprite in a weak table in registry so lua values can easily be retrieved in future using address as key
  registerSpriteTable(L, *s);
  
  // Handle user initialization
  lua_getfield(L, 1, "init"); 
  lua_pushvalue(L, -2);     // Our new instance should be lying on stack right below function 'init'
  lua_call(L, 1, 0);     

  return 1; 
}

static int init(lua_State *L) 
{
  // int n = lua_gettop(L);  // Number of arguments
  // if (n != 1)
  //   return luaL_error(L, "Got %d arguments expected 1 (self)", n); 
  //   
  // Sprite* sprite = checkSprite(L);
  // 
  // cout << "initializing sprite: 0x" << hex << (int)sprite << endl;
  
  return 0;
}

// __gc for Sprite
static int destroySprite(lua_State* L)
{
  Sprite* sprite = 0;
  checkUserData(L, "Lusion.Sprite", sprite);
  sprite->release();
  return 0;
}

// Accessors
static int name(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    lua_pushstring(L, sprite->name().c_str());
  }
  else
    return luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setName(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    const char* name = luaL_checkstring (L, 2); 
    sprite->setName(name);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

// sprite:setPosition(x,y)
static int setPosition(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 3) {
    Sprite* sprite = checkSprite(L);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    sprite->setPosition(Point2(x,y));
  }
  else if (n == 2) 
    checkSprite(L)->setPosition(Vector2_pull(L, 2));    
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

// sprite:position()
static int position(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  Point2 p = sprite->position();
  pushTable(L, p);
  
  return 1;
}

static int prevPosition(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  Point2 p = sprite->prevPosition();
  pushTable(L, p);
  
  return 1;
}

static int nextPosition(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  Point2 p = sprite->nextPosition();
  pushTable(L, p);
  
  return 1;
}

// sprite:velocity()
static int velocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    Vector2 v = sprite->velocity();
    pushTable(L,v);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int speed(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    real x = sprite->speed();
    lua_pushnumber(L, x);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

// sprite:setVelocity(x,y)
static int setVelocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 3) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    sprite->setVelocity(Vector2(x,y));
  }
  else if (n == 2) 
    checkSprite(L)->setVelocity(Vector2_pull(L, 2));      
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

static int setSpeed(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    real speed = luaL_checknumber (L, 2); 
    sprite->setSpeed(speed);
  }
  else
    luaL_error(L, "Got %d arguments expected 2 (self, speed)", n); 
  
  return 0;
}

static int setDirection(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 3) {
    Sprite* sprite = checkSprite(L);
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    sprite->setDirection(Vector2(x,y));
  }
  else if (n == 2) 
    checkSprite(L)->setDirection(Vector2_pull(L, 2));        
  else
    luaL_error(L, "Got %d arguments expected 3", n); 
  
  return 0;
}

// sprite:direction()
static int direction(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    pushTable(L, sprite->direction());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setRotation(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    real deg = luaL_checknumber (L, 2); 
    sprite->setRotation(deg);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int rotation(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    lua_pushnumber(L, sprite->rotation());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setAngularVelocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    real deg = luaL_checknumber (L, 2); 
    sprite->setAngularVelocity(deg);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int angularVelocity(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    lua_pushnumber(L, sprite->angularVelocity());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setAngularAcceleration(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    real deg = luaL_checknumber (L, 2); 
    sprite->setAngularAcceleration(deg);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int angularAcceleration(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    lua_pushnumber(L, sprite->angularAcceleration());
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setMotionState(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);    
    MotionState* s = checkMotionState(L,2);
    assert(s != 0);        
    sprite->setMotionState(s);
  }
  else
    return luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int motionState(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    MotionState *s = sprite->motionState();
    MotionState_push(L,s);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

// sprite:prevDirection()
// static int prevDirection(lua_State *L) 
// {
//   int n = lua_gettop(L);  // Number of arguments
//   if (n == 1) {
//     Sprite* sprite = checkSprite(L);
//     assert(sprite != 0);
//     real dir = sprite->prevDirection();
//     lua_pushnumber(L, dir);
//   }
//   else
//     luaL_error(L, "Got %d arguments expected 1", n); 
//   
//   return 1;
// }

static int depth(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    real depth = sprite->depth();
    lua_pushnumber(L, depth);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setDepth(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    int depth = luaL_checkint (L, 2); 
    sprite->setDepth(depth);
  }
  else
    luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int view(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
  checkSprite(L);  
  lua_getfield(L, 1, "__view");
  return 1;   
}

static int setView(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, view)", n); 
  Sprite* sprite = checkSprite(L,1);
  assert(sprite != 0);  
  View* v = checkView(L,2);
  assert(v != 0);
  lua_pushvalue(L, 2);
  lua_setfield(L, 1, "__view");
  sprite->setView(v);
  return 0;
}

static int subViewIndex(lua_State *L)
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);  
  assert(sprite != 0);
  lua_pushinteger(L, sprite->subViewIndex());
  return 1;   
}

static int setSubViewIndex(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2", n); 
  Sprite* sprite = checkSprite(L,1);
  assert(sprite != 0);  
  sprite->setSubViewIndex(luaL_checkint(L,2));
  return 0;
}

static int polygon(lua_State *L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n);  
  Sprite* sprite = checkSprite(L);
  ShallowPoints2 p = sprite->collisionPolygon();
  for_each(p.first, p.second, PushValue<Point2>(L));
  return 1;
}

// sprite:id()  -- Gets the address of sprite
static int id(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);  
  lua_pushlightuserdata(L, sprite);
  return 1;
}

static int groups(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 1)
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);  
  GroupSet groups = sprite->groups();
  groups.erase(renderGroup());  // We don't have a corresponding Lua object for this one
  for_each(groups.begin(), groups.end(), PushUserData<Group*>(L,1));

  return 1;
}

static int setCollisionHandler(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, function)", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);  
  
  if (lua_isnil(L, 2)) {
    sprite->setCollisionCommand(0);
  }
  else {
    lua_pushvalue(L,2);
    SpriteCommand* cmd = new LuaCommand(L);
    sprite->setCollisionCommand(cmd);
    cmd->release();    
  }

  return 0;
}

static int setInsideHandler(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, function)", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);  
  
  if (lua_isnil(L, 2)) {
    sprite->setInsideCommand(0);
  }
  else {
    lua_pushvalue(L,2);
    SpriteCommand* cmd = new LuaCommand(L);
    sprite->setInsideCommand(cmd);
    cmd->release();    
  }

  return 0;
}

static int setUpdateHandler(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, function)", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);  
  
  if (lua_isnil(L, 2)) {
    sprite->setUpdateCommand(0);
  }
  else {
    lua_pushvalue(L,2);
    SpriteCommand* cmd = new LuaCommand(L);
    sprite->setUpdateCommand(cmd);
    cmd->release();    
  }

  return 0;
}



static int setPlanningHandler(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, function)", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);  
  
  if (lua_isnil(L, 2)) {
    sprite->setPlanCommand(0);
  }
  else {
    lua_pushvalue(L,2);
    SpriteCommand* cmd = new LuaCommand(L);
    sprite->setPlanCommand(cmd);
    cmd->release();    
  }

  return 0;
}


// Request
static int visible(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    bool is_visible = sprite->visible();
    lua_pushboolean(L, is_visible);
  }
  else
    luaL_error(L, "Got %d arguments expected 1", n); 
  
  return 1;
}

static int setVisible(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  
  if (n == 2) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);    
    bool is_visible = checkBool(L, 2); 
    sprite->setVisible(is_visible);
  }
  else
    luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int viewCollide(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 1) {
    Sprite* sprite = checkSprite(L);
    assert(sprite != 0);
    lua_pushboolean(L, sprite->viewCollide());
  }
  else
    return luaL_error(L, "Got %d arguments expected 1", n); 
  return 1;
}

// Calculations
static int collide(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 5 && n != 4)
    return luaL_error(L, "Got %d arguments expected 5 or 4 (self, group, start_time, delta_time [, function])", n); 
  Sprite* sprite = checkSprite(L,1);
//  Shape* obj = checkShape(L,2); 
  ShapeGroup* obj = checkShapeGroup(L,2);   
  assert(sprite != 0);    
  assert(obj != 0);      

  real t = luaL_checknumber(L, 3);
  real dt = luaL_checknumber(L, 4);
  
  bool ret = false;
  if (n == 4)
    ret = sprite->collide(obj, t, dt);
  else if (n == 5) {
    lua_pushvalue(L,5);
    LuaCommand cmd(L);
    ret = sprite->collide(obj, t, dt, &cmd);
  }
    
  lua_pushboolean(L, ret);
  
  return 1;
}

static int inside(lua_State* L)
{
  int n = lua_gettop(L);
  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 (self, point)", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);    

  Point2 point = Vector2_pull(L,2);
  
  lua_pushboolean(L, sprite->inside(point, 0, 0, 0));
  
  return 1;
}

// Operations
static int render(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  sprite->draw(worldView());

  return 0;
}

static int update(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3) 
    return luaL_error(L, "Got %d arguments expected 2 (self, start_time, delta_time)", n); 
  Sprite* sprite = checkSprite(L);
  assert(sprite != 0);
  
  sprite->update(luaL_checknumber(L,2), luaL_checknumber(L,3));

  return 0;
}

static int advance(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    return luaL_error(L, "Got %d arguments expected 2 (self, delta_time)", n); 
  Sprite* sprite = checkSprite(L);
  sprite->advance(luaL_checknumber(L,2));

  return 0;
}

static int move(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 3 || n != 2) 
    return luaL_error(L, "Got %d arguments expected 3", n); 
  Sprite* sprite = checkSprite(L);

  if (n == 2)
    sprite->move(Vector2_pull(L, 2));        
  else {
    real x = luaL_checknumber (L, 2); 
    real y = luaL_checknumber (L, 3);      
    sprite->move(Vector2(x,y));    
  }

  return 0;
}

static int accelerate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n == 2) {
    Sprite* sprite = checkSprite(L);    
    assert(sprite != 0);
    real a = luaL_checknumber (L, 2); 
    sprite->accelerate(a);
  }
  else
    luaL_error(L, "Got %d arguments expected 2", n); 
  
  return 0;
}

static int rotate(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments

  if (n != 2)
    return luaL_error(L, "Got %d arguments expected 2 or 3", n); 

  Sprite* sprite = checkSprite(L);    
  assert(sprite != 0);
  real angle = luaL_checknumber (L, 2); 
  sprite->rotate(angle);

  return 0;
}

static int reverse(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  sprite->reverse();

  return 0;
}

static int stop(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    return luaL_error(L, "Got %d arguments expected 1", n); 
  Sprite* sprite = checkSprite(L);
  sprite->stop();

  return 0;
}

static int kill(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    luaL_error(L, "Got %d arguments expected 1", n);   
    
  Sprite* sprite = checkSprite(L);    
  assert(sprite != 0);
  sprite->kill();
  
  return 0;
}

static int onPlan(lua_State *L) 
{
  return 0;
}

// Global sprite functions
static int setShowCollision(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 1) 
    luaL_error(L, "Got %d arguments expected 1", n);   
    
  setShowCollision(checkBool(L,1));
  
  return 0;
}

static int showCollision(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 0) 
    luaL_error(L, "Got %d arguments expected 0", n);   
    
  lua_pushboolean(L, showCollision());
  
  return 1;
}

// Debug
// Just to test performance of call
static int dummy(lua_State *L) 
{
  int n = lua_gettop(L);  // Number of arguments
  if (n != 2) 
    luaL_error(L, "Got %d arguments expected 2", n);   

  for (int i=0; i<1000; ++i) {
    Sprite* sprite = checkSprite(L,1);    
    assert(sprite != 0);
    sprite = checkSprite(L,2);      
    assert(sprite != 0);    
  }
  
  return 0;
}

// functions that will show up in our Lua environment
static const luaL_Reg gDestroySpriteFuncs[] = {
  // Destruction
  {"__gc", destroySprite},
  {NULL, NULL}
};

static const luaL_Reg gSpriteFuncs[] = {
  // Creation
  {"new", newSprite},
  {"init", init},
  
  // Accessors
  {"name", name},
  {"setName", setName},
  
  {"position", position},
  {"prevPosition", prevPosition},  
  {"nextPosition", nextPosition},    
  {"setPosition", setPosition}, 
   
  {"setVelocity", setVelocity},
  {"velocity", velocity},    
  {"setSpeed", setSpeed},      
  {"speed", speed},      
  
  {"direction", direction},
  {"setDirection", setDirection},  
  {"setRotation", setRotation},  
  {"rotation", rotation},  
  
  {"setAngularVelocity", setAngularVelocity},  
  {"angularVelocity", angularVelocity},  
  {"setAngularAcceleration", setAngularAcceleration},  
  {"angularAcceleration", angularAcceleration},  

  {"motionState", motionState},
  {"setMotionState", setMotionState},  
  
  // {"prevDirection", prevDirection},  
  {"depth", depth},
  {"setDepth", setDepth},    
  {"polygon", polygon},    
  
  {"setView", setView},    
  {"view", view},      
  {"subViewIndex", subViewIndex},      
  {"setSubViewIndex", setSubViewIndex},      
    
  {"id", id},        
  {"groups", groups},        

  {"setCollisionHandler", setCollisionHandler},
  {"setInsideHandler", setInsideHandler},
  {"setUpdateHandler", setUpdateHandler},
  {"setPlanningHandler", setPlanningHandler},
    
  // Request
  {"visible", visible},
  {"setVisible", setVisible},    
  {"viewCollide", viewCollide}, 
    
  // Calculations
  {"collide", collide},    
  {"inside", inside},     
     
  // Operations
  {"render", render},      
  {"update", update},          
  {"advance", advance},            
  {"move", move},    
  {"accelerate", accelerate},      
  {"rotate", rotate},  
  {"reverse", reverse},    
  {"stop", stop},  
  
  {"private_kill", kill},  
    
  // For all sprites
  {"showCollision", showCollision},  
  {"setShowCollision", setShowCollision},      

  //debug
  {"dummy", dummy},    
  {NULL, NULL}
};

// Initialization
void initLuaSprite(lua_State *L)
{  
  // Metatable to be used for userdata identification
  luaL_newmetatable(L, "Lusion.Sprite");
  luaL_register(L, 0, gDestroySpriteFuncs);      
  luaL_register(L, 0, gSpriteFuncs);      
  lua_pushvalue(L,-1);
  lua_setfield(L,-2, "__index");  
  
  luaL_register(L, "Sprite", gSpriteFuncs);  
  
  initSpriteBookkeeping(L);
}