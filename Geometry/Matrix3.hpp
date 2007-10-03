/******************************************************************
Name	: Matrix3
Author	: Erik Engheim
Date	: 09/06/2005
Desc	: Class for matrix calculations typically used in geometric
          calculations like rotate, scale, transform etc. This is a
          4x4 matrix. 
Comment	: This class has not been tested yet.
*******************************************************************/

#pragma once

#include <Core/Core.h>
#include <Geometry/Vector3.hpp>

class Matrix3
{
public:
    // Constructors
    Matrix3();
    Matrix3( const Matrix3 &aOrigin );
    ~Matrix3();
    
    // Calculations
    Matrix3	getInvert() const;
    Matrix3	getTranspose() const;    
    Vector3	transformLoc( const Vector3& aVec ) const;
    Vector3	transformVec( const Vector3& aVec ) const;    
    real	det() const;
        
    // Operations
    void	invert();
    void	transpose();
    
    // Operators
    Matrix3&		operator+=( const Matrix3& aM );
    Matrix3&		operator-=( const Matrix3& aM );
    Matrix3&		operator*=( const Matrix3& aM );
    Matrix3&		operator*=( real aS );    
    
    friend Matrix3	operator-( const Matrix3& aL, const Matrix3& aR ); 
    friend Matrix3	operator+( const Matrix3& aL, const Matrix3& aR ); 
    friend Matrix3	operator*( const Matrix3& aL, const Matrix3& aR ); 
    friend Vector3	operator*( const Matrix3& aL, const Vector3& aR ); 
    friend Matrix3	operator*( const Matrix3& aL, real aR );     
    
    // Static
    static Matrix3	zero();
    static Matrix3	identity();
    static Matrix3	translate( real aX, real aY, real aZ );
    static Matrix3	scale( real aX, real aY, real aZ );
    static Matrix3	rotate( const Vector3& aAxis, real aAngle );
    static Matrix3	rotateX( real aAngle );
    static Matrix3	rotateY( real aAngle );
    static Matrix3	rotateZ( real aAngle );
    static Matrix3	view( const Vector3& aEye, const Vector3& aGaze, const Vector3& aUp );
public:    
    // Make member variables public for optimization reasons
    real	iX[4][4];
};

// 3x3 matrix determinant helper function
inline real det3( real a, real b, real c,
                   real d, real e, real f, 
                   real g, real h, real i)
{
    return a*e*i+d*h*c+g*b*f+g*e*c+d*b*i+a*h*f;
}
