/*
 *  EngineSDL.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 8.1.09.
 *  Copyright 2009 Translusion. All rights reserved.
 *
 */

#include "Engine.h"
#include "LuaEngine.h"

#include <iostream>

using namespace std;

static SDL_TimerID gTimer;
static EngineLoopCallback engineLoop = 0;

static const int RUN_GAME_LOOP = 1;
static const int ENGINE_STATS = 2;

// Key and mouse state
static real gMouseX = 0.0;
static real gMouseY = 0.0;
static bool gMouseButton = false; // True if left mouse button down

// Statistics
static real gAverageLoopTime = 0.0;

static void engineStats()
{
  cout << "Average looptime: " << gAverageLoopTime << " ms" << endl;
}

static uint32 engineLoopTimer(uint32 interval, void* param)
{
  // Create a user event to call the game loop.
  SDL_Event event;

  event.type = SDL_USEREVENT;
  event.user.code = RUN_GAME_LOOP;
  event.user.data1 = (void *)getTicks(); // So we know when event was sent
  event.user.data2 = 0;

  SDL_PushEvent(&event);

  return interval;
}

// private
void swapBuffers() {
  SDL_GL_SwapBuffers();
}
  
void installTimer()
{
    // Called almost for each frame if we assume 60 frames per second

    gTimer = SDL_AddTimer(ticksPerFrame(), engineLoopTimer, NULL);

  #ifdef PRINT_AVG_LOOP_TIME  
    // Called each 2 sec to show statistics
    gStatTimer = SDL_AddTimer(5000, engineStatsTimer, NULL);        
  #endif    
}

/*! To collect and print out statistics about engine */
uint32 engineStatsTimer(uint32 interval, void* param)
{
  // Create a user event to call the game loop.
  SDL_Event event;

  event.type = SDL_USEREVENT;
  event.user.code = ENGINE_STATS;
  event.user.data1 = 0;
  event.user.data2 = 0;

  SDL_PushEvent(&event);

  return interval;
}

void handleUserEvents(SDL_Event* event, EngineLoopCallback engineLoop)
{
    uint32 start_ticks, end_ticks;
    switch (event->user.code) {
        case RUN_GAME_LOOP:
          start_ticks = (uint32)event->user.data1;
          engineLoop(start_ticks);
          end_ticks = getTicks();
          gAverageLoopTime = (gAverageLoopTime+end_ticks-start_ticks)/2.0;
          break;
        case ENGINE_STATS:
          engineStats();
          break;
        default:
            break;
    }
}

// Inititialization
void initSDL(int gViewportWidth, int gViewportHeight)
{
  int error;
  SDL_Surface* drawContext;

  error = SDL_Init(SDL_INIT_EVERYTHING);

  // Create a real-buffered draw context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                      
  uint32 flags;
  flags = SDL_OPENGL;// | SDL_FULLSCREEN;
  drawContext = SDL_SetVideoMode(gViewportWidth, gViewportHeight, 0, flags);
}

void handleMouse(const Rect2& view, int viewportWidth, int viewportHeight)
{
  int x, y;
  ubyte state = SDL_GetMouseState(&x, &y);
  real vx = x;
  real vy = viewportHeight-y;
  gMouseX = (vx/viewportWidth)*view.width()+view.left();
  gMouseY = (vy/viewportHeight)*view.height()+view.bottom();         
  gMouseButton = state & SDL_BUTTON(1);    
}

// public
void setEngineLoopCallback(EngineLoopCallback callback) {
  engineLoop = callback;
}

void engineCleanup()
{
    SDL_bool success;
    success = SDL_RemoveTimer(gTimer);
  #ifdef PRINT_AVG_LOOP_TIME  
    success = SDL_RemoveTimer(gStatTimer);
  #endif
    closeLua();
    
    SDL_Quit();
}

void engineEventLoop()
{
    SDL_Event event;
    
    while(!isDone() && (SDL_WaitEvent(&event))) {
        switch(event.type) {
            case SDL_USEREVENT:
                handleUserEvents(&event, engineLoop);
                break;
                
            case SDL_QUIT:
                setDone(true);
                break;
                
            default:
                break;
        }   // End switch
            
    }   // End while
        
}

ubyte*  getKeyState() {
  return SDL_GetKeyState(0);
}

bool isMouseButtonDown()
{
  return gMouseButton;
}

Vector2 mousePosition()
{
  return Vector2(gMouseX, gMouseY);
}
