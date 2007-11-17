/*
 *  PolygonView.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Base/View.h"
#include <OpenGL/gl.h>

class PolygonView : public View
{
public:
  // Constructors
  PolygonView( ConstPointIterator2 begin, ConstPointIterator2 end, GLenum style = GL_POLYGON );
  PolygonView( GLenum style = GL_POLYGON );
  virtual ~PolygonView();

  // Accessors
  void    setDrawStyle(GLenum style);
  GLenum  drawStyle() const;  

  // Request

  // Calculations
	virtual void draw(const Point2& pos, real rot, int image_index = 0) const;
  
  // Operations
  void init( ConstPointIterator2 begin, ConstPointIterator2 end, GLenum style );
  
private:
  GLenum iStyle;
};