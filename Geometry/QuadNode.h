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
#include <Geometry/Rect2.hpp>
#include <Geometry/Circle.hpp>

#include "Types.h"

#include <vector>

typedef std::vector<Circle>::const_iterator ConstCircleIterator;
typedef std::pair<ConstCircleIterator, ConstCircleIterator> ShallowCircles;

class QuadNode
{
public:
  // Constructors
  QuadNode();
  virtual ~QuadNode();

  // Accessors
  ShallowCircles discs() const;
  
  // Request
  bool inside(const Vector2& pos) const;
  bool findNode(const Vector2& pos, QuadNode& node) const;

  // Operations

private:
  Rect2             iBBox;
  std::vector<QuadNode>  iKids;
  std::vector<Circle>    iDiscs;
};
