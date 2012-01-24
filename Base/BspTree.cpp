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

#include "BspTree.h"

// Operators
BspNode* BspTree::createNode(Segments2::iterator begin, Segments2::iterator end) {
  int no_segs = end - begin;
  
  if (no_segs <= 1) 
    return new BspNode(begin, end);

  Segments2::iterator it = begin;
  Segments2 segs_positive, segs_negative, segs_intersect;
  Line2 l(it->origin(), it->direction());
  
  for (it = begin; it != end; ++it) {
    if (l.isPositive(*it)) {
      segs_positive.push_back(*it);
      BspNode* t_positive = createNode(segs_positive);
    }
    else if (l.isNegative(*it)) {
      segs_negative(*it);
      BspNode* t_negative = createNode(segs_negative);            
    }
    else
      segs_intersect.push_back(*it);
  }
  BspNode* t = new BspNode(t_positive, t_negative, segs_intersect);
  
  return t;
}
