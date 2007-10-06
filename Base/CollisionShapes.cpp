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
   \class CollisionCircle 
   \brief  

*/
 
// Constructors
CollisionCircle::CollisionCircle()
{
  
}


// Accessors

// Request
bool CollisionCircle::intersect(CollisionObject* obj) const
{
  if (!obj->isSimple())
    return obj->intersect(this);
    
  CollisionCircle* c = dynamic_cast<CollisionCircle*>(obj);
  if (c) 
    return 
  
  CollisionRect2* r = dynamic_cast<CollisionRect2*>(obj);  
  CollisionSegment2* s = dynamic_cast<CollisionSegment2*>(obj);    
}