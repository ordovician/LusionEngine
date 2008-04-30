/*
 *  GLUtils.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 10.11.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Utils/GLUtils.h"
#include "Utils/PolygonUtils.h"

#include <OpenGL/gl.h>

void gltTranslate(const Point2& pos)
{  
  glTranslated(pos.x(), pos.y(),0.0);
}

void gltVertex(const Point2& pos)
{  
  glVertex2d(pos.x(), pos.y());
}

void gltVertex(const Rect2& rect)
{
  glVertex2d(rect.xmin(), rect.ymin());
  glVertex2d(rect.xmax(), rect.ymin());
  glVertex2d(rect.xmax(), rect.ymax());
  glVertex2d(rect.xmin(), rect.ymax());                    
}

void gltVertex(const Segment2& segment)
{
  gltVertex(segment.source());
  gltVertex(segment.target());
}

/*! 
 Draws polygon in current OpenGL context with current color. 
 angle is given in degrees (not rad).
*/
void gltVertex(ConstPointIterator2 begin, ConstPointIterator2 end, const Point2& pos, double angle)
{
  glPushMatrix();
    gltTranslate(pos);
    glRotated(angle, 0.0, 0.0, 1.0);
    gltVertex(begin, end);
  glPopMatrix();  
}

/*! 
 Draws polygon in current OpenGL context with current color. 
*/
void gltVertex(ConstPointIterator2 begin, ConstPointIterator2 end)
{
  Points2::const_iterator p = begin;
  while (p != end) {
    gltVertex(*p);
    ++p;        
  }
}

void drawCircle(double r)
{
  glPushMatrix(); 

  glColor3d(1.0, 0.0, 1.0);      
  glBegin(GL_LINE_LOOP);
  for (double angle=0.0; angle<360.0; angle+=15.0) {
    glVertex2d(cos(rad(angle))*r, sin(rad(angle))*r);
  }
  glEnd();
  glPopMatrix();       
}

void draw(const Rect2& box)
{
  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_LINE_LOOP);
    gltVertex(box);
  glEnd();
}

void draw(const Points2& poly)
{
  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_POLYGON);  
    gltVertex(poly.begin(), poly.end());
  glEnd();
}

// Debug
void gltPerformanceTest(int n)
{
  double m[16];
  GLenum matrixMode;
  glGetIntegerv(GL_MATRIX_MODE, (GLint*)&matrixMode);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX, m);
 
  for (int i=0; i<n; ++i) {
    glRotated(40, 0,0,1);
    glGetDoublev(GL_MODELVIEW_MATRIX, m);    
    glMultMatrixd(m);
  }
  glPopMatrix();  
  glMatrixMode(matrixMode);
}

void trigPerformanceTest(int n)
{
  double m[16];
  GLenum matrixMode;
  glGetIntegerv(GL_MATRIX_MODE, (GLint*)&matrixMode);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();    
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX, m);
    
  double s, c;
  double angle = 0.15;

  s = sin(angle);
  c = cos(angle);      

  
  for (int i=0; i<n; ++i) {

    // glGetDoublev(GL_MODELVIEW_MATRIX, m);    
    // m[0] = c;
    // m[1] = s;
    // m[4] = -s;
    // m[5] = c;
    // glLoadMatrixd(m);    
  }
  
  glPopMatrix();  
  glMatrixMode(matrixMode);
}