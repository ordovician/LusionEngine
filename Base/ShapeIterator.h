/*!
  \file ShapeIterator.h
  \author Erik Engheim - Translusion
  \date 18.10.2007
  
*/

#pragma once

#include "Utils/Iterator.h"

class Shape;
  
typedef Iterator<Shape*>               ShapeIterator;
typedef MutableIterator<Shape*>        MutableShapeIterator;

typedef VectorIterator<Shape*>         VectorShapeIterator;
typedef MutableVectorIterator<Shape*>  MutableVectorShapeIterator;

typedef SetIterator<Shape*>            SetShapeIterator;