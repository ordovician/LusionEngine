/*
 *  Vector2Tests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 18.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class Vector2Tests : public TestCase {
public:
    Vector2Tests(TestInvocation* invocation);
    virtual ~Vector2Tests();
    
public:
  void testOperators();
  void testAdvanceOperations();
};
