/*
 *  TrapezoidView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/TrapezoidView.h"
#include "Geometry/Trapezoid2.hpp"
#include "Utils/PolygonUtils.h"

#include <OpenGL/gl.h>

#include <algorithm>

using namespace std;

TrapezoidView::TrapezoidView( Trapezoid2* t ) : iTrapezoid(t)
{
  iColor[0] = 1.0;
  iColor[1] = 0.0;
  iColor[2] = 0.0;  
}

TrapezoidView::TrapezoidView()
{

}

TrapezoidView::~TrapezoidView()
{

}


// Calculations
void TrapezoidView::draw(int) const
{
  assert(iTrapezoid != 0);
  
  glColor3dv(iColor);
  glLineWidth(2.0f);  
  glBegin(GL_LINES);  
    gltVertex(iTrapezoid->top());
    gltVertex(iTrapezoid->bottom());    
  glEnd();
  
  glColor3d(1.0, 1.0, 1.0);	
  
  glPointSize(4.0f);
  glBegin(GL_POINTS);
    gltVertex(iTrapezoid->left());
    gltVertex(iTrapezoid->right());    
  glEnd();
}