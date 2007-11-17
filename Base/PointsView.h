/*
 *  PointsView.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Base/View.h"
#include "Types.h"

class PointsView : public View
{
public:
  // Constructors
  PointsView(Points2::iterator begin, Points2::iterator end);
  PointsView();
  virtual ~PointsView();

  // Accessors
  
  // Request

  // Calculations
	virtual void draw(const Point2& pos, real rot, int image_index = 0) const;
  
  
private:
  Points2 iPoints;
};