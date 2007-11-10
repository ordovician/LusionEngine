/*
 *  Algorithms.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 6.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

#include "Core/Core.h"
#include "Utils/Iterator.h"

namespace Util
{
  /*!
    Inserts elements from \a source into collection pointed to by \a dest
    Collection pointed to by \a dest can be empty, it will be expanded as
    needed.
  */
  template<class T>
  void insert(Iterator<T>* source, MutableIterator<T>* dest) {
    while (source->hasNext()) {
      dest->insert(source->next());
    }
  }

  /*!
    Inserts elements from \a source into collection pointed to by \a dest
    Collection pointed to by \a dest can be empty, it will be expanded as
    needed. Elements are transformed with function \a f before they are
    inserted. 
  */
  template<class T, class F>
  void insert(Iterator<T>* source, MutableIterator<T>* dest, F f) {
    while (source->hasNext()) {
      dest->insert(f(source->next()));
    }
  }

  /*!
    No new elements are inserted into \a collection pointed to by
    \a dest. Instead the value of each element is replaced with
    value from \a source.
  */
  template<class T>
  void replace(Iterator<T>* source, MutableIterator<T>* dest) {
    while (source->hasNext()) {
      dest->setNext(source->next());
    }
  }

  template<class T, class F>
  void transform(Iterator<T>* source, MutableIterator<T>* dest, F f) {
    while (source->hasNext()) {
      dest->setNext(f(source->next()));
    }
  }

  template<class T, class F>
  void foreach(Iterator<T>* source, F f) {
    while (source->hasNext()) {
      f(source->next());
    }
  }
}

