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

#include <iostream>

using namespace std;

/*!
  The collision polygon calculated isn't optimal. We do it because it is used
  for drawing. For collision handling it is poor.
  
  \todo Refactor so collision and drawing polygon don't need to be the same
*/
PointsView::PointsView(Points2::iterator begin, Points2::iterator end) : iPoints(begin, end)
{
  // Find rectangular polygon that will enclose all
  // added points
  Rect2 r;
  Points2::iterator it;
  for (it = begin; it != end; ++it) {
    r = r.surround(*it);
  }
  
  Points2 p;
  p.push_back(r.bottomLeft());
  p.push_back(r.bottomRight());
  p.push_back(r.topRight());
  p.push_back(r.topLeft());      
  
  setCollisionPolygon(p.begin(), p.end());
}

PointsView::PointsView()
{

}

PointsView::~PointsView()
{
  cout << hex << "0x" << (int)this << " PointsView removed" << endl;  // DEBUG  
}

static void drawPoint(const Point2& p) {
  gltVertex(p);
}

// Calculations
void PointsView::draw(const Point2& pos, real rot, int) const
{  
  glPushMatrix();
    gltTranslate(pos);
    glRotated(rot, 0.0, 0.0, 1.0); 

    glColor3dv(iColor);  
    glPointSize(4.0f);
    glBegin(GL_POINTS);
      for_each(iPoints.begin(), iPoints.end(), drawPoint);    
    glEnd();
  glPopMatrix();  
}