/*
 *  Engine.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#include "Engine.h"
#include "Base/Sprite.h"
#include "Lua/Base/LuaSprite.h"
#include "LuaEngine.h"
#include "Base/ImageView.h"
#include "Base/Group.h"
#include "Base/PolygonView.h"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

#include <Core/SharedObject.hpp>
#include <Core/AutoreleasePool.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

// #define PRINT_AVG_LOOP_TIME

using namespace std;

// private member variables
// Timer
static const int RUN_GAME_LOOP = 1;
static const int ENGINE_STATS = 2;
static SDL_TimerID gTimer;
static int gStartTime = 0;

static bool       gDone;

// Key and mouse state
static real gMouseX = 0.0;
static real gMouseY = 0.0;
static bool gMouseButton = false; // True if left mouse button down

// Viewport
static int gViewportWidth = 640;
static int gViewportHeight = 640;

// View
static Rect2  gView(-20.0, -20.0, 20.0, 20.0);

static bool gGLInitialized = false;

// Statistics
static real gAverageLoopTime = 0.0;



// private functions
template <typename ForwardIterator>
void deleteAll(ForwardIterator begin, ForwardIterator end)
{
    while (begin != end) {
        delete *begin;
        ++begin;
    }    
}

template <typename Container>
inline void deleteAll(const Container &c)
{
    deleteAll(c.begin(), c.end());
}

// Game related functions
static void renderFrame(real start_time) 
{
    glClear(GL_COLOR_BUFFER_BIT);
    renderGroup()->draw(worldView());
    // luaRenderFrame(start_ticks); // don't see a need to be able to control this form lua
    SDL_GL_SwapBuffers();
}

static void handleKeys()
{
  Uint8 *keystate = SDL_GetKeyState(0);
  
  gDone = keystate[SDLK_q];
  
  if (keystate[SDLK_d]) debugLua();      
}

static void handleMouse()
{
  int x, y;
  Uint8 state = SDL_GetMouseState(&x, &y);
  real vx = x;
  real vy = gViewportHeight-y;
  gMouseX = (vx/gViewportWidth)*gView.width()+gView.left();
  gMouseY = (vy/gViewportHeight)*gView.height()+gView.bottom();         
  gMouseButton = state & SDL_BUTTON(1);    
}

static void engineLoop(Uint32 start_ticks)
{
  real secs = start_ticks*(1.0/1000.0); 
  handleKeys();
  handleMouse();
  renderFrame(secs);    
  luaUpdate(secs);
  
  AutoreleasePool::currentPool()->releasePool();
}

static void engineStats()
{
  cout << "Average looptime: " << gAverageLoopTime << " ms" << endl;
}



static Uint32 engineLoopTimer(Uint32 interval, void* param)
{
  // Create a user event to call the game loop.
  SDL_Event event;

  event.type = SDL_USEREVENT;
  event.user.code = RUN_GAME_LOOP;
  event.user.data1 = (void *)SDL_GetTicks(); // So we know when event was sent
  event.user.data2 = 0;

  SDL_PushEvent(&event);

  return interval;
}

/*! To collect and print out statistics about engine */
static Uint32 engineStatsTimer(Uint32 interval, void* param)
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

static void handleUserEvents(SDL_Event* event)
{
    Uint32 start_ticks, end_ticks;
    switch (event->user.code) {
        case RUN_GAME_LOOP:
          start_ticks = (Uint32)event->user.data1;
          engineLoop(start_ticks);
          end_ticks = SDL_GetTicks();
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
static void initSDL()
{
  int error;
  SDL_Surface* drawContext;

  error = SDL_Init(SDL_INIT_EVERYTHING);

  // Create a real-buffered draw context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                      
  Uint32 flags;
  flags = SDL_OPENGL;// | SDL_FULLSCREEN;
  drawContext = SDL_SetVideoMode(gViewportWidth, gViewportHeight, 0, flags);
}

static void initGL()
{
#ifdef USE_TEXTURES
  glEnable( GL_TEXTURE_2D );                // Enable texture mapping
  glShadeModel( GL_SMOOTH );                // Enable smooth shading
  // glEnable( GL_DEPTH_TEST );                // Enable depth testing
  // glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
  glBlendFunc( GL_SRC_ALPHA, GL_ONE );
  glEnable( GL_BLEND );
  glDisable( GL_DEPTH_TEST );  
#endif
  glClearColor( 0.0, 0.0, 0.0, 1.0 );   // Black background  

  glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(gView.left(), gView.right(), gView.bottom(), gView.top());  
  glMatrixMode(GL_MODELVIEW);    

  gGLInitialized  = true;
}

static void installTimer()
{
    // Called almost for each frame if we assume 60 frames per second

    gTimer = SDL_AddTimer(ticksPerFrame(), engineLoopTimer, NULL);

  #ifdef PRINT_AVG_LOOP_TIME  
    // Called each 2 sec to show statistics
    gStatTimer = SDL_AddTimer(5000, engineStatsTimer, NULL);        
  #endif    
}


void engineInit()
{   
  gDone = false;
  initLua();  
  initSDL();
  initGL();
#ifdef USE_TEXTURES  
  initTextures();  
#endif
  initGame();
  installTimer();
  
  AutoreleasePool::currentPool()->releasePool();  
}

void engineEventLoop()
{
    SDL_Event event;
    
    while((!gDone) && (SDL_WaitEvent(&event))) {
        switch(event.type) {
            case SDL_USEREVENT:
                handleUserEvents(&event);
                break;
                
            case SDL_QUIT:
                gDone = true;
                break;
                
            default:
                break;
        }   // End switch
            
    }   // End while
        
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

// Accessors
void setViewportHeight(int height)
{
  gViewportHeight = height;
}

void setViewportWidth(int width)
{
  gViewportWidth = width;
}

void setViewport(int width, int height)
{
  gViewportWidth = width;
  gViewportHeight = height;
}

void setWorldView(const Rect2& rect)
{
  gView = rect;
  
  if (gGLInitialized) {
    glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(gView.left(), gView.right(), gView.bottom(), gView.top());  
    glMatrixMode(GL_MODELVIEW);    
  }
}

void lookAt(const Point2& p)
{
  gView.moveCenter(p);
  if (gGLInitialized) {
    glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(gView.left(), gView.right(), gView.bottom(), gView.top());  
    glMatrixMode(GL_MODELVIEW);    
  }
}

Rect2 worldView()
{
  return gView;
}


int ticksLeft(int start_ticks)
{
  return ticksPerFrame() - (SDL_GetTicks()-start_ticks);
}

void startTimer()
{
  gStartTime = SDL_GetTicks();
}

int stopTimer()
{
  return SDL_GetTicks()-gStartTime;
}

bool isMouseButtonDown()
{
  return gMouseButton;
}

Vector2 mousePosition()
{
  return Vector2(gMouseX, gMouseY);
}

// Request
bool stateOfKey(int key)
{
  Uint8 *keystate = SDL_GetKeyState(0);  
  return keystate[key];
}