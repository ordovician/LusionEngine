/*
 *  PolygonView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/PolygonView.h"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

static Point2 gPoints[] = {Point2(-1.0, -1.0), Point2(1.0, 0.0), Point2(-1.0, 1.0)};

PolygonView::PolygonView(const Polygon2& poly, GLenum style)
{
  init(poly, style);
}

PolygonView::PolygonView(GLenum style)
{
  Polygon2 poly(gPoints, gPoints+3);
  init(poly, style);
}

PolygonView::~PolygonView()
{

}

// Accessors
void PolygonView::setDrawStyle(GLenum style)
{
  iStyle = style;
}

GLenum PolygonView::drawStyle() const
{
  return iStyle;
}

// Calculations
void PolygonView::draw(const Point2& pos, real rot, int) const
{
  glPushMatrix();
    gltTranslate(pos);
    glRotated(rot, 0.0, 0.0, 1.0);

    const Polygon2& poly = collisionPolygon();
    glColor3dv(iColor);	
    glBegin(iStyle);  
      gltVertex(poly.begin(), poly.end());  
    glEnd();
  glPopMatrix(); 
}

// Operations
void PolygonView::init( const Polygon2& poly, GLenum style )
{
  setCollisionPolygon(poly);
  iStyle = style;
}
