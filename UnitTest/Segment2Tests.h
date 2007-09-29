/*
 *  Segment2Tests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.9.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class Segment2Tests : public TestCase {
public:
    Segment2Tests(TestInvocation* invocation);
    virtual ~Segment2Tests();
  
    void testSegmentIntersections();
    void testRectIntersections();
    void testBasics();
    void testNearest();
};
