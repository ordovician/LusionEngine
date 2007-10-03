#include "AutoreleasePool.hpp"

#include <algorithm>
#include <assert.h>

#include <iostream>

// #define DEBUG_MEMORY

using namespace std;

/***************************************************************************
  CleanupPool
***************************************************************************/
////////////////////////////// Static member variables
CleanupPool CleanupPool::sDefaultPool;
  
////////////////////////////// Constructors
CleanupPool::CleanupPool() {

}

CleanupPool::~CleanupPool() {
  releasePool();
}

////////////////////////////// Access
void  
CleanupPool::add(SharedObject *aObj) {
  iPoolObjects.insert(aObj);
}

////////////////////////////// Operations
void 
CleanupPool::releasePool() {
  for_each(iPoolObjects.begin(), iPoolObjects.end(), mem_fun(&SharedObject::release));
  iPoolObjects.clear();
}

////////////////////////////// Static access
CleanupPool * 
CleanupPool::defaultPool() {
  return &sDefaultPool;
} 

/*!
    \class AutoreleasePool AutoreleasePool.h
    \brief Keeps track of shared objects that should be released.
     
    Look at description of SharedObject.
    When retreiving a shared object from an accessor it is cumbersome to always have to 
    release it after using it. 
    
    AutoreleasePool helps deal with that problem. Accessors should call autorelease() on
    the shared objects they return. This will allow the AutoreleasePool to call release on the
    shared objects later so the user calling the accessors don't have to bother with it. It is a way
    of moving complexity and responsibility over from the class user to the class maker. 
    
    Usage:
    AutoreleasePool::begin();   // Creates a new pool where all autorelease objects will be added to
    SharedObject* obj = myObj->getSomeObj();
    obj->doSomething();
    AutoreleasePool::end();     // Calls release() all objects marked for autorelease.    
    
    Not in this example we did not need to call release() on obj. That is because AutoreleasePool will
    do it when end() is called. 
    
    AutoreleasePool's can be nested:
    AutoreleasePool::begin();    
    AutoreleasePool::begin();    
    AutoreleasePool::end();
    AutoreleasePool::end();     
    
    Calling end() in a sub block will not release objects allocated or accessed
    in the outer block.    
*/
////////////////////////////// Static member variables
stack<AutoreleasePool*> AutoreleasePool::sPoolStack;
  
////////////////////////////// Constructors
AutoreleasePool::AutoreleasePool() 
{

}

AutoreleasePool::AutoreleasePool(const AutoreleasePool& pool) : SharedObject(pool)
{
}

AutoreleasePool& 
AutoreleasePool::operator=(const AutoreleasePool&) {
  return *this;
}  

AutoreleasePool::~AutoreleasePool() 
{
  releasePool();
  #ifdef DEBUG_MEMORY  
  cout << hex << "0x" << (int)this << " AutoreleasePool removed" << endl;  // DEBUG    
  #endif
}

////////////////////////////// Access
void  
AutoreleasePool::add(SharedObject *aObj) 
{
  #ifdef DEBUG_MEMORY
    cout << "AutoreleasePool::add:pool:" << hex << (int)this << " obj: " << hex << (int)aObj << endl;
  #endif
  iPoolObjects.insert(aObj);
}

////////////////////////////// Operations
void 
AutoreleasePool::releasePool() {
  #ifdef DEBUG_MEMORY
      cout << "AutoreleasePool::releasePool:" << hex << (int)this << endl;
  #endif

  for_each(iPoolObjects.begin(), iPoolObjects.end(), mem_fun(&SharedObject::release));
  iPoolObjects.clear();
}

////////////////////////////// Static access           
void AutoreleasePool::begin()
{
   sPoolStack.push(new AutoreleasePool);
}   

void AutoreleasePool::end()
{   
    assert(!sPoolStack.empty());
    AutoreleasePool* pool = sPoolStack.top();    
    sPoolStack.pop();
    pool->release();
}

AutoreleasePool *AutoreleasePool::currentPool()
{
    if (sPoolStack.empty())
        return 0;
    return sPoolStack.top();
}