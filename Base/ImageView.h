/*
 *  ImageView.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Base/View.h"

#include <OpenGL/gl.h>

typedef std::vector<GLuint> UIntVector;
 
void    initTextures();
GLuint  loadTexture(const char* filename);

class ImageView : public View
{
public:
  // Constructors
  ImageView( const StringList& filenames, real width = 2.0, real height = 2.0);  
  ImageView( const StringList& filenames, const Points2& poly, real width = 2.0, real height = 2.0);  
  virtual ~ImageView();

  // Accessors

  // Request
  
  // Calculations
	virtual void draw(const Point2& pos, real rot, int image_index = 0) const;

  // Operations
  void init(const StringList& filenames, real width, real height);
  
private:
  UIntVector  iTextures;  

  Point2 iBottomLeft;
  Point2 iBottomRight;
  Point2 iTopRight;
  Point2 iTopLeft;  
};