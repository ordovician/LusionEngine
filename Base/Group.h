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
#include <Base/ShapeListener.h>

// Group used for rendering
Group* renderGroup(); 

class Group : public Shape, public ShapeListener
{
public:
  // Constructors
  Group();
  virtual ~Group();

  // Accessors
  std::string typeName() const;
  Rect2 boundingBox() const;  
  int   noShapes() const;  
  ShapeIterator* iterator() const;
  
  // Request
  bool contains(Shape* shape) const;
  bool isSimple() const;
  
  // Calculations
  bool collide(Shape* other, real t, real dt, CollisionAction* command = 0);
  bool inside(const Point2& p, real t, real dt, Action* command);
  void draw(const Rect2& r) const;
        
  // Operations
  void addKid(Shape* shape);
  void removeKid(Shape* shape);
  void update(real start_time, real delta_time);
  void doPlanning(real start_time, real delta_time);
  void clear();

  Shape* nextShape();
  
  // Even handling
  void shapeDestroyed(Shape* shape);
  void shapeKilled(Shape* shape);
        
private:
  std::set<Shape*> iShapes;
  std::set<Shape*>::iterator iCurShape;
  
  Rect2 iBBox;
};