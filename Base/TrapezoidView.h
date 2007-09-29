/*
 *  TrapezoidView.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Base/View.h"

class Trapezoid2;

class TrapezoidView : public View
{
public:
  // Constructors
  TrapezoidView( Trapezoid2* t );
  TrapezoidView();
  virtual ~TrapezoidView();

  // Accessors
  
  // Request

  // Calculations
	virtual void draw(int image_index = 0) const;
  
  
private:
  Trapezoid2* iTrapezoid;
};