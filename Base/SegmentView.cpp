/*
 *  SegmentView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/SegmentView.h"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

#include <OpenGL/gl.h>

#include <algorithm>

using namespace std;

SegmentView::SegmentView(Segments2::iterator begin, Segments2::iterator end )
  : iSegments(begin, end),  iShowArrowHead(false)
{
}

SegmentView::SegmentView() : iShowArrowHead(false)
{

}

SegmentView::~SegmentView()
{

}

// Request
void SegmentView::setArrowVisible(bool visible)
{
  iShowArrowHead = visible;
}

bool SegmentView::arrowVisible() const
{
  return iShowArrowHead;
}


// Operations
static void drawSegment(const Segment2& s) {
  gltVertex(s);  
}

static void drawArrowHead(const Segment2& s)
{
  Vector2 v  = s.toVector();
  Vector2 u = v.unit()*0.25;
  Point2 p1 = s.target()-u*2.0+Point2(-u.y(), u.x());
  Point2 p2 = s.target()-u*2.0+Point2(u.y(), -u.x());  

  gltVertex(p1);  
  gltVertex(s.target());    
  gltVertex(p2);
}

// Calculations
void SegmentView::draw(const Point2& pos, real rot, int) const
{
  glPushMatrix();
    gltTranslate(pos);
    glRotated(rot, 0.0, 0.0, 1.0);

    glColor3dv(iColor);	
    glBegin(GL_LINES);  
      for_each(iSegments.begin(), iSegments.end(), drawSegment);
    glEnd();
    if (iShowArrowHead) {
      glBegin(GL_TRIANGLES);
        for_each(iSegments.begin(), iSegments.end(), drawArrowHead);  
      glEnd();    
    }
  glPopMatrix(); 
}

// Operations
void SegmentView::addSegment(const Segment2& aSeg)
{
  iSegments.push_back(aSeg);
}