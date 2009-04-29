/*
 *  PolygonView.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include <Base/View.h>

#include <Geometry/Polygon2.hpp>

//#include <OpenGL/gl.h>
#include <GL/glew.h>

class PolygonView : public View
{
public:
  // Constructors
  PolygonView( const Polygon2& poly, GLenum style = GL_POLYGON );
  PolygonView( GLenum style = GL_POLYGON );
  virtual ~PolygonView();

  // Accessors
  void    setDrawStyle(GLenum style);
  GLenum  drawStyle() const;  

  // Request

  // Calculations
	virtual void draw(const Point2& pos, real rot, int image_index = 0) const;
  
  // Operations
  void init( const Polygon2& poly, GLenum style );
  
private:
  GLenum iStyle;
};
