/*
 *  ShapeListener.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 12.12.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "ShapeListener.h"

/*!
 \class ShapeListener
 \brief interface for all objects which want to be notified of changes to shapes
 
 Shapes can be contained within various groups. A Shape could be deleted for different
 reasons. If it is a Sprite it could have been "killed" by an enemy. If this happens
 it should no longer be rendered and should thus not be in the render group anymore.
 
 This highlights the need for a mechanism of notifying other objects of changes to a
 shape. E.g. in the case of sprites we want to notify the render group that the sprite
 has been destroyed so that it can remove it from the group and avoid trying to render
 a dead object. 
 
 ShapeListener is an interface that any object which wants to be notified of changes to
 a shape can implement, such as the event that shape was destroyed.
*/

ShapeListener::ShapeListener()
{
  
}

ShapeListener::~ShapeListener()
{
  
}

