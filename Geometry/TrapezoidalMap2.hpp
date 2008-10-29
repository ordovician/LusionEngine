/******************************************************************
Name	: TrapezoidalMap2
Author	: Erik Engheim
Date	: 07/09/2005
Desc	: Describes a 2d line segment. 
Comment	: This class has not been tested yet.

Copyright 2007 Translusion. All rights reserved.
*******************************************************************/

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
