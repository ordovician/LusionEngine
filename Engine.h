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

#include <Geometry/Vector2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>

#include "Timing.h"
#include "Types.h"

class Shape;
class Sprite;
class CircleShape;
class RectShape2;
class SegmentShape2;
class View;

 #define USE_TEXTURES

void engineInit();
void engineEventLoop();
void engineCleanup();

// Constructors
Sprite*        createSprite();
Sprite*        createSprite(View* view);
Sprite*        createSprite(const Point2& aPos, real aDir, real aSpeed);
CircleShape*   createCircle(const Circle& circle);
RectShape2*    createRect2(const Rect2& rect);
SegmentShape2* createSegment2(const Segment2& seg);

// Accessors
void setViewportHeight(int height);
void setViewportWidth(int width);
void setViewport(int width, int height);
void setWorldView(const Rect2& rect);
void lookAt(const Point2& p);
Rect2 worldView();

int     ticksLeft(int start_ticks);
void    startTimer();
int     stopTimer();
bool    isMouseButtonDown();
Vector2 mousePosition();

// Request
bool stateOfKey(int key);