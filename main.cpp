/*
 *  main.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#include "Engine.h"
#include <Core/AutoreleasePool.hpp>

int main(int argc, char *argv[])
{  
  AutoreleasePool::begin();
    engineInit();
    engineEventLoop();
    engineCleanup();
  AutoreleasePool::end();
          
  return 0;
}
