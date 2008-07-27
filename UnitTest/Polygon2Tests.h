/*
 *  Polygon2Tests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 4/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class Polygon2Tests : public TestCase {
public:
    Polygon2Tests(TestInvocation* invocation);
    virtual ~Polygon2Tests();
    
    void testIntersections();
    void testMinkowski();    
};