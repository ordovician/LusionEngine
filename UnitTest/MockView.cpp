/*
 *  MockView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "UnitTest/MockView.h"
#include "Utils/PolygonUtils.h"

static Point2 gPoints[] = {Point2(-1.0, -1.0), Point2(1.0, 0.0), Point2(-1.0, 1.0)};

/*!
  MockView is used for unit testing Sprite class. For sprite class to work properly
  it needs a view object, this is provided by the MockView
*/

MockView::MockView(ConstPointIterator2 begin, ConstPointIterator2 end)
{
  setCollisionPolygon(begin, end);
}

MockView::MockView()
{
  Points2 poly(gPoints, gPoints+3);
  setCollisionPolygon(poly.begin(), poly.end());
}

MockView::~MockView()
{

}

// Calculations
void MockView::draw(const Point2& pos, real rot, int) const
{
}
