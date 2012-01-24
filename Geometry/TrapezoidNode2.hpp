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

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/TrapezoidNode2.hpp>
#include <Geometry/Trapezoid2.hpp>

#include <vector>
#include <string>

/**
  A node in a tree used a a search tree for trapezoids in a trapezoidal map.
*/
class TrapezoidNode2
{
public:
    // Constructors
    TrapezoidNode2();
    virtual ~TrapezoidNode2();
    
    // Accessors
    TrapezoidNode2* above() const { return left(); }
    TrapezoidNode2* below() const { return right(); }

    void setAbove(TrapezoidNode2* left) { setLeft(left); }
    void setBelow(TrapezoidNode2* right) { setRight(right); }
        
    TrapezoidNode2* left() const;
    TrapezoidNode2* right() const;    

    void setLeft(TrapezoidNode2* left);
    void setRight(TrapezoidNode2* right);

    std::vector<TrapezoidNode2*> children() const;
    TrapezoidNode2* child(int index) const;
    void setChild(int index, TrapezoidNode2* child);
        
    virtual Trapezoid2* trapezoid() const;    
        
    // Calculations
    virtual TrapezoidNode2* locate(const Point2& p);
    virtual TrapezoidNode2* locate(const Segment2& s);        

    virtual TrapezoidNode2* find(const Point2& p) = 0;    
    virtual TrapezoidNode2* find(const Segment2& s) = 0;
    
    void getTrapezoids(Trapezoids2& out) const;
    
    // Operations
    virtual void replaceWith(TrapezoidNode2* n) = 0;
    
    // Debug
    int tag() const;
    void printTree() const;
    virtual std::string description() const = 0;

    
public:
    static void beginDelete();
    static void endDelete();
    
protected:
    TrapezoidNode2 *iKids[2];

    // Debug
    int iTag;
};

typedef std::vector<TrapezoidNode2*> TrapezoidNodes2;

TrapezoidNode2* newNode(const Segment2& s);
TrapezoidNode2* newNode(const Point2& p);
TrapezoidNode2* newNode(Trapezoid2* t);
TrapezoidNode2* newNode(TrapezoidNode2* n);