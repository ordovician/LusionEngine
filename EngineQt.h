/*
 *  EngineSDL.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 8.1.09.
 *  Copyright 2009 Translusion. All rights reserved.
 *
 */
 
#pragma once

#include <Geometry/Vector2.hpp>

#include "Types.h"

// private
void swapBuffers();
void installTimer();
uint32 engineStatsTimer(uint32 interval, void* param);
// void handleUserEvents(SDL_Event* event, EngineLoopCallback engineLoop);
void initKeyboard();
void handleMouse(const Rect2& view, int viewportWidth, int viewportHeight);

// public
void setEngineLoopCallback(EngineLoopCallback callback);
void engineEventLoop();
void engineCleanup();

ubyte*  getKeyState();
void    setKeyState(uint32 index, ubyte state);

bool    isMouseButtonDown();
Vector2 mousePosition();
