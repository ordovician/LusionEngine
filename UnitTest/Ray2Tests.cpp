/*
 *  Ray2Tests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Ray2Tests.h"

#include <Geometry/Ray2.hpp>

Ray2Tests::Ray2Tests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


Ray2Tests::~Ray2Tests()
{
}

void Ray2Tests::testBasics()
{
  Ray2 ray1(Point2(2.0f, 0.0f), Vector2(2.0f, 0.0f));
  Ray2 ray2(Point2(1.0f, 1.0f), Vector2(2.0f, 2.0f));
  
  CPTAssert(ray1.origin() == Point2(2.0f, 0.0f));
  CPTAssert(ray1.direction() == Point2(1.0f, 0.0f));  
  CPTAssert(ray2.origin() == Point2(1.0f, 1.0f));
  CPTAssert(ray2.direction() == Point2(1.0f, 1.0f).unit());  
  
}

void Ray2Tests::testSegmentIntersections()
{
  Ray2 r1(Point2(1.0f, -10.0f), Vector2(0.0f, 1.0f));
  Ray2 r2(Point2(1.0f, 10.0f), Vector2(0.0f, 1.0f));
  Ray2 r3(Point2(-2.0f, -10.0f), Vector2(0.0f, 1.0f));
  
  Segment2 s1(Point2(-2.0f, 1.0f), Point2(2.0f, 1.0f));
  
  Point2 p(0.0f, 0.0f);
  
  CPTAssert(r1.intersection(s1, p));
  CPTAssert(p == Point2(1.0f, 1.0f));  

  CPTAssert(r3.intersection(s1, p));
  CPTAssert(p == Point2(-2.0f, 1.0f));  
  
  CPTAssert(!r2.intersection(s1, p));
  CPTAssert(p != Point2(1.0f, 1.0f));  
  
}

void Ray2Tests::testRectIntersections()
{
  Rect2 rect(Vector2(1.0f, 1.0f), Vector2(4.0f, 4.0f));
  
  Ray2 r1(Vector2(-1.0f, 2.0f), Vector2(1.0f, 0.0f)); // Enter from left
  Ray2 r2(Vector2(5.0f, 2.0f), Vector2(-1.0f, 0.0f)); // right
  
  Ray2 r3(Vector2(2.0f, -1.0f), Vector2(0.0f, 1.0f)); // bottom  
  Ray2 r4(Vector2(2.0f, 5.0f), Vector2(0.0f, -1.0f)); // top    
  
  Ray2 r5(Vector2(-1.0f, 2.0f), Vector2(-1.0f, 0.0f)); // Outside left
  Ray2 r6(Vector2(5.0f, 2.0f), Vector2(1.0f, 0.0f)); // right
  
  Ray2 r7(Vector2(2.0f, -1.0f), Vector2(0.0f, -1.0f)); // bottom  
  Ray2 r8(Vector2(2.0f, 5.0f), Vector2(0.0f, 1.0f)); // top    
  
  Ray2 r9(Vector2(2.0f, 2.0f), Vector2(1.0f, 1.0f)); // Inside out    
  Ray2 r10(Vector2(2.0f, 3.0f), Vector2(1.0f, -2.0f)); // Inside out      
  Ray2 r11(Vector2(1.2f, 3.0f), Vector2(-1.0f, -2.0f)); // Inside out        
  
  Ray2 r12(Vector2(1.2f, 5.0f), Vector2(1.2f, 1.0f)); // Outside
    
  CPTAssert(r1.intersect(rect));
  CPTAssert(r2.intersect(rect));
  CPTAssert(r3.intersect(rect));
  CPTAssert(r4.intersect(rect));      
  
  CPTAssert(!r5.intersect(rect));
  CPTAssert(!r6.intersect(rect));
  CPTAssert(!r7.intersect(rect));
  CPTAssert(!r8.intersect(rect));      

  CPTAssert(r9.intersect(rect));  
  CPTAssert(r10.intersect(rect));  
  CPTAssert(r11.intersect(rect));      
  
  CPTAssert(!r12.intersect(rect));        
}

static Ray2Tests test1(TEST_INVOCATION(Ray2Tests, testBasics));
static Ray2Tests test2(TEST_INVOCATION(Ray2Tests, testSegmentIntersections));
static Ray2Tests test3(TEST_INVOCATION(Ray2Tests, testRectIntersections));

//bool intersect(const Ray2& r, const Segment2& s, Vector2& result);
//bool intersect(const Ray2& r, const Polygon2& poly, Vector2& result);
//    Ray2();    
//    Ray2(const Point2& origin, const Vector2& dir);
//    
//    // Accessors
//    Point2 origin() const;
//    Vector2 direction() const;    
//                                    
//    // Operators
//    bool operator==(const Ray2& q) const;
//    bool operator!=(const Ray2& q) const;