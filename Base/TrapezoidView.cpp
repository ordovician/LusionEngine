/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "Base/TrapezoidView.h"
#include "Geometry/Trapezoid2.hpp"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

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
void TrapezoidView::draw(const Point2& pos, real rot, int) const
{
  assert(iTrapezoid != 0);
  
  glPushMatrix();
    gltTranslate(pos);
    glRotated(rot, 0.0, 0.0, 1.0);

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
  glPopMatrix();   
}