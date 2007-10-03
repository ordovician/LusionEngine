/******************************************************************
Name	: SharedObject
Author	: Erik Engheim
Date	: 30/10/2005
Desc	: 
Comment	: 
*******************************************************************/

#pragma once

#include <set>
    
/**
 * Shared object. One object can be shared among multiple objects. 
 * @author Erik Engheim
 */
class SharedObject {                       
public:
  // Constructors
  SharedObject();
  SharedObject(int aTag);  
  SharedObject(const SharedObject& aObj);
  SharedObject& operator=(const SharedObject& aObj);
  virtual ~SharedObject();
  
public:                
  // Access
  void  setTag(int aTag);
  int   tag() const;
  
  // Operations
  void  retain();
  void  release();
  void  autorelease();  

private:
  int  iTag;
  int  iRefCount;
};

typedef std::set<SharedObject *> SharedObjects;

/**
* Use this class to automatically release shared objects created in this scope.
*/
class ScopeReleaser {
public:
  ScopeReleaser(SharedObject *aObj) : iObj(aObj) { }
  ~ScopeReleaser() { iObj->release(); }
private:
    SharedObject *iObj;
};

/**
* Use this class to retain a shared object for the duration of a scope and then release it
* when exiting scope.
*/
class ScopeRetainer {
public:
  ScopeRetainer(SharedObject *aObj) : iObj(aObj) { iObj->retain(); }
  ~ScopeRetainer() { iObj->release(); }
private:
    SharedObject *iObj;
};


/**
 * Smart pointer class. Does automatic reference counting
 * Template class for smart pointers to T object.
 * T must support the SharedObject interface
 * @author Erik Engheim
 */
template<class T>                      
class SmartPointer {                         
public:                                
  SmartPointer(T* aRealPtr = 0);
  SmartPointer(const SmartPointer& aObj);
  ~SmartPointer();
  SmartPointer& operator=(const SmartPointer& aObj);                     
  T* operator->() const;
  T& operator*() const;

private:
  void init();
  
private:
  T *iSharedObject;

};

template<class T>
void SmartPointer<T>::init()
{
  if (iSharedObject == 0) return;
}

template<class T>
SmartPointer<T>::SmartPointer(T* aObj)
: iSharedObject(aObj)
{
  init();
}

template<class T>
SmartPointer<T>::SmartPointer(const SmartPointer& aObj)
: iSharedObject(aObj.iSharedObject)
{ 
  init();
}

template<class T>
SmartPointer<T>::~SmartPointer()
{
  if (iSharedObject) iSharedObject->release();
}

template<class T>
SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer& aObj)
{
  if (iSharedObject != aObj.iSharedObject) {                   
    T *old = iSharedObject;                      
                                                
    iSharedObject = aObj.iSharedObject;
    init(); 

    if (old) old->release();                
  }
  
  return *this;
}

template<class T>
T* SmartPointer<T>::operator->() const 
{
  return iSharedObject;
}

template<class T>
T& SmartPointer<T>::operator*() const
{
  return *iSharedObject;
}