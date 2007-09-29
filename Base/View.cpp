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

#include <OpenGL/gl.h>

#include <iostream>

static Point2 gPoints[] = {Point2(-1.0, -1.0), Point2(1.0, -1.0), Point2(1.0, 1.0), Point2(-1.0, 1.0)};
static int gNextTag = 1;

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
void View::setOrigin(const Point2 & aOrigin)
{
	iOrigin = aOrigin;
}

Point2 View::origin() const
{
	return iOrigin;
}

void View::setCollisionPolygon(ConstPointIterator2 begin, ConstPointIterator2 end)
{
  iPolygon.resize(end-begin);
  copy(begin, end, iPolygon.begin());
  iRadius = for_each(iPolygon.begin(), iPolygon.end(), Longest()).length;  
}

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