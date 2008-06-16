/*
 *  MockView.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Base/View.h"

class MockView : public View
{
public:
  // Constructors
  MockView( const Polygon2& poly);
  MockView();
  virtual ~MockView();

  // Calculations
	virtual void draw(const Point2& pos, real rot, int image_index = 0) const;
  
};