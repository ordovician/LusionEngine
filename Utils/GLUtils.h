/*
 *  GLUtils.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 10.11.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

void gltTranslate(const Point2& pos);
void gltVertex(const Point2& pos);
void gltVertex(const Rect2& rect);
void gltVertex(const Segment2& segment);
void gltVertex(ConstPointIterator2 begin, ConstPointIterator2 end, const Point2& position, real angle);
void gltVertex(ConstPointIterator2 begin, ConstPointIterator2 end);

void drawCircle(double radius);

// Debug
void gltPerformanceTest(int n);
void trigPerformanceTest(int n);