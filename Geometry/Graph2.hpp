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
#include <Geometry/Trapezoid2.hpp>

#include <vector>

#include "Types.h"

using namespace std;

class Trapezoid2;

struct EdgeData
{
  int     utag, vtag;
  Point2  upos, vpos;
  real    weight;
  
  Trapezoid2  *trap;
};

/*! Simple class to store data about an edge in roadmap embedded in trapezoidal map */
class EdgePair 
{
public:
  EdgePair();
  EdgePair(Trapezoid2* au, Trapezoid2* av, int atag, const Point2& apos);
  
  EdgeData first() const;
  EdgeData second() const;
  EdgeData edge(int index) const;
  
  // Operators
  bool operator==(const EdgePair& data) const;
  
private:
  int         tag;
  Point2      pos;
  Trapezoid2 *u, *v;
};

typedef vector<EdgePair> EdgePairs;

class Paths2 
{
public:
  virtual bool pathFrom(Trapezoid2* target, Points2& path) const = 0;
  virtual real distanceFrom(Trapezoid2* target) const = 0;  
  virtual void printPathFrom(Trapezoid2* trap) const = 0;
  virtual void printGraph() const = 0;    
};

typedef pair<Point2, Vector2> ChokePoint;
typedef vector<ChokePoint>    ChokePoints;

class Graph2
{
public:
  
  // Creation
  static Graph2* create( size_t noVerticies, EdgePairs::iterator begin, EdgePairs::iterator end);
  
  // Accessors

  // Request

  // Calculations
  virtual Paths2* shortestPaths(Trapezoid2* source) const = 0;
  virtual bool    shortestPath(Trapezoid2* source, Trapezoid2* target, Points2& path) const = 0;  
  virtual bool    fixedLengthPath(Trapezoid2* source, Trapezoid2* target, real distance, Points2& path) const = 0;
  virtual bool    chokePoints(Trapezoid2* start_trap, const Trapezoids2& important_loc, ChokePoints& chokepoints) const = 0;

  virtual void    printGraph() const = 0;
  
  // Operations
    
};

