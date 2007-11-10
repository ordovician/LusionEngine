/*
 *  ShapeTests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 10.11.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "ShapeTests.h"

 #include "Base/CircleShape.h"
 #include "Base/RectShape2.h"
 #include "Base/SegmentShape2.h"

static real t = 0.0f;
static real dt = 1.0f;

ShapeTests::ShapeTests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


ShapeTests::~ShapeTests()
{
}

void ShapeTests::testIntersections()
{
  // Test circle vs circle
  CircleShape* c1 = new CircleShape(Circle(Vector2(0.0f, 0.0f), 4.0f));
  CircleShape* c2 = new CircleShape(Circle(Vector2(2.0f, 2.0f), 4.0f)); // overlap
  CircleShape* c3 = new CircleShape(Circle(Vector2(10.0f, 0.0f), 2.0f)); // outside
      
  CPTAssert(c1->inside(Vector2(1.0f, 1.0f), t, dt));
  CPTAssert(!c1->inside(Vector2(5.0f, 1.0f), t, dt));  
  
  CPTAssert(c1->collide(c2, t, dt));    
  CPTAssert(!c1->collide(c3, t, dt));    
    
  // Test circle with rectangles
  RectShape2* r1 = new RectShape2(Rect2(Vector2(1.0f, 1.0f), Vector2(6.0f, 6.0f)));
  RectShape2* r2 = new RectShape2(Rect2(Vector2(6.0f, 6.0f), Vector2(10.0f, 10.0f)));
    
  CPTAssert(c1->collide(r1, t, dt));    
  CPTAssert(!c1->collide(r2, t, dt));    
  
      
  // Cleanup
  c1->release();
  c2->release();
  c3->release();    
  
  r1->release();
  r2->release();
}

void ShapeTests::testMovement()
{
}

void ShapeTests::testHierarchyIntersections()
{
}

static ShapeTests test1(TEST_INVOCATION(ShapeTests, testIntersections));
static ShapeTests test2(TEST_INVOCATION(ShapeTests, testMovement));
static ShapeTests test3(TEST_INVOCATION(ShapeTests, testHierarchyIntersections));