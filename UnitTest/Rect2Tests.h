/*
 *  Rect2Tests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class Rect2Tests : public TestCase {
public:
    Rect2Tests(TestInvocation* invocation);
    virtual ~Rect2Tests();
    
    void testBasics();
    void testSegmentIntersections();
    void testRectIntersections();
};
