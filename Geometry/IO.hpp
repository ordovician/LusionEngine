/******************************************************************
Name	: IO
Author: Erik Engheim
Date	: 07/09/2005
Desc	: io functions for geometry classes 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/Rect2.hpp>
#include <iostream>

std::ostream& operator<< ( std::ostream & aOut, const Vector2& aV );
std::ostream& operator<< ( std::ostream & aOut, const Segment2& aS );
std::ostream& operator<< ( std::ostream & aOut, const Rect2& aR );