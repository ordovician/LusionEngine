/*
 *  LuaGraph2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

struct lua_State;

class Graph2;

void initLuaGraph2(lua_State *L);
Graph2  *checkGraph2(lua_State* L, int index = 1);
