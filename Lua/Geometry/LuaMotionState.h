/*
 *  LuaMotionState.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

class MotionState;
struct lua_State;

void initLuaMotionState(lua_State *L);
MotionState  *checkMotionState(lua_State* L, int index = 1);
void MotionState_push(lua_State *L, MotionState* mstate);
