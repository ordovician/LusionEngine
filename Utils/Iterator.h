/*!
  \file Iterator.h
  \author Erik Engheim - Translusion
  \date 18.10.2007
  
  Copyright 2007 Translusion. All rights reserved.
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

*/
template<class T>
class Iterator : public SharedObject
{
public:
    virtual void  first() = 0; 
    virtual void  next() = 0; 
    virtual bool  done() const = 0;
    virtual const T& value() const = 0;
    
    std::string typeName() const { return "Iterator"; }
};

/*!

*/
template<class T>
class MutableIterator : public Iterator<T>
{
public:
     virtual void setValue(const T &t)  = 0;
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
    void first() {} 
    bool done() const { return true; } 
    void next() {}
      
    /*! \throw IteratorOutOfBoundsException */
    const T& value() const { throw IteratorOutOfBoundsException(); }  
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
    void first()      { i = c.begin(); } 
    bool done() const { return i == c.end(); } 
    void next()       { ++i; }

    /*! \throw IteratorOutOfBoundsException */
    const T& value() const { 
      if (done())
        throw IteratorOutOfBoundsException();
      return *i;
    }       
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
  
  void first()      { i = c.begin(); } 
  bool done() const { return i == c.end(); } 
  void next()       { ++i; } 

  /*! \throw IteratorOutOfBoundsException */
  const T& value() const { 
    if (done())
      throw IteratorOutOfBoundsException();
    return *i;
  }

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
  void first()       { i = c.begin(); }
  bool done() const  { return c.end() == i; }
  void next()        { ++i; }
  void setValue(const T &t)  { *i = t; }
  void insert(const T &t)    { i = c.insert(i, t); ++i; }

  /*! \throw IteratorOutOfBoundsException */
  const T& value() const { 
    if (done())
      throw IteratorOutOfBoundsException();
    return *i;
  }     
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
  
  void first()               { i = c.begin(); } 
  bool done() const          { return i == c.end(); } 
  void next()                { ++i; } 
  void setValue(const T &t)  { throw IteratorOutOfBoundsException(); }
  void insert(const T& t)    { i = c.insert(t).first;  ++i; }
  
  /*! \throw IteratorOutOfBoundsException */
  const T& value() const { 
    if (done())
      throw IteratorOutOfBoundsException();
    return *i;
  }  
};


/*!
  Does preorder iteration. All that is required for this to work 
  is that we are provided with a pointer to a node in the tree
  which is a model of Concept NodePtr. An instance 'n' of a NodePtr
  model must support n->iterator(), which should return an iterator
  over the nodes immediate children. The children should also be models
  of NodePtr.
*/
template<class NodePtr>
class PreorderIterator : public Iterator<NodePtr>
{
public:
  PreorderIterator(const NodePtr root) 
      : iRoot(root) {} 

  void first() { 
    Iterator<NodePtr>* i = iRoot->iterator();
    i->retain();
    if (i != 0) {
      i->first();
      iIterators.clear();
      iIterators.push_back(i);
      assert(iIterators.size() == 1);
    } 
  } 
  
  bool done() const { 
    return iIterators.empty();
  } 
  
  void next() {
    assert(!done());
    Iterator<NodePtr>* i = iIterators.back()->value()->iterator();
    i->retain();
    i->first();
    assert(i->refCount() == 2);
    iIterators.push_back(i);
    
    while (iIterators.back()->done()) {
      assert(iIterators.back()->refCount() <= 2);
      iIterators.back()->release();
      iIterators.pop_back();
      if (done())
        break;
      iIterators.back()->next();
    }        
  }

  /*! \throw IteratorOutOfBoundsException */
  const NodePtr& value() const { 
    if (iIterators.empty())
      throw IteratorOutOfBoundsException();
    return iIterators.back()->value();
  }
  
private:
  NodePtr iRoot;
  std::vector<Iterator<NodePtr>* > iIterators;
};