/*!
  \file Iterator.h
  \author Erik Engheim - Translusion
  \date 18.10.2007
  
*/

#pragma once

#include "Core/Core.h"
#include "Core/SharedObject.hpp"

#include "Utils/Exception.h"

#include <vector>
#include <set>

/*!
  Thrown when trying to dereference an iterator and it is pointing beyond end
  or before beginning. 
*/
class IteratorOutOfBoundsException : public Exception
{
  
};

/*!
  Base class for iterators modeled after Java style iterators as opposed to
  STL type of iterators. The advantage of this class over STL classes is that
  one can iterate over collections that are not known at runtime. E.g. when one only
  knows the interface to a Collection but not its concrete implementation and want
  to iterate over it.
  
  This iterator does not allow you to change the elements of the data you iterate over.
*/
template<class T>
class Iterator : public SharedObject
{
public:
    virtual void  toFront() = 0; 
    virtual void  toBack() = 0; 
    virtual bool  hasNext() const = 0; 
    virtual const T &next() = 0;
};

/*!
  Base class for iterators modeled after Java style iterators as opposed to
  STL type of iterators. The advantage of this class over STL classes is that
  one can iterate over collections that are not known at runtime. E.g. when one only
  knows the interface to a Collection but not its concrete implementation and want
  to iterate over it.
  
  This iterator does  allow you to change the elements of the data you iterate over.
*/
template<class T>
class MutableIterator : public Iterator<T>
{
public:
     virtual void setNext(const T &t)  = 0;
     virtual void insert(const T &t) = 0;
};

/*!
  NullIterator is always at the end.
*/
template<class T>
class NullIterator : public Iterator<T>
{
public:
    NullIterator() {} 
    void toFront() {} 
    void toBack()  {} 
    bool hasNext() const { return false; } 
    
    /*! \throw IteratorOutOfBoundsException */
    const T &next() { throw IteratorOutOfBoundsException(); }  
};

/*!
  Subclass mean to wrap an STL type of iterator. You provide
  in the constructor the range you want to iterate over with this
  iterator.
*/
template<class T>
class VectorIterator : public Iterator<T>
{
  typedef typename std::vector<T>::const_iterator const_iterator;
  const std::vector<T>& c;
  const_iterator i;

public:
    VectorIterator(const std::vector<T> &container) 
        : c(container), i(c.begin()) {} 
    VectorIterator &operator=(const std::vector<T> &container) 
    { c = container; i = c.begin(); return *this; } 
    void toFront() { i = c.begin(); } 
    void toBack() { i = c.end(); } 
    bool hasNext() const { return i != c.end(); } 
    const T &next() { return *i++; } 
};


/*!
  Subclass mean to wrap an STL type of iterator. You provide
  in the constructor the range you want to iterate over with this
  iterator.
*/
template<class T>
class SetIterator : public Iterator<T>
{
public:
  typedef typename std::set<T>::const_iterator iterator;
  
public:
  SetIterator(const std::set<T>& container) 
    : c(container),
      i(container.begin()) {}
  
  void toFront() { i = c.begin(); } 
  void toBack() { i = c.end(); } 
  bool hasNext() const { return i != c.end(); } 
  const T &next() { return *i++; } 

private:
  const std::set<T>& c;
  iterator i;  
};

/*!
  Subclass mean to wrap an STL type of iterator. You provide
  in the constructor the range you want to iterate over with this
  iterator.
*/
template <class T>
class MutableVectorIterator : public MutableIterator<T>
{
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    std::vector<T>& c;
    iterator i;
    
public:
     MutableVectorIterator(std::vector<T> &container)
        : c(container)
     { i = c.begin(); }   
     void toFront() { i = c.begin(); }
     void toBack() { i = c.end(); }
     bool hasNext() const { return c.end() != i; }
     const T &next() { return *i++; }
     void setNext(const T &t)  { *i++ = t; }
     void insert(const T &t) { i = c.insert(i, t); ++i; }
};

template<class T>
class MutableSetIterator : public MutableIterator<T>
{
  typedef typename std::set<T>::iterator iterator;
  typedef typename std::set<T>::const_iterator const_iterator;
  std::set<T>& c;
  iterator i;  
  
public:
  MutableSetIterator(std::set<T>& container) 
    : c(container),
      i(container.begin()) {}
  
  void toFront() { i = c.begin(); } 
  void toBack() { i = c.end(); } 
  bool hasNext() const { return i != c.end(); } 
  const T &next() { return *i++; } 
  void setNext(const T &t)  { throw UnsuportedMethodException(); }

  void insert(const T& t) { i = c.insert(t).first;  ++i; }
};