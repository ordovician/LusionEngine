/*
 *  View.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include <Core/SharedObject.hpp>

class View : public SharedObject
{
public:
	// Constructors
	View();
	virtual ~View();

	// Accessors
	void setOrigin(const Point2 & aOrigin);
	Point2 origin() const;
			
	void setCollisionPolygon(ConstPointIterator2 begin, ConstPointIterator2 end);

  ShallowPoints2 collisionPolygon() const;
  MutableShallowPoints2 collisionPolygon();	
	
  real radius() const;
	
  void setColor(real red, real green, real blue);
  	
	// Request

	// Calculations
	virtual void draw(const Point2& pos, real rot, int image_index = 0) const = 0;
		  
private:
	Point2    iOrigin;
  real      iRadius;
  Polygon2  iPolygon;	
  
protected:
  real      iColor[3];
};