/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
 
#pragma once

#include <SDL/SDL.h>

#include <Geometry/Vector2.hpp>

#include "Types.h"

// private
void swapBuffers();
void installTimer();
uint32 engineStatsTimer(uint32 interval, void* param);
void handleUserEvents(SDL_Event* event, EngineLoopCallback engineLoop);
void initSDL(int gViewportWidth, int gViewportHeight);
void handleMouse(const Rect2& view, int viewportWidth, int viewportHeight);

// public
void setEngineLoopCallback(EngineLoopCallback callback);
void engineEventLoop();
void engineCleanup();

ubyte*  getKeyState();

bool    isMouseButtonDown();
Vector2 mousePosition();
