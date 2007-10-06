/*
 *  CollisionObject.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/CollisionObject.h"
#include <iostream>

using namespace std;

/*!
   \class CollisionObject CollisionObject.h
   \brief A node in the scene graph. 

   The scene graph is used for rendering. The node is also used for collision
   detection.

*/
 
// Helper functions

// Constructors
CollisionObject::CollisionObject()
{
  
}

CollisionObject::~CollisionObject()
{
  // cout << hex << "0x" << (int)this << " CollisionObject removed" << endl;  // DEBUG    
}

// Accessors

// Request
/*! Returns true if this object is a simple non recursive collision object */
bool CollisionObject::isSimple() const
{
  return true;
}

// Operations
