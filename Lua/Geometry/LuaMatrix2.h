/*
 *  LuaMatrix2.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

class Matrix2;
struct lua_State;

void initLuaMatrix2(lua_State *L);
Matrix2 *checkMatrix2(lua_State* L, int index=1);
void Matrix2_push(lua_State *L, int index, Matrix2* mstate);
void Matrix2_push(lua_State *L, Matrix2* mstate);