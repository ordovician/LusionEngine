/*
 *  SpriteTests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.11.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class SpriteTests : public TestCase {
public:
    SpriteTests(TestInvocation* invocation);
    virtual ~SpriteTests();
    
    void testIntersections();
    void testTrickyIntersections();
    void testMoving();
    void testHierarchyIntersect();
};
