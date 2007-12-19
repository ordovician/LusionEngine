/*
 *  IteratorTests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 8.11.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "IteratorTests.h"

#include "Utils/Iterator.h"
#include "Utils/Algorithms.h"

#include <vector>
#include <set>

using namespace std;

IteratorTests::IteratorTests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


IteratorTests::~IteratorTests()
{
}

void IteratorTests::testConstruction()
{
  vector<int> v, u;
  
  v.push_back(2);
  v.push_back(4);  
  v.push_back(6);    
  
  VectorIterator<int> src(v);
  MutableVectorIterator<int> dst(u);
  
  CPTAssert(src.hasNext());
  CPTAssert(!dst.hasNext());  
  CPTAssert(src.next() == 2);    
  CPTAssert(src.next() == 4);      
  CPTAssert(src.next() == 6);        
}

int square(int n) { return n*n; }

void IteratorTests::testVectorCopy()
{
  vector<int> v, u, w;
  
  v.push_back(2);
  v.push_back(4);  
  v.push_back(6);    
  
  VectorIterator<int> src(v);
  MutableVectorIterator<int> dst(u);
  MutableVectorIterator<int> tdst(w);

  Util::insert(&src, &dst);
  src.toFront();
  Util::insert(&src, &tdst, square);
    
  CPTAssert(v == u);
  CPTAssert(w.size() == 3);  
  CPTAssert(w[0] == 4);    
  CPTAssert(w[1] == 16);    
  CPTAssert(w[2] == 36);        
}

void IteratorTests::testVectorSetCopy()
{
  set<int> v;
  set<int> s;
  vector<int> u, w, z;

  v.insert(6);      
  v.insert(2);
  v.insert(4);  

  z.push_back(3);
  z.push_back(2);
  z.push_back(1);
  
  SetIterator<int> src(v);
  VectorIterator<int> zsrc(z);
  
  MutableVectorIterator<int> dst(u);
  MutableVectorIterator<int> tdst(w);
  MutableSetIterator<int> sdst(s);

  Util::insert(&src, &dst);
  src.toFront();
  Util::insert(&src, &tdst, square);
  dst.toFront();
  
  Util::insert(&zsrc, &sdst);
  
  CPTAssert(u[0] == 2);    
  CPTAssert(u[1] == 4);    
  CPTAssert(u[2] == 6);        

  CPTAssert(w.size() == 3);  
  CPTAssert(w[0] == 4);    
  CPTAssert(w[1] == 16);    
  CPTAssert(w[2] == 36);        
  
  CPTAssert(s.size() == 3);  

  set<int>::const_iterator i = s.begin();
  CPTAssert(*i++ == 1);    
  CPTAssert(*i++ == 2);    
  CPTAssert(*i++ == 3);          
}

void IteratorTests::testReplace()
{
  vector<int> u, v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  
  u.push_back(2);
  u.push_back(4);
  u.push_back(6);      
  
  VectorIterator<int> i_v(v);
  MutableVectorIterator<int> i_u(u);

  CPTAssert(u[0] == 2);    
  CPTAssert(u[1] == 4);    
  CPTAssert(u[2] == 6);        
    
  Util::replace(&i_v, &i_u);
  
  CPTAssert(u[0] == 1);    
  CPTAssert(u[1] == 2);    
  CPTAssert(u[2] == 3);        
}

void IteratorTests::testAccess()
{
  vector<int> u, v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  
  u.push_back(4);
  u.push_back(6);
  u.push_back(8);      
  
  VectorIterator<int> i_v(v);
  MutableVectorIterator<int> i_u(u);
  
  i_u.next();
  i_v.next();
  Util::insert(&i_v, &i_u);
  
  CPTAssert(u[0] == 4);    
  CPTAssert(u[1] == 2);    
  CPTAssert(u[2] == 3);        
  CPTAssert(u[3] == 6);        
  CPTAssert(u[4] == 8);            
}

static IteratorTests test1(TEST_INVOCATION(IteratorTests, testConstruction));
static IteratorTests test2(TEST_INVOCATION(IteratorTests, testVectorCopy));
static IteratorTests test3(TEST_INVOCATION(IteratorTests, testVectorSetCopy));
static IteratorTests test4(TEST_INVOCATION(IteratorTests, testReplace));
static IteratorTests test5(TEST_INVOCATION(IteratorTests, testAccess));