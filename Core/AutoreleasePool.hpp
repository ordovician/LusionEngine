/******************************************************************
Name	: AutoreleasePool
Author	: Erik Engheim
Date	: 30/10/2005
Desc	: 
Comment	: 
*******************************************************************/

#pragma once

#include "SharedObject.hpp"

#include <stack>

/**
* Put all objects that needs to be released when program ends in this pool.
*/
class CleanupPool {
public:
  // Constructors
  CleanupPool();
  virtual ~CleanupPool();  
  
public:
  // Access
  void add(SharedObject *aObj);
  
  // Operations
  void  releasePool();
  
public:
  // Static Access
  static CleanupPool *defaultPool();
    
private:
  SharedObjects       iPoolObjects;
  static CleanupPool  sDefaultPool;
};

/**
 * Takes care of automatically releasing Shared Objects marked for autorelease.
 * When autorelease is called for a share object it is placed in a autorelease pool.
 * If no spesific pool is specified, then the default pool is used. When release is called on
 * the pool or it's object is destroyed then the pool will call release on all 
 * shared objects that have been placed in its pool.
 */
class AutoreleasePool : public SharedObject 
{                       
public:
  // Constructors
  AutoreleasePool();
  AutoreleasePool(const AutoreleasePool& aObj);
  AutoreleasePool& operator=(const AutoreleasePool& aObj);
  virtual ~AutoreleasePool();
  
public:              
  // Access
  void  add(SharedObject *aObj);

  // Operations
  void  releasePool();
  
public:
  // Static Access
  static void begin();
  static void end();  
  static AutoreleasePool *currentPool();
    
private:
  SharedObjects                       iPoolObjects;  
  static std::stack<AutoreleasePool*> sPoolStack;
};