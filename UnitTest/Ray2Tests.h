/*
 *  Ray2Tests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class Ray2Tests : public TestCase {
public:
    Ray2Tests(TestInvocation* invocation);
    virtual ~Ray2Tests();
    
    void testBasics();
    void testSegmentIntersections();
    void testRectIntersections();
};
