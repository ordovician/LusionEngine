/*
 *  CircleTests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "CircleTests.h"

#include <Geometry/Vector2.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/Ray2.hpp>

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
  Segment2 s5(Vector2(5.0f, 1.0f), Vector2(8.0f, 1.0f)); // Tagents outside   
  
  CPTAssert(circle.intersect(s1));  // TODO: This is inconsistent with how rect intersect works       
  CPTAssert(circle.intersect(s2));  // since rect requires the two to be strictly intersecting, 
  CPTAssert(circle.intersect(s3));  //  while segment also considers contained as intersecting        
  CPTAssert(circle.intersect(s4));
  CPTAssert(!circle.intersect(s5));  
}

void CircleTests::testPolygonIntersect()
{
  Vector2 p1(1.0f, 1.0f), p2(6.0f, 2.0f), p3(6.0f, 6.0f), p4(2.0f, 6.0f), p5(1.0f, 5.0f);
  Circle c1(Vector2(1.0f, 1.0f), 2.0f);  // Intersect
  Circle c2(Vector2(0.0f, 0.0f), 1.0f);  // Tangents
  Circle c3(Vector2(-1.0f, -1.0f), 1.0f);  // Outside
  Circle c4(Vector2(3.0f, 3.0f), 1.0f);    // Inside
  
  Points2 poly;
  poly.push_back(p1);
  poly.push_back(p2);
  poly.push_back(p3);
  poly.push_back(p4);      
  poly.push_back(p5);
  
  CPTAssert(c1.intersect(poly.begin(), poly.end()));
  CPTAssert(!c2.intersect(poly.begin(), poly.end()));
  CPTAssert(!c3.intersect(poly.begin(), poly.end()));
  CPTAssert(!c4.intersect(poly.begin(), poly.end()));
  
  // Test against standard collision polygon
  Circle c5(Vector2(10.0f, 0.0f), 2.0f);
  Point2 points[] = {Point2(-1.0, -1.0), Point2(1.0, 0.0), Point2(-1.0, 1.0)};
  
  // Make the test that will be performed against polygon by circle
  Segment2 
    seg1(points[0], points[1]), 
    seg2(points[1], points[2]),
    seg3(points[2], points[0]);    
  
  CPTAssert(!c5.intersect(seg1));
  CPTAssert(!c5.intersect(seg2));
  CPTAssert(!c5.intersect(seg3));
      
  Points2 colPoly(points, points+3);
  
  CPTAssert(!c5.intersect(colPoly.begin(), colPoly.end()));  
}

void CircleTests::testRayIntersect()
{
  Circle c1(Vector2(2.0f, 2.0f), 4.0f);
  
  Ray2 r1(Vector2(2.0f, 2.0f), Vector2(1.0f, 0.0f)); // Going out
  Ray2 r2(Vector2(2.0f, 2.2f), Vector2(-1.0f, 0.0f)); // Going out
  Ray2 r3(Vector2(2.1f, 2.0f), Vector2(1.0f, 1.0f)); // Going out
  Ray2 r4(Vector2(1.0f, 2.0f), Vector2(-1.0f, -1.0f)); // Going out      
  Ray2 r5(Vector2(2.0f, 3.0f), Vector2(0.0f, -1.0f)); // Going out        
  Ray2 r6(Vector2(2.5f, 2.5f), Vector2(0.0f, 1.0f)); // Going out          
  
  Ray2 r7(Vector2(6.5f, 2.5f), Vector2(1.0f, 1.0f)); // Outside not intersecting          
  Ray2 r8(Vector2(2.5f, 7.5f), Vector2(0.0f, 1.0f)); // Outside not intersecting          
  Ray2 r9(Vector2(-6.5f, 2.5f), Vector2(-1.0f, 1.0f)); // Outside not intersecting          
  Ray2 r10(Vector2(2.5f, -10.5f), Vector2(0.2f, -1.0f)); // Outside not intersecting                
  
  // Border values
  Ray2 r11(Vector2(6.0f, 2.0f), Vector2(1.0f, 0.0f)); // Tangenting
  Ray2 r12(Vector2(-2.0f, 2.5f), Vector2(-1.0f, 0.2f)); // Tangenting
  
  
  CPTAssert(c1.intersect(r1));
  CPTAssert(c1.intersect(r2));
  CPTAssert(c1.intersect(r3));
  CPTAssert(c1.intersect(r4));
  CPTAssert(c1.intersect(r5));        
  CPTAssert(c1.intersect(r6));          
  
  CPTAssert(!c1.intersect(r7));
  CPTAssert(!c1.intersect(r8));
  CPTAssert(!c1.intersect(r9));
  CPTAssert(!c1.intersect(r10));  
  
  CPTAssert(!c1.intersect(r11));
  CPTAssert(!c1.intersect(r12));    
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
static CircleTests test6(TEST_INVOCATION(CircleTests, testRayIntersect));


