/*
 *  Vector2Tests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 18.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Vector2Tests.h"

#include <iostream>
#include <Geometry/Vector2.hpp>

using namespace std;

Vector2Tests::Vector2Tests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


Vector2Tests::~Vector2Tests()
{
}

void Vector2Tests::testOperators()
{
  Vector2 v(1.0f, 2.0f), u(3.0f, 4.0f);
  CPTAssert(v+u == Vector2(4.0f, 6.0f));
  CPTAssert(v-u == Vector2(-2.0f, -2.0f));
}

void Vector2Tests::testAdvanceOperations()
{

}

static Vector2Tests operatorTest(TEST_INVOCATION(Vector2Tests, testOperators));
static Vector2Tests advanceTests(TEST_INVOCATION(Vector2Tests, testAdvanceOperations));
