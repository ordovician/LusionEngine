/******************************************************************
Name	: TrapezoidNode2
Author	: Erik Engheim
Date	: 06/02/2006
Desc	: A node in a tree used a a search tree for trapezoids in a trapezoidal map.
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include "Types.h"

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Segment2.hpp>
#include <Geometry/TrapezoidNode2.hpp>
#include <Geometry/Trapezoid2.hpp>

#include <vector>
#include <string>

class TrapezoidNode2
{
public:
    // Constructors
    TrapezoidNode2();
    virtual ~TrapezoidNode2();
    
    // Accessors
    TrapezoidNode2* above() const { return left(); }
    TrapezoidNode2* below() const { return right(); }

    void setAbove(TrapezoidNode2* left) { setLeft(left); }
    void setBelow(TrapezoidNode2* right) { setRight(right); }
        
    TrapezoidNode2* left() const;
    TrapezoidNode2* right() const;    

    void setLeft(TrapezoidNode2* left);
    void setRight(TrapezoidNode2* right);

    std::vector<TrapezoidNode2*> children() const;
    TrapezoidNode2* child(int index) const;
    void setChild(int index, TrapezoidNode2* child);
        
    virtual Trapezoid2* trapezoid() const;    
        
    // Calculations
    virtual TrapezoidNode2* locate(const Point2& p);
    virtual TrapezoidNode2* locate(const Segment2& s);        

    virtual TrapezoidNode2* find(const Point2& p) = 0;    
    virtual TrapezoidNode2* find(const Segment2& s) = 0;
    
    void getTrapezoids(Trapezoids2& out) const;
    
    // Operations
    virtual void replaceWith(TrapezoidNode2* n) = 0;
    
    // Debug
    int tag() const;
    void printTree() const;
    virtual std::string description() const = 0;

    
public:
    static void beginDelete();
    static void endDelete();
    
protected:
    TrapezoidNode2 *iKids[2];

    // Debug
    int iTag;
};

typedef std::vector<TrapezoidNode2*> TrapezoidNodes2;

TrapezoidNode2* newNode(const Segment2& s);
TrapezoidNode2* newNode(const Point2& p);
TrapezoidNode2* newNode(Trapezoid2* t);
TrapezoidNode2* newNode(TrapezoidNode2* n);