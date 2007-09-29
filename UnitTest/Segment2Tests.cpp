/*
 *  Segment2Tests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Segment2Tests.h"

#include <Geometry/Segment2.hpp>
#include <Geometry/Rect2.hpp>

Segment2Tests::Segment2Tests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


Segment2Tests::~Segment2Tests()
{
}

void Segment2Tests::testSegmentIntersections()
{
  Segment2 s1(Vector2(-1.0f, 1.0f), Vector2(2.0f, 1.0f));
  Segment2 s2(Vector2(0.0f, 2.0f), Vector2(0.0f, -2.0f));  
  Segment2 s3(Vector2(-1.0f, -2.0f), Vector2(-1.0f, 2.0f));  
  Segment2 s4(Vector2(-1.0f, 0.0f), Vector2(2.0f, 0.0f));  
  Segment2 s5(Vector2(-2.0f, 1.0f), Vector2(-1.0f, 1.0f));  
  Segment2 s6(Vector2(-2.0f, 1.0f), Vector2(0.0f, 1.0f));  
    
  Segment2 s7(Vector2(1.0f, 1.0f), Vector2(4.0f, 4.0f));
  Segment2 s8(Vector2(1.0f, 4.0f), Vector2(4.0f, 1.0f));
    
  Vector2 result(0.0f, 0.0f);
  
  // NOTE: Parallell line segments are defined as not intersecting even when they overlap
  CPTAssert(s1.intersection(s2, result));
  CPTAssert(result == Vector2(0.0f, 1.0f));
  CPTAssert(result != Vector2(0.0f, 0.0f));  
  CPTAssert(s7.intersect(s8));          
  CPTAssert(!s1.intersect(s1)); // Completly Overlapping and parallell  
  CPTAssert(s1.intersect(s2));  // Perpendicular intersect
  CPTAssert(s1.intersect(s3));  // Perpendicular tangenting
  CPTAssert(!s1.intersect(s4)); // Parallell segments 
  CPTAssert(!s1.intersect(s5)); // Tangenting segmen parallell
  CPTAssert(!s1.intersect(s6));  // Parallell overlapping    
}

void Segment2Tests::testRectIntersections()
{
  Rect2 rect(Vector2(0.0f, 0.0f), Vector2(2.0f, 2.0f));
  
  Segment2 s1(Vector2(1.0f, -1.0f), Vector2(1.0f, 3.0f)); // Across vertical
  Segment2 s2(Vector2(-1.0f, 1.0f), Vector2(3.0f, 1.0f)); // Across horizontal
  Segment2 s3(Vector2(-1.0f, 1.0f), Vector2(1.0f, 1.0f)); // Enter from left
  Segment2 s4(Vector2(1.0f, 1.0f), Vector2(3.0f, 1.0f));  // Enter from right
  Segment2 s5(Vector2(2.0f, 1.0f), Vector2(3.0f, 2.0f));  // Tangent left 
  Segment2 s6(Vector2(1.0f, 2.0f), Vector2(1.5f, 3.0f));  // Tangtn top
    
  Segment2 s7(Vector2(0.5f, 0.5f), Vector2(1.5f, 1.0f)); // Diagonal inside
  Segment2 s8(Vector2(0.8f, 0.0f), Vector2(0.9f, 1.0f));    // Tagent inside
  Segment2 s9(Vector2(3.0f, 3.0f), Vector2(4.0f, 4.0f));    // Outside
  Segment2 s10(Vector2(-3.0f, 1.0f), Vector2(-1.0f, 1.0f));    // Outside  
  
  CPTAssert(s1.intersect(rect));
  CPTAssert(s2.intersect(rect));
  CPTAssert(s3.intersect(rect));
  CPTAssert(s4.intersect(rect));
  CPTAssert(s5.intersect(rect));
  CPTAssert(s6.intersect(rect));          
  CPTAssert(!s7.intersect(rect)); // Inside does not count as intersection
  CPTAssert(s8.intersect(rect));
  CPTAssert(!s9.intersect(rect));          
  CPTAssert(!s10.intersect(rect));            
}

void Segment2Tests::testBasics()
{
  Segment2 s1(Vector2(-1.0f, 1.0f), Vector2(2.0f, 1.0f));
  Segment2 s2(Vector2(0.0f, 2.0f), Vector2(0.0f, -2.0f));  
  Segment2 s3(Vector2(-1.0f, -2.0f), Vector2(-1.0f, 2.0f));  
  Segment2 s4(Vector2(-1.0f, -2.0f), Vector2(-1.0f, -2.0f));  

  CPTAssert(!s1.isDegenerate());
  CPTAssert(s4.isDegenerate());  
  CPTAssert(s1.isAbove(Vector2(0.0f, 0.0f)));
  CPTAssert(s1.isBelow(Vector2(0.0f, 2.0f)));

  CPTAssert(!s1.isBelow(Vector2(0.0f, 0.0f)));
  CPTAssert(!s1.isAbove(Vector2(0.0f, 2.0f)));
  
  CPTAssert(s2.isOn(Vector2(0.0f, 1.0f)));    
  CPTAssert(!s2.isOn(Vector2(1.0f, 0.0f))); // Is not on line or segment.     
  CPTAssert(s2.isOn(Vector2(0.0f, 3.0f)));  // Is not on segment but on line defined by it
  CPTAssert(s3.isOnEnd(Vector2(-1.0f, -2.0f)));    
}

void Segment2Tests::testNearest()
{
  Segment2 s1(Vector2(-1.0f, 1.0f), Vector2(2.0f, 1.0f));
  Segment2 s2(Vector2(0.0f, 2.0f), Vector2(0.0f, -2.0f));  
  Segment2 s3(Vector2(0.0f, 0.0f), Vector2(2.0f, 2.0f));  
    
  CPTAssert(s1.nearestPoint(Vector2(0.0f, 0.0f)) == Vector2(0.0f, 1.0f));
  CPTAssert(s2.nearestPoint(Vector2(1.0f, 0.0f)) == Vector2(0.0f, 0.0f));
  CPTAssert(s2.nearestPoint(Vector2(1.0f, 1.0f)) == Vector2(0.0f, 1.0f));
  CPTAssert(s2.nearestPoint(Vector2(1.0f, -2.0f)) == Vector2(0.0f, -2.0f));
  CPTAssert(s3.nearestPoint(Vector2(2.0f, 5.0f)) == Vector2(2.0f, 2.0f));  
  CPTAssert(s3.nearestPoint(Vector2(0.0f, -5.0f)) == Vector2(0.0f, 0.0f));    
}


static Segment2Tests test1(TEST_INVOCATION(Segment2Tests, testSegmentIntersections));
static Segment2Tests test2(TEST_INVOCATION(Segment2Tests, testRectIntersections));
static Segment2Tests test3(TEST_INVOCATION(Segment2Tests, testBasics));
static Segment2Tests test4(TEST_INVOCATION(Segment2Tests, testNearest));