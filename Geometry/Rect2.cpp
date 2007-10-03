/*!
    \class Rect2 Rect2.h
    \brief Represents an Axis-Aligned Bounding Box.
    \author Erik Engheim

    An Axis-Aligned Bounding Box (Rect2) specified by a minimum position and a
    maximum position. The center, size, half size, and surface area are
    computed on the fly.

    The minimum position is the smallest coordinate of the Rect2
    (bottom-left), the maximum position is the largest coordinate of the
    Rect2 (top-right).
*/

#include <Geometry/Rect2.hpp>

// Constructors
Rect2::Rect2() 
{
}

Rect2::Rect2(const Vector2 &aMin, const Vector2 &aMax) 
{
  setMin(aMin);
  setMax(aMax);
}

Rect2::Rect2(real xmin, real ymin, real xmax, real ymax) 
{
  setMin(Point2(xmin, ymin));
  setMax(Point2(xmax, ymax));
}

Rect2::~Rect2() 
{
}

// Accessors
Vector2& Rect2::min()
{ 
  return iPos[0]; 
}

const Vector2& Rect2::min() const
{ 
  return iPos[0]; 
}

void Rect2::setMin(const Vector2 &aMin) 
{ 
  iPos[0] = aMin; 
}

Vector2& Rect2::max()
{ 
  return iPos[1]; 
}

const Vector2& Rect2::max() const
{ 
  return iPos[1]; 
}

void Rect2::setMax(const Vector2 &aMax) 
{ 
  iPos[1] = aMax; 
}

Vector2 Rect2::center() const
{ 
  return (min() + max()) * 0.5f; 
}

Vector2 Rect2::size() const
{ 
  return (max() - min()).abs(); 
}

Vector2 Rect2::halfSize() const
{ 
  return size() * 0.5f; 
}

// Calculations
/*!
  Get area of rectangle
 */
real Rect2::area() const 
{
  Vector2 s = size();
  return s.x()*s.y();
}

Rect2 Rect2::surround(const Rect2& aBox) const
{
    return Rect2(min().minComponents(aBox.min()), max().maxComponents(aBox.max()));
}


Rect2 Rect2::surround(const Vector2& aV) const
{
    return Rect2(min().minComponents(aV), max().maxComponents(aV));
}

/*!
    Returns true if this Rect2 overlaps with the given Rect2.

    \param aRect2 the Rect2 to test overlap with
 */
bool Rect2::intersect(const Rect2 &aRect2) const {
    
    // d is distance and h is halfsize. Almost like intersection test for circles
    // halfsize coresponds to radius
    Vector2 d = (aRect2.center() - center()).abs();

    Vector2 h1 = halfSize();
    Vector2 h2 = aRect2.halfSize();

    return d.x() <= h1.x() + h2.x() && 
           d.y() <= h1.y() + h2.y();
}

/*!
  Point \a aP is inside this rectangle if it is either fully inside the boundary of
  the rectangle or on the boundary.
*/
bool Rect2::inside(const Point2& aP) const
{
  return min().isMin(aP) && max().isMax(aP) || (aP == min() || aP == max());
}

Rect2 Rect2::translated(real dx, real dy) const
{ 
  return translated(Vector2(dx, dy)); 
}

Rect2 Rect2::translated(const Vector2 &p) const
{ 
  return Rect2(min()+p, max()+p);
}

// Operations
void Rect2::translate(real dx, real dy)
{
  translate(Vector2(dx, dy));
}

void Rect2::translate(const Vector2 &p)
{
  setMin(min()+p);
  setMax(max()+p);  
}


void Rect2::moveTo(real ax, real ay)
{
  moveTo(Vector2(ax, ay));
}

void Rect2::moveTo(const Vector2 &p)
{
  setMax(size()+p);
  setMin(p);
}

void Rect2::moveCenter(const Vector2& p)
{
   translate(p-center());
}