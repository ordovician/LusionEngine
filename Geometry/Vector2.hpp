/******************************************************************
Name	: Vector2
Author	: Erik Engheim
Date	: 07/09/2005
Desc	: Describes a 2 dimensional offset or location. 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

class Vector2
{
public:
    static Vector2 Zero;
    
public:
    // Constructors
    Vector2();
    Vector2( real aX, real aY);
    Vector2( real aAngle);    
    Vector2( const Vector2 &aVector ) {*this = aVector; }
    
    // Accessors
    real   x() const { return iE[0]; }
    real   y() const { return iE[1]; }
    void   setX( real aX ) { iE[0] = aX; }    
    void   setY( real aY ) { iE[1] = aY; }  
    void   set( real aX, real aY ) { iE[0] = aX;  iE[1] = aY; }  
    
    // Search
    real   minComponent() const;
    real   minAbsComponent() const;        
    real   maxComponent() const;
    real   maxAbsComponent() const;
    int    indexOfMinComponent() const;
    int    indexOfMinAbsComponent() const;    
    int    indexOfMaxComponent() const;    
    int    indexOfMaxAbsComponent() const;        
    
    // Calculations
    real   length() const;
    real   squaredLength() const;
    
    Vector2 unit() const;
    Vector2 normal() const;
    Vector2 minComponents( const Vector2& aV ) const;    
    Vector2 maxComponents( const Vector2& aV ) const;        
    Vector2 min( const Vector2& aV ) const;    
    Vector2 max( const Vector2& aV ) const;    
    real    dot( const Vector2& aV ) const;    
    real    cross( const Vector2& aV ) const;        
    real    tripleProduct( const Vector2& aV1,  const Vector2& aV2) const;
    Vector2 abs() const;
    real    angle() const;
    
    // Reguest
    bool  isLeft(const Vector2& v) const;
    bool  isRight(const Vector2& v) const;
    bool  isMin(const Vector2& v) const;
    bool  isMax(const Vector2& v) const;
      
    // Operations
    void    makeUnitVector();
    void    makeAbs();
    
    // Operators
    const Vector2& operator+() const;
    Vector2        operator-() const;
    real           operator[]( int aIndex ) const { return iE[aIndex]; }
    real&          operator[]( int aIndex )  { return iE[aIndex]; }    
    
    friend bool     operator==( const Vector2& aV1, const Vector2& aV2 );
    friend bool     operator!=( const Vector2& aV1, const Vector2& aV2 );   
    friend Vector2  operator+( const Vector2& aV1, const Vector2& aV2 );   
    friend Vector2  operator-( const Vector2& aV1, const Vector2& aV2 );   
    friend Vector2  operator/( const Vector2& aVec, real aScalar );   
    friend Vector2  operator*( const Vector2& aVec, real aScalar ); 
    friend Vector2  operator*( real aScalar, const Vector2& aVec );       
    
    Vector2&  operator=( const Vector2& aVec );
    Vector2&  operator+=( const Vector2& aVec );
    Vector2&  operator-=( const Vector2& aVec );
    Vector2&  operator*=( real aScalar );            
    Vector2&  operator/=( real aScalar );                
    
private:
    real iE[2];
};

typedef Vector2 Point2;
typedef Vector2 Direction2;
typedef Vector2 Size2;

typedef vector<Vector2>   Points2;
typedef vector<Vector2>   Polygon2;

typedef Points2::iterator       PointIterator2;
typedef Points2::const_iterator ConstPointIterator2;
typedef pair<PointIterator2, PointIterator2>            PointIteratorPair2;
typedef pair<ConstPointIterator2, ConstPointIterator2>  ConstPointIteratorPair2;

typedef ConstPointIteratorPair2 ShallowPoints2;
typedef PointIteratorPair2      MutableShallowPoints2;

// Search
inline 
real Vector2::minComponent() const
{
    return iE[1] < iE[0] ? iE[1] : iE[0];   
}

inline 
real Vector2::minAbsComponent() const
{
    real a = std::abs(iE[0]);
    real b = std::abs(iE[1]);    

    return b < a ? b : a;
}

inline 
real Vector2::maxComponent() const
{
    return iE[1] > iE[0] ? iE[1] : iE[0];  
}

inline 
real Vector2::maxAbsComponent() const
{
    real a = std::abs(iE[0]);
    real b = std::abs(iE[1]);    

    return b > a ? b : a;
}

inline 
int Vector2::indexOfMinComponent() const
{
    return iE[1] < iE[0] ? 1 : 0;   
}

inline 
int Vector2::indexOfMinAbsComponent() const
{
    real a = std::abs(iE[0]);
    real b = std::abs(iE[1]);    

    return b < a ? 1 : 0;
}

inline 
int Vector2::indexOfMaxComponent() const
{
    return iE[1] > iE[0] ? 1 : 0;     
}

inline 
int Vector2::indexOfMaxAbsComponent() const
{
    real a = std::abs(iE[0]);
    real b = std::abs(iE[1]);    

    return b > a ? 1 : 0;  
} 

// Calculations
inline 
real Vector2::length() const
{
    return sqrt( iE[0]*iE[0] + iE[1]*iE[1] );
}

inline 
real Vector2::squaredLength() const 
{
    return iE[0]*iE[0] + iE[1]*iE[1];
}


inline 
Vector2 Vector2::unit()  const
{
    real length = this->length();
    return (*this)/length;
}

inline 
Vector2 Vector2::normal()  const
{
    return Vector2(-iE[1], iE[0]);
}

inline 
Vector2 Vector2::minComponents( const Vector2& aV )  const
{
    return Vector2(std::min(x(), aV.x()), std::min(y(), aV.y()));
}

inline 
Vector2 Vector2::maxComponents( const Vector2& aV )  const  
{
  return Vector2(std::max(x(), aV.x()), std::max(y(), aV.y()));
}

inline 
Vector2 Vector2::min( const Vector2& aV )  const
{
    return isMin(aV) ? *this : aV;
}

inline 
Vector2 Vector2::max( const Vector2& aV )  const  
{
  return isMax(aV) ? *this : aV;    
}

inline 
real Vector2::dot( const Vector2& aV) const
{
   const Vector2& v = *this;    
   return v.x() * aV.x() + v.y() * aV.y();
}

inline 
real Vector2::cross( const Vector2& aV) const
{
   const Vector2& v = *this;    
   return v.x()*aV.y() - v.y()*aV.x();
}

inline 
Vector2 Vector2::abs() const {
    return Vector2( std::abs(iE[0]), std::abs(iE[1]));
}

inline 
real Vector2::angle() const {
  return atan2(iE[1], iE[0]);
}

// Operations
inline 
void Vector2::makeUnitVector() 
{
    *this = *this / (*this).length();
}

inline 
void Vector2::makeAbs() 
{
    iE[0] = std::abs(iE[0]);
    iE[1] = std::abs(iE[1]);
}

// Operators
inline 
const Vector2& Vector2::operator+() const 
{
    return *this;
}

inline 
Vector2 Vector2::operator-() const 
{
    return Vector2(-iE[0], -iE[1]); 
}


inline 
bool operator==( const Vector2& aV1, const Vector2& aV2 ) 
{
    if (aV1.iE[0] != aV2.iE[0]) return false;
    if (aV1.iE[1] != aV2.iE[1]) return false;
    
    return true;
}

inline bool operator!=( const Vector2& aV1, const Vector2& aV2 ) 
{
    return !(aV1 == aV2);
}

inline Vector2 operator+( const Vector2& aV1, const Vector2& aV2 ) 
{
    return Vector2(aV1.iE[0]+aV2.iE[0], aV1.iE[1]+aV2.iE[1]);
}

inline Vector2 operator-( const Vector2& aV1, const Vector2& aV2 ) 
{
    return Vector2(aV1.iE[0]-aV2.iE[0], aV1.iE[1]-aV2.iE[1]);  
}

inline Vector2 operator/( const Vector2& aVec, real aScalar ) 
{
    return Vector2(aVec.iE[0]/aScalar, aVec.iE[1]/aScalar);  
}

inline Vector2 operator*( const Vector2& aVec, real aScalar ) 
{
    return Vector2(aVec.iE[0]*aScalar, aVec.iE[1]*aScalar);     
}

inline Vector2 operator*( real aScalar, const Vector2& aVec ) 
{
    return Vector2(aVec.iE[0]*aScalar, aVec.iE[1]*aScalar);     
}


inline Vector2& Vector2::operator=( const Vector2& aVec ) 
{
    iE[0] = aVec.iE[0]; iE[1] = aVec.iE[1]; 
    return *this;
}

inline Vector2& Vector2::operator+=( const Vector2& aVec ) 
{
    *this = *this + aVec;
    return *this;
}

inline Vector2& Vector2::operator-=( const Vector2& aVec ) 
{
    *this = *this - aVec;
    return *this;                                  
}

inline Vector2& Vector2::operator*=( real aScalar ) 
{
    *this = *this * aScalar;
    return *this;    
}

inline Vector2& Vector2::operator/=( real aScalar )
{                                  
    *this = *this / aScalar;
    return *this;                                      
}

