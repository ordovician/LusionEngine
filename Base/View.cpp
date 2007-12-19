/*
 *  View.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#include "Base/View.h"
#include "Utils/PolygonUtils.h"

#include <iostream>

static Point2 gPoints[] = {Point2(-1.0, -1.0), Point2(1.0, -1.0), Point2(1.0, 1.0), Point2(-1.0, 1.0)};
static int gNextTag = 1;

/*!
    \class View View.h
    \brief Abstract base class to be implemented by all views

    In Lusion Engine the main objects in the game are either Shapes or Views. Shapes/Sprites
    keep track of where they are in space, if they collide etc. However they don't know
    how to draw themselves, to do that they need to be provided a View. Together a Shape
    and a View form a composite that can be drawn and moved around on screen. 
    
    The strategy pattern is used to compose them. The View is a strategy for a Sprite
    that defines how to draw a sprite. 
    
    The benefit of this is that you can construct a View object at runtime, defining a
    particular shape, e.g. a Space ship that can then be shared by different sprites wich
    are all located at different positions on screen going in different directions. 
    
    If this had not been done then all the points or pixels defining a View would have had
    to be duplicated across all Sprites that looked the same. 
    
    Since a View can be both a bitmap image and a polygon, the visual representation of it
    can not be used for collision detection as well. Or rather doing so could be overly
    computational expensive. So simpler polygons are used to define the view for collision
    purposes. 
*/

// Constructors
View::View() 
  : iOrigin(0.0, 0.0)
{
  iColor[0] = 0.0;
  iColor[1] = 1.0;  
  iColor[2] = 0.0;    
  Polygon2 poly(gPoints, gPoints+4);
  setCollisionPolygon(poly.begin(), poly.end());
 
  // cout << hex << "0x" << (int)this << " view created" << endl;  // DEBUG
  setTag(gNextTag++); // DEBUG
}

View::~View()
{
  cout << "tag: " << tag() << endl;
  cout << hex << "0x" << (int)this << " view removed" << endl;  // DEBUG
}

// Accessors
std::string
View::typeName() const
{
  return "View";
}

void View::setOrigin(const Point2 & aOrigin)
{
	iOrigin = aOrigin;
}

Point2 View::origin() const
{
	return iOrigin;
}

/*!
  \see collisionPolygon
*/
void View::setCollisionPolygon(ConstPointIterator2 begin, ConstPointIterator2 end)
{
  iPolygon.resize(end-begin);
  copy(begin, end, iPolygon.begin());
  iRadius = for_each(iPolygon.begin(), iPolygon.end(), Longest()).length;  
}

/*!
  The shape representing the View can be quite elaborate and using directly for
  collision calculations can thus be computationally expensive. A CollisionPolygon
  is a simpler representation of the view optimized for collision testing. 
  
  Subclasses should create and set collisionPolygon if possible.
*/
ShallowPoints2 View::collisionPolygon() const
{
  return make_pair(iPolygon.begin(), iPolygon.end());
}

MutableShallowPoints2 View::collisionPolygon()
{
  return make_pair(iPolygon.begin(), iPolygon.end());
}


real View::radius() const
{
  return iRadius;
}

void View::setColor(real red, real green, real blue)
{
  iColor[0] = red;
  iColor[1] = green;
  iColor[2] = blue;    
}