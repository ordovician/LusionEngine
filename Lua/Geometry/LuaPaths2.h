/*
 *  LuaPaths2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

struct lua_State;
class Paths2;

void initLuaPaths2(lua_State *L);

void    Paths2_push(lua_State *L, Paths2* t);
Paths2 *checkPaths2(lua_State* L, int index=1);
void    pushTable(lua_State* L, Paths2* p);