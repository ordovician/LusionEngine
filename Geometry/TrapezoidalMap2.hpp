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

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/Trapezoid2.hpp>
#include <Geometry/Rect2.hpp>
#include <vector>

// Helper functions
Rect2 calcBoundingBox(Segments2::const_iterator begin, Segments2::const_iterator end);
  
class TrapezoidalMap2
{
public:
  // Constructors
  TrapezoidalMap2();
  TrapezoidalMap2(Segments2::const_iterator begin, Segments2::const_iterator end);
  TrapezoidalMap2(Segments2::const_iterator begin, Segments2::const_iterator end, const Rect2& boundingBox);  
  
  virtual ~TrapezoidalMap2();
  
  void init(Segments2::const_iterator begin, Segments2::const_iterator end, const Rect2& boundingBox);
        
  // Calculate
  Trapezoid2* locate(const Point2& p) const;
  Trapezoid2* locate(const Segment2& s) const;
  void getTrapezoids(Trapezoids2& out) const;
  void remove(Trapezoid2* t);

  // Operations  
  int  assignUniqueTags();
  
private:
  // Operations  
  void update(const Segment2& si, Trapezoids2::iterator begin, Trapezoids2::iterator end);  
  void updateMultiple(const Segment2& si, Trapezoids2::iterator begin, Trapezoids2::iterator end);
  void updateSingle(const Segment2& si, Trapezoid2* t);
  
private:
  Trapezoid2* iT;
  TrapezoidNode2* iD;
  Trapezoid2* iDummy;
};
