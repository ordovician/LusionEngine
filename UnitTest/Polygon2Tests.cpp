/*
 *  Polygon2Tests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 4/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Polygon2Tests.h"

#include "Geometry/Polygon2.hpp"

#include <numeric>

using namespace std;

Polygon2Tests::Polygon2Tests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


Polygon2Tests::~Polygon2Tests()
{
}

void Polygon2Tests::testIntersections()
{
  Polygon2 poly;
  poly.push_back(Vector2(5, 46.571428571429));
  poly.push_back(Vector2(4.8571428571429, 26.857142857143));
  poly.push_back(Vector2(10, 27));
  poly.push_back(Vector2(20.857142857143, 43.285714285714));
  poly.push_back(Vector2(11.857142857143, 46.142857142857));
  
  Vector2 p(14.0, 33.0);
  bool is_inside = poly.inside(p);
  CPTAssert(is_inside);
}

void Polygon2Tests::testMinkowski()
{
  Polygon2 big(Rect2(-1.0, -1.0, 1.0, 1.0));
  Polygon2 small(Rect2(-0.5, -0.5, 0.5, 0.5));
  Polygon2 small2(Rect2(0.0, 0.0, 1.0, 1.0));
  
  Polygon2 result;
  big.minkowskiSum(small, result);
        
  CPTAssert(result[0] == Vector2(-1.5, -1.5));
  CPTAssert(result[2] == Vector2(1.5, 1.5));

  result.clear();
  big.minkowskiSum(small2, result);

  CPTAssert(result[0] == Vector2(-1.0, -1.0));
  CPTAssert(result[2] == Vector2(2.0, 2.0));  
}

static Polygon2Tests test1(TEST_INVOCATION(Polygon2Tests, testIntersections));
static Polygon2Tests test2(TEST_INVOCATION(Polygon2Tests, testMinkowski));
