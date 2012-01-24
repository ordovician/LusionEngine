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

#include <Base/Shape.h>

class ShapeGroup : public Shape
{
public:
  // Constructors
  ShapeGroup();
  ShapeGroup(ShapeIterator* iterator);
  ShapeGroup(std::vector<Shape*>::iterator begin, std::vector<Shape*>::iterator end);
  ShapeGroup(Shape *left, Shape *right);  
  ShapeGroup(Shape *left, Shape *right, const Rect2& box);
  
  virtual ~ShapeGroup();

  // Accessors
  std::string typeName() const;
  Rect2 boundingBox() const;
  int   noShapes() const;  
  ShapeIterator* iterator() const;
    
  // Request
  bool isSimple() const;
  
  // Calculations
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);  
  bool inside(const Point2& p, real t, real dt, Action* command = 0);
  void draw(const Rect2& r) const;
  
  // Operations
  void update(real start_time, real delta_time);  
  
  void init();
  void init(std::vector<Shape*>::iterator begin, std::vector<Shape*>::iterator end);
  void init(Shape *left, Shape *right);
  void init(Shape *left, Shape *right, const Rect2& box);
  Shape* buildBranch(std::vector<Shape*>::iterator begin, std::vector<Shape*>::iterator end, int axis);
  
private:
  Shape *iLeft, *iRight;
  Rect2 iBox;                 // Bounding box
};