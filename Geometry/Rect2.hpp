/******************************************************************
Name    : Rect2
Author  : Erik Engheim
Date    : 18/10/2005
Desc    : 
Comment : This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>

class Rect2
{
public:
    // Constructors
    Rect2();
    Rect2(const Point2 &aMin, const Point2 &aMax);
    Rect2(real xmin, real ymin, real xmax, real ymax);
    ~Rect2();
    
    // Accessors
    Point2&       min();
    const Point2& min() const;
    void          setMin(const Point2 &aMin);
    Point2&       max();
    const Point2& max() const;
    void          setMax(const Point2 &aMax);

    real left() const;
    real top() const;
    real right() const;
    real bottom() const;
    
    real xmin() const;
    real ymin() const;
    real xmax() const;
    real ymax() const;
    
    real x() const;
    real y() const;   
    
    void setLeft(real aPos);
    void setTop(real aPos);
    void setRight(real aPos);
    void setBottom(real aPos);  
    void setX(real x);
    void setY(real y);      
         
    void setTopLeft(const Point2 &aP);
    void setBottomRight(const Point2 &aP);
    void setTopRight(const Point2 &aP);
    void setBottomLeft(const Point2 &aP);

    Point2 topLeft() const;
    Point2 bottomRight() const;
    Point2 topRight() const;
    Point2 bottomLeft() const;
    Point2 center() const;
    
    Size2 size() const;
    real  width() const;
    real  height() const;
    
    Size2 halfSize() const;
    
    // Request
    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;
    
    // Calculations
    real  area() const;    
    Rect2 surround(const Vector2& aV) const;    
    Rect2 surround(const Rect2& aBox) const;
    bool  intersect(const Rect2 &aRect2) const;
    bool  inside(const Point2& aP) const;

    Rect2 translated(real aDx, real aDy) const;
    Rect2 translated(const Vector2 &aP) const;    

    // Operations
    void translate(real aDx, real aDy);
    void translate(const Vector2 &aP);
    
    void moveTo(real aX, real aT);
    void moveTo(const Vector2 &aP); 
    void moveCenter(const Vector2& aP);
    
private:
    Point2 iPos[2];
};

// Accessors
inline 
real Rect2::left() const
{ 
  return xmin(); 
}

inline 
real Rect2::top() const
{ 
  return ymax(); 
}

inline 
real Rect2::right() const
{ 
  return xmax(); 
}

inline 
real Rect2::bottom() const
{ 
  return ymin(); 
}

inline 
real Rect2::xmin() const
{ 
  return min().x(); 
}

inline 
real Rect2::ymax() const
{ 
  return max().y(); 
}

inline 
real Rect2::xmax() const
{ 
  return max().x(); 
}

inline 
real Rect2::ymin() const
{ 
  return min().y(); 
}

inline 
real Rect2::x() const
{ 
  return xmin(); 
}

inline 
real Rect2::y() const
{ 
  return ymin(); 
}

inline 
void Rect2::setLeft(real pos)
{ 
  setX(pos); 
}

inline 
void Rect2::setTop(real pos)
{ 
  max().setY(pos); 
}

inline 
void Rect2::setRight(real pos)
{ 
  max().setX(pos); 
}

inline 
void Rect2::setBottom(real pos)
{ 
  min().setY(pos); 
}

inline 
void Rect2::setTopLeft(const Point2 &p)
{ 
  setLeft(p.x());
  setTop(p.y()); 
}

inline 
void Rect2::setBottomRight(const Point2 &p)
{ 
  setBottom(p.y());
  setRight(p.x());
}

inline 
void Rect2::setTopRight(const Point2 &p)
{ 
  setTop(p.y());
  setRight(p.x());
}

inline 
void Rect2::setBottomLeft(const Point2 &p)
{ 
  setBottom(p.y());
  setLeft(p.x());
}

inline 
void Rect2::setX(real ax)
{ 
  min().setX(ax); 
}

inline 
void Rect2::setY(real ay)
{ 
  min().setY(ay); 
}

inline 
Point2 Rect2::topLeft() const
{ 
  return Point2(left(), top()); 
}

inline 
Point2 Rect2::bottomRight() const
{ 
  return Point2(right(), bottom()); 
}

inline 
Point2 Rect2::topRight() const
{ 
  return Point2(right(), top()); 
}

inline 
Point2 Rect2::bottomLeft() const
{ 
  return Point2(left(), bottom()); 
}

inline 
real Rect2::width() const
{
  return  xmax() - xmin(); 
}

inline 
real Rect2::height() const
{ 
  return  ymax() - ymin(); 
}

// Request
inline 
bool Rect2::isNull() const
{ 
  return min() == max(); 
}

inline 
bool Rect2::isEmpty() const
{ 
  return left() > right() || top() < bottom(); 
}

inline 
bool Rect2::isValid() const
{ 
  return left() <= right() && bottom() <= top(); 
}