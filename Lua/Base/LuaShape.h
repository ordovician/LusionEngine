/*
 *  LuaShape.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

struct lua_State;
class Shape;

void initLuaShape(lua_State *L);
Shape *checkShape(lua_State* L, int index=1);
