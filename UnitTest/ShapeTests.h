/*
 *  ShapeTests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 10.11.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class ShapeTests : public TestCase {
public:
  ShapeTests(TestInvocation* invocation);
  virtual ~ShapeTests();
    
  void testIntersections();
  void testMovement();
  void testHierarchyIntersections();
};