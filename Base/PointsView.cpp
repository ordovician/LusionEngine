/*
 *  PointsView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/PointsView.h"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

#include <Geometry/Vector2.hpp>

#include <OpenGL/gl.h>
#include <algorithm>

using namespace std;

PointsView::PointsView(Points2::iterator begin, Points2::iterator end) : iPoints(begin, end)
{
}

PointsView::PointsView()
{

}

PointsView::~PointsView()
{

}

static void drawPoint(const Point2& p) {
  gltVertex(p);
}

// Calculations
void PointsView::draw(int) const
{  
  glColor3dv(iColor);  
  glPointSize(4.0f);
  glBegin(GL_POINTS);
    for_each(iPoints.begin(), iPoints.end(), drawPoint);    
  glEnd();
}