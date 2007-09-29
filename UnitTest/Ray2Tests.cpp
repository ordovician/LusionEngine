/*
 *  Ray2Tests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
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

void Ray2Tests::testIntersections()
{
  Ray2 r1(Point2(1.0f, -10.0f), Vector2(0.0f, 1.0f));
  Ray2 r2(Point2(1.0f, 10.0f), Vector2(0.0f, 1.0f));
  Ray2 r3(Point2(-2.0f, -10.0f), Vector2(0.0f, 1.0f));
  
  Segment2 s1(Point2(-2.0f, 1.0f), Point2(2.0f, 1.0f));
  
  Point2 p(0.0f, 0.0f);
  
  CPTAssert(intersect(r1, s1, p));
  CPTAssert(p == Point2(1.0f, 1.0f));  

  CPTAssert(intersect(r3, s1, p));
  CPTAssert(p == Point2(-2.0f, 1.0f));  
  
  CPTAssert(!intersect(r2, s1, p));
  CPTAssert(p != Point2(1.0f, 1.0f));  
  
}

static Ray2Tests test1(TEST_INVOCATION(Ray2Tests, testBasics));
//static Ray2Tests test2(TEST_INVOCATION(Ray2Tests, testIntersections));

//bool intersect(const Ray2& r, const Segment2& s, Vector2& result);
//bool intersect(const Ray2& r, ConstPointIterator2 begin, ConstPointIterator2 end, Vector2& result);
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