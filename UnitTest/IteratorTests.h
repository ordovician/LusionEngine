/*
 *  IteratorTests.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 8.11.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>


class IteratorTests : public TestCase {
public:
    IteratorTests(TestInvocation* invocation);
    virtual ~IteratorTests();
    
    void testConstruction();
    void testVectorCopy();
    void testVectorSetCopy();    
    void testReplace();
    void testAccess();
};
