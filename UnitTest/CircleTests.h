/*
 *  CircleTests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>

#include <Geometry/Circle.hpp>

class CircleTests : public TestCase {
public:
    CircleTests(TestInvocation* invocation);
    virtual ~CircleTests();
    
    void testCircleIntersect();
    void testRectIntersect();
    void testSegmentIntersect();
    void testPolygonIntersect();
    void testRayIntersect();
    void testBasics();
};
