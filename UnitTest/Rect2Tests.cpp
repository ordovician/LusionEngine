/*
 *  Rect2Tests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Rect2Tests.h"

#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>

Rect2Tests::Rect2Tests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


Rect2Tests::~Rect2Tests()
{
}


void Rect2Tests::testBasics()
{
  Rect2 rect(Point2(0.0f, 0.0f), Point2(10.0f, 10.0f));

  CPTAssert(rect.min() == Point2(0.0f, 0.0f) && rect.max() == Point2(10.0f, 10.0f));  
}

void Rect2Tests::testSegmentIntersections()
{

}

void Rect2Tests::testRectIntersections()
{
  Rect2 r1(Point2(0.0f, 0.0f), Point2(10.0f, 10.0f));
  Rect2 r2(Point2(10.0f, 10.0f), Point2(20.2f, 20.2f));  
  
//  CPTAssert(r2.halfSize() == Vector2(5.1f, 5.1f));
//  CPTAssert(r2.center() == Vector2(15.0f, 15.0f));  
}



    
static Rect2Tests test1(TEST_INVOCATION(Rect2Tests, testBasics));
static Rect2Tests test2(TEST_INVOCATION(Rect2Tests, testSegmentIntersections));
static Rect2Tests test3(TEST_INVOCATION(Rect2Tests, testRectIntersections));
