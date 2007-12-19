/*
 *  ShapeListener.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 12.12.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

class Shape;

class ShapeListener
{
public:
  ShapeListener();
  virtual ~ShapeListener();
  
  virtual void shapeDestroyed(Shape* shape) = 0;
  virtual void shapeKilled(Shape* shape) = 0;
};