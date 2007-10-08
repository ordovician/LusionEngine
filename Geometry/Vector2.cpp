#include <Geometry/Vector2.hpp>

/*!
    \class Vector2 Vector3.h
    \brief Describes a 2 dimensional offset or location. 

    Vector can be used to store a point, the vertex of a geometric object, an offset or a vector in 2D space. 
*/

Vector2 Vector2::Zero(0.0f, 0.0f);

// Constructors
Vector2::Vector2() 
{
    iE[0] = 0.0f;
    iE[1] = 0.0f;
}

Vector2::Vector2( real aAngle)
{
  iE[0] = cos(aAngle);
  iE[1] = sin(aAngle);
}

Vector2::Vector2( real aX, real aY) 
{
    iE[0] = aX;
    iE[1] = aY;
}

// Request
bool Vector2::isLeft(const Vector2& v) const
{
  return x() < v.x();
}

bool Vector2::isRight(const Vector2& v) const
{
 return x() > v.x();
}

bool Vector2::isMin(const Vector2& q) const
{
  const Vector2& p = *this;
  return p.x() < q.x() || (p.x() == q.x() && p.y() < q.y());
}

bool Vector2::isMax(const Vector2& q) const
{
  const Vector2& p = *this;  
  return p.x() > q.x() || (p.x() == q.x() && p.y() > q.y());  
}

