/*
 *  Engine.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */
#pragma once

#include <SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "Types.h"

class Sprite;

 #define USE_TEXTURES

void engineInit();
void engineEventLoop();
void engineCleanup();

// Accessors
void setViewportHeight(int height);
void setViewportWidth(int width);
void setViewport(int width, int height);
void setWorldView(const Rect2& rect);
void lookAt(const Point2& p);
Rect2 worldView();

int     ticksPerFrame();
void    setTicksPerFrame(int noTicks);
real    secondsPerFrame();
real    secondsPassed();
int     ticksLeft(int start_ticks);
void    startTimer();
int     stopTimer();
bool    isMouseButtonDown();
Vector2 mousePosition();

// Request
bool stateOfKey(int key);