/******************************************************************
Name	: Matrix2
Author	: Erik Engheim
Date	: 09/06/2005
Desc	: Class for matrix calculations typically used in geometric
          calculations like rotate, scale, transform etc. This is a
          3x3 matrix. 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>

class Matrix2
{
public:
    // Constructors
    Matrix2();
    Matrix2( const Matrix2 &aOrigin );
    ~Matrix2();
    
    // Accessors
    real at(int row, int col) const;
    void setAt(int row, int col, real value);
    
    // Calculations
    Matrix2	getTranspose() const;    
    Vector2	transformPoint( const Point2& aPoint ) const;
    Vector2	transformVector( const Vector2& aVec ) const;    
        
    // Operations
    void	transpose();
    
    // Operators
    Matrix2&		operator+=( const Matrix2& aM );
    Matrix2&		operator-=( const Matrix2& aM );
    Matrix2&		operator*=( const Matrix2& aM );
    Matrix2&		operator*=( real aS );    
    Vector2	    operator()(const Vector2& aR ); 
    
    
    friend Matrix2	operator-( const Matrix2& aL, const Matrix2& aR ); 
    friend Matrix2	operator+( const Matrix2& aL, const Matrix2& aR ); 
    friend Matrix2	operator*( const Matrix2& aL, const Matrix2& aR ); 
    friend Vector2	operator*( const Matrix2& aL, const Vector2& aR );     
    friend Matrix2	operator*( const Matrix2& aL, real aR );     
    
    // Static
    static Matrix2	zero();
    static Matrix2	identity();
    static Matrix2	translate( real aX, real aY );
    static Matrix2	translate(const Vector2& v);    
    static Matrix2	scale( real aX, real aY );
    static Matrix2	rotate( real aAngle );
public:    
    // Make member variables public for optimization reasons
    real	iX[3][3];
};