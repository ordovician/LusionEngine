/*
 *  Shape.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Shape.h"
#include <iostream>

using namespace std;

/*!
   \class CircleShape 
   \brief  

*/
 
// Constructors
CircleShape::CircleShape()
{
  
}


// Accessors

// Request
bool CircleShape::intersect(Shape* obj) const
{
  if (!obj->isSimple())
    return obj->intersect(this);
    
  CircleShape* c = dynamic_cast<CircleShape*>(obj);
  if (c) 
    return 
  
  RectShape2* r = dynamic_cast<RectShape2*>(obj);  
  SegmentShape2* s = dynamic_cast<SegmentShape2*>(obj);    
}