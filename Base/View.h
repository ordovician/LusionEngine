/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#pragma once

#include "Types.h"

#include <Core/SharedObject.hpp>
#include <Geometry/Polygon2.hpp>

class View : public SharedObject
{
public:
	// Constructors
	View();
	virtual ~View();

	// Accessors
  std::string typeName() const;  
	void setOrigin(const Point2 & aOrigin);
	Point2 origin() const;
			
	void setCollisionPolygon(const Polygon2& poly);

  const Polygon2& collisionPolygon() const;
  Polygon2& collisionPolygon();	
	
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