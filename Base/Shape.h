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
#include <Geometry/Rect2.hpp>
#include <Geometry/Circle.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/Polygon2.hpp>

#include <Core/SharedObject.hpp>

#include <Base/ShapeIterator.h>
#include <Base/ShapeListener.h>

#include <set>

class CollisionAction;
class Action;

class Shape : public SharedObject
{
public:
  // Constructors
  Shape();
  virtual ~Shape();

  // Accessors
  std::string typeName() const;
  virtual Rect2 boundingBox() const = 0;  
  virtual int   noShapes() const;  
  virtual ShapeIterator* iterator() const;

	void setDepth(int aDepth);
	int depth() const;
    
  // Request
  virtual bool isSimple() const;
  virtual bool collide(Shape* other, real t, real dt, CollisionAction* action = 0) = 0;  
  virtual bool inside(const Point2& p, real t, real dt, Action* action = 0) = 0;
    
  // Calculations
  virtual bool intersection(const Circle& c, Points2& points) const;
  virtual bool intersection(const Rect2& r, Points2& points) const;
  virtual bool intersection(const Segment2& s, Points2& points) const;
  virtual bool intersection(const Polygon2& poly, Points2& points) const;      
     
  virtual void draw(const Rect2& r) const;
   
  // Operations
  virtual void addKid(Shape* shape);
  virtual void removeKid(Shape* shape);

  virtual void update(real start_time, real delta_time);  
  virtual void handleCollision(Shape* other, Points2& points, real start_time, real delta_time);    
  virtual void doPlanning(real start_time, real delta_time);  
  
  void addListener(ShapeListener* listener);
  void removeListener(ShapeListener* listener);
  void removeAllListeners();
  void kill();
  
private:
  int                      iDepth;
  std::set<ShapeListener*> iListeners;   
};