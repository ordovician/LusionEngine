#include "SharedObject.hpp"
#include "AutoreleasePool.hpp"

#include <iostream>

using namespace std;

// #define DEBUG_MEMORY

/*!
    \class SharedObject SharedObject.h
    \brief Does reference counting on objects to it can be determine when it needs to be freed.
    
    Since C++ does not have garbadge collection, we have the problem of determening when we need
    to release the memory of an object allocated on the heap (allocated using the new operator). It
    can be difficult to keep track of which class has reponsibility/ownership of an allocated object (which 
    class contains methods with code that will free allocated objects).
    
    SharedObject trie to deal with this problem by requiring the user of an object to call retain()
    when he/she whishes to store it or use it. When object is no longer needed release() should be called.
    SharedObject does reference counting so calling release() will not make the object be deleted if
    other blocks of code is still using it. 
    
    If you want this kind of memory management on your objects you can simply subclass SharedObject.
    
    Usage:
    -When SharedObject are created they will automatically have their referenc counter set to 1. So there
     is no need to retain them. Just call release() when done using it.
    -When one of your methods receives a SharedObject pointer as argument and you want to store
     the object it points to for later use by other methods copy the pointer into a instance variable
     and call retain() on it. When class does not need object anymore, call release() on instance variable.         
    -For more advance usage look at AutoreleasePool class. It describes how shared objects can be returned
    from accessor methods. 
*/


////////////////////////////// Constructors
SharedObject::SharedObject() : iTag(0) , iRefCount(1) 
{
  #ifdef DEBUG_MEMORY  
  // cout << "0x" << hex << (int)this << " SharedObject created" << endl; 
  #endif
}

SharedObject::SharedObject(int aTag) : iTag(aTag) , iRefCount(1) 
{

}

SharedObject::SharedObject(const SharedObject&)
: iRefCount(1) {}

SharedObject& 
SharedObject::operator=(const SharedObject&) {
  return *this;
}  

SharedObject::~SharedObject() 
{
  #ifdef DEBUG_MEMORY  
  // cout << "0x" << hex << (int)this << " SharedObject removed" << endl; 
  #endif  
}

////////////////////////////// Access
void 
SharedObject::setTag(int aTag) 
{
  iTag = aTag;
}

int 
SharedObject::tag() const 
{
  return iTag;
}

////////////////////////////// Operations

/*!
  Increments reference counter. It is safe to call this on a NULL pointer, since
  method will check if it is NULL or not before doing anything.
*/
void 
SharedObject::retain() 
{
  if (this != 0) { 
    ++iRefCount; 
    #ifdef DEBUG_MEMORY
    cout << "0x" << hex << (int)this << " retained, refcount: " << dec << iRefCount << endl; 
    #endif
  }
}

/*!
  Decrements reference counter. It is safe to call this on a NULL pointer, since
  method will check if it is NULL or not before doing anything.
*/
void 
SharedObject::release() 
{
  if (this != 0 && --iRefCount == 0) 
    delete this;
  #ifdef DEBUG_MEMORY
  if (this != 0) {
    cout << "0x" << hex << (int)this << " released, refcount: " << dec << iRefCount << endl; 
    if (iRefCount < 0)
      cout << "Refcount mismatch!" << endl;
  }
  #endif  
}

void 
SharedObject::autorelease() 
{  
  if (this != 0)
    AutoreleasePool::currentPool()->add(this);
}