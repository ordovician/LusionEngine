/*
 *  LuaTrapezoid2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

struct lua_State;
class Trapezoid2;

void initLuaTrapezoid2(lua_State *L);

Trapezoid2* Trapezoid2_pull(lua_State *L, int index);
void Trapezoid2_push(lua_State *L, Trapezoid2* t);
Trapezoid2 *checkTrapezoid2(lua_State* L, int index=1);
void pushTable(lua_State* L, Trapezoid2* t);