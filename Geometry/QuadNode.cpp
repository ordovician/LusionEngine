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

#include <Geometry/QuadNode.h>

using namespace std;

// Constructors
QuadNode::QuadNode()
{
  
}

QuadNode::~QuadNode()
{
  
}

// Accessors
ShallowCircles QuadNode::discs() const
{
  return make_pair(iDiscs.begin(), iDiscs.end());
}

// Request
/*!
 True if \a pos is inside node. This does not say anything about whether
 it is inside a child node or not, simple that position exist within this
 node or a childnode.
*/
bool QuadNode::inside(const Vector2& pos) const
{
  return iBBox.inside(pos);
}

/*! 
  Locate child node containing position \a pos. Child node
  is returned in \a node. Method returns false if no node containing
  \a pos was found.
*/
bool QuadNode::findNode(const Vector2& pos, QuadNode& node) const
{
  if (!inside(pos))
    return false;
    
  vector<QuadNode>::const_iterator it;
  for (it = iKids.begin(); it != iKids.end(); ++it) {
    if (it->findNode(pos, node))
      return true;
  }
  return false;
}


