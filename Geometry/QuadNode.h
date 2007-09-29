/******************************************************************
Name    : Graph2
Author  : Erik Engheim
Date    : 
Desc    : 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Circle.hpp>

#include "Types.h"

#include <vector>

typedef vector<Circle>::const_iterator ConstCircleIterator;
typedef pair<ConstCircleIterator, ConstCircleIterator> ShallowCircles;

using namespace std;

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
  vector<QuadNode>  iKids;
  vector<Circle>    iDiscs;
};
