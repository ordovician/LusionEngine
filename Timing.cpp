/*
 *  Timing.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 10.11.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Timing.h"

#include <assert.h>
#include <time.h>

static int gTicksPerFrame = 30;  // milliseconds per frame

void setTicksPerFrame(int noTicks)
{
  gTicksPerFrame = noTicks;
}

int ticksPerFrame()
{
  return gTicksPerFrame;
}

real secondsPerFrame()
{
  return gTicksPerFrame*(1.0/1000.0);
}

real secondsPassed()
{
  return clock()*(1.0f/CLOCKS_PER_SEC);
}