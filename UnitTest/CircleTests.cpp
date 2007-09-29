/*
 *  CircleTests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "CircleTests.h"

#include <Geometry/Vector2.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>

CircleTests::CircleTests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


CircleTests::~CircleTests()
{
}


void CircleTests::testCircleIntersect()
{
  Circle c1(Vector2(1.0f, 1.0f), 4.0f);
  Circle c2(Vector2(2.0f, 1.0f), 4.0f);
  Circle c3(Vector2(10.0f, 1.0f), 4.0f);  
  Circle c4(Vector2(9.0f, 1.0f), 4.0f);  
    
  CPTAssert(c1.intersect(c2));  
  CPTAssert(!c1.intersect(c3));    
  CPTAssert(!c1.intersect(c4));    // Make sure circle is not defined on border  
}

void CircleTests::testRectIntersect()
{
  Rect2 rect(Point2(0.0f, 0.0f), Point2(10.0f, 10.0f));
  Circle c1(Vector2(5.0f, 5.0f), 2.0f);   // Clearly inside
  Circle c2(Vector2(5.0f, 5.0f), 5.0f);   // Tagents inside
  Circle c3(Vector2(5.0f, 5.0f), 6.0f);   // Clearly intersect
  Circle c4(Vector2(12.0f, 5.0f), 2.0f);  // Tagents outside
  Circle c5(Vector2(12.0f, 5.0f), 1.0f);  // Outside
  Circle c6(Vector2(15.0f, 15.0f), 5.0f); // Would tagent in upper right corner if rect

  // Circles placed at all the corners of rectangle
  Circle c7(Vector2(15.0f, 15.0f), 7.0f);  // upper right
  Circle c8(Vector2(-5.0f, -5.0f), 7.0f);  // lower left
  Circle c9(Vector2(-5.0f, 15.0f), 7.0f);  // upper left
  Circle c10(Vector2(15.0f, -5.0f), 7.0f); // lower right
      
  CPTAssert(c1.intersect(rect));  // Inside but not intersecting
  CPTAssert(c2.intersect(rect));  // Tagents inside but not intersecting
  CPTAssert(c3.intersect(rect));  
  CPTAssert(!c4.intersect(rect));  
  CPTAssert(!c5.intersect(rect));          
  CPTAssert(!c6.intersect(rect));            
  CPTAssert(!c7.intersect(rect));
  CPTAssert(!c8.intersect(rect));
  CPTAssert(!c9.intersect(rect));
  CPTAssert(!c10.intersect(rect));  
}

void CircleTests::testSegmentIntersect()
{
  Circle circle(Vector2(1.0f, 1.0f), 4.0f);

  Segment2 s1(Vector2(1.0f, 1.0f), Vector2(1.0f, 2.0f)); // Inside not intersecting
  Segment2 s2(Vector2(1.0f, 1.0f), Vector2(1.0f, 5.0f)); // Tagents  
  Segment2 s3(Vector2(1.0f, 1.0f), Vector2(1.0f, 6.0f)); // Clearly intersect    
  Segment2 s4(Vector2(1.0f, 1.0f), Vector2(6.0f, 6.0f)); // Clearly intersect      
  
  CPTAssert(circle.intersect(s1));  // TODO: This is inconsistent with how rect intersect works       
  CPTAssert(circle.intersect(s2));  // since rect requires the two to be strictly intersecting, 
  CPTAssert(circle.intersect(s3));  //  while segment also considers contained as intersecting        
  CPTAssert(circle.intersect(s4));
}

void CircleTests::testPolygonIntersect()
{
  Vector2 p1(1.0f, 1.0f), p2(6.0f, 2.0f), p3(6.0f, 6.0f), p4(2.0f, 6.0f), p5(1.0f, 5.0f);
  Circle c1(Vector2(1.0f, 1.0f), 2.0f);  // Intersect
  Circle c2(Vector2(0.0f, 0.0f), 1.0f);  // Tangents
  Circle c3(Vector2(-1.0f, -1.0f), 1.0f);  // Outside
  Circle c4(Vector2(3.0f, 3.0f), 1.0f);    // Inside
  
  Polygon2 poly;
  poly.push_back(p1);
  poly.push_back(p2);
  poly.push_back(p3);
  poly.push_back(p4);      
  poly.push_back(p5);
  
  CPTAssert(c1.intersect(poly.begin(), poly.end()));
  CPTAssert(!c2.intersect(poly.begin(), poly.end()));
  CPTAssert(!c3.intersect(poly.begin(), poly.end()));
  CPTAssert(!c4.intersect(poly.begin(), poly.end()));      
}

void CircleTests::testBasics()
{
  Circle circle(Vector2(1.0f, 1.0f), 4.0f);
  
  CPTAssert(circle.radius() == 4.0f);
  CPTAssert(circle.center() == Vector2(1.0f, 1.0f));  
  CPTAssert(circle.inside(Vector2(0.0f, 0.0f)));
  CPTAssert(!circle.inside(Vector2(6.0f, 0.0f)));  
  CPTAssert(!circle.inside(Vector2(5.0f, 0.0f)));  // Make sure that circle is not defined on the border
}
    
static CircleTests test1(TEST_INVOCATION(CircleTests, testCircleIntersect));
static CircleTests test2(TEST_INVOCATION(CircleTests, testRectIntersect));
static CircleTests test3(TEST_INVOCATION(CircleTests, testSegmentIntersect));
static CircleTests test4(TEST_INVOCATION(CircleTests, testPolygonIntersect));
static CircleTests test5(TEST_INVOCATION(CircleTests, testBasics));


