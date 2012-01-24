/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <vector>

class TrapezoidNode2;

class Trapezoid2
{
public: 
  typedef int Side;
  static const int LEFT = 0;
  static const int RIGHT = 1;
  
  // class iterator
  // {
  // public:
  //   typedef Trapezoid2 *T;
  //   typedef std::forward_iterator_tag  iterator_category;
  //   typedef std::allocator::difference_type  difference_type;
  //   typedef T value_type;
  //   typedef T *pointer;
  //   typedef T &reference;
  //   
  // public:
  //   iterator(T t, int index = 0) { 
  //     iT = t;
  //     iIndex = index;
  //     while (iIndex < 4 && iT->neighbor(iIndex) == 0)
  //       ++iIndex;      
  //   }
  //   inline T operator*() const { return iT->neighbor(iIndex); }
  //   inline iterator &operator++() { nextIndex(); return *this; }
  //   inline iterator operator++(int) { iterator it(iT, iIndex); nextIndex(); return it; }
  //   inline iterator &operator--() { prevIndex(); return *this; }
  //   inline iterator operator--(int) { iterator it(iT, iIndex); prevIndex(); return it; }
  //   inline bool operator==(const iterator &it) const { return iIndex == it.iIndex; }
  //   inline bool operator!=(const iterator &it) const { return iIndex != it.iIndex; }
  // 
  // private:
  //   void nextIndex() {
  //     if (iIndex == 4)
  //       return;
  //     do {
  //       ++iIndex;
  //     } while (iIndex < 4 && iT->neighbor(iIndex) == 0);
  //   }
  //   
  //   void prevIndex() {
  //     if (iIndex == 0)
  //       return;
  //     int i = iIndex;
  //     do {
  //       --i;
  //     } while (i > 0 && iT->neighbor(i) == 0);
  //     if (iT->neighbor(i) != 0)
  //       iIndex = i;
  //   }
  //   
  //   T iT;
  //   int iIndex;
  // };
  
public:
    // Constructors
    Trapezoid2();
    Trapezoid2(const Point2& p, const Point2& q, const Segment2& bottom, const Segment2& top);
    virtual ~Trapezoid2();
    
    // Accessors
    Point2    left() const;
    void      setLeft(const Point2& p);
    Point2    right() const;    
    void      setRight(const Point2& p);
    Segment2  bottom() const;
    Segment2  top() const;    
    Point2    center() const;
    Point2    centerLeft() const;
    Point2    centerRight() const;

    Point2    endPoint(int index);
    void      setEndPoint(int index, const Point2& p);

    void setNeighbors(Trapezoid2 * ll, Trapezoid2 * ul, Trapezoid2 * lr, Trapezoid2 *ur);
    void setRightNeighbors(Trapezoid2 * lower, Trapezoid2 * upper);
    void setLeftNeighbors(Trapezoid2 * lower, Trapezoid2 * upper);    
    
    void setLeftNeighbor(Trapezoid2* t);
    void setRightNeighbor(Trapezoid2* t);    
    
    Trapezoid2 *upperLeft() const;
    Trapezoid2 *upperRight() const;    
    Trapezoid2 *lowerLeft() const;
    Trapezoid2 *lowerRight() const;    
    
    void setLowerLeft(Trapezoid2 *t);
    void setLowerRight(Trapezoid2 *t);
    void setUpperLeft(Trapezoid2 *t);
    void setUpperRight(Trapezoid2 *t);            

    Trapezoid2* neighbor(int index) const;
    void setNeighbor(int index, Trapezoid2* t);
    void setNeighbor(Trapezoid2::Side side, int index, Trapezoid2* t);
    
    std::vector<Trapezoid2*> neighbors() const;    
    void getNeighbors(std::vector<Trapezoid2*>& ts) const;  
    std::vector<Trapezoid2*> leftNeighbors() const;    
    std::vector<Trapezoid2*> rightNeighbors() const;        

    std::vector<Trapezoid2*> neighbors(Side side) const;    
    Trapezoid2* neighbor(Side side, int index) const;    
        
    void setNode(TrapezoidNode2* node);
    TrapezoidNode2* node() const;  
    
    int noLeftNeighbors() const;
    int noRightNeighbors() const;
        
    // Request
    bool isBordering(Trapezoid2* t) const;
    
    // Calculations
    real distance(Trapezoid2* t) const;
    Trapezoid2* clone(const Point2& p, const Point2& q, const Segment2& bottom, const Segment2& top) const;
    
    // Operations
    void connect(int side, Trapezoid2* st);    
    void cleanup(std::vector<Trapezoid2*>::iterator begin, std::vector<Trapezoid2*>::iterator end);
    
    // Debug
    void setTag(int tag);
    int tag() const;
    
private:
  Point2 iPoint[2];
  Segment2 iTopS, iBottomS;   // segments defining bottom and top of trapezoid
  Trapezoid2 *iNeighbor[4];   // The neighbour trapezoids
  TrapezoidNode2* iNode;
  
  // Debug
  int iTag;
};

// Helper functions
int opposite(Trapezoid2::Side side);

// Typedefs
typedef std::vector<Trapezoid2*> Trapezoids2;