#include <Geometry/Matrix2.hpp>

#include <cmath>

/*!
    \class Matrix2 Matrix2.h
    \brief 3 by 3 matrix for doing transformations on homogenous coordinates.

    \mainclass

    Class for matrix calculations typically used in geometric
    calculations like rotate, scale, transform etc. This is a
    3x3 matrix.  
*/


// Constructors
Matrix2::Matrix2() 
{

}

/*!
  Copy constructor.
*/
Matrix2::Matrix2( const Matrix2 &aOrigin ) 
{
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            iX[i][j] = aOrigin.iX[i][j];
}


Matrix2::~Matrix2() 
{

}

// Accessors
real Matrix2::at(int row, int col) const
{
  return iX[row][col];
}

void Matrix2::setAt(int row, int col, real value)
{
  iX[row][col] = value;
}

// Calculations
Matrix2	Matrix2::getTranspose() const 
{
    Matrix2 ret = *this;
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            ret.iX[i][j] = iX[j][i];
    return ret;
}

/*!
  Transforms point \a aVec using this vector.
*/    
Vector2	Matrix2::transformPoint( const Point2& aPoint ) const 
{
    return (*this)*aPoint;
}

/*!
  Transforms vector \a aVec using this vector.
*/
Vector2	Matrix2::transformVector( const Vector2& aVec ) const 
{
    Vector2 ret;
    const Matrix2& m = *this;
    ret[0] = aVec[0]*m.iX[0][0]+aVec[1]*m.iX[0][1];
    ret[1] = aVec[0]*m.iX[1][0]+aVec[1]*m.iX[1][1];
                                                  
    return ret;
}
  
// Operations
void	Matrix2::transpose() 
{
  for (int i=0; i<3; ++i) 
    for (int j=0; j<3; ++j) { 
      real tmp = iX[i][j];
      iX[i][j] = iX[j][i];
      iX[j][i] = tmp;
    }
}


// Operators
Matrix2&  Matrix2::operator+=( const Matrix2& aM ) 
{
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            iX[i][j] += aM.iX[i][j];
    return *this;
}


Matrix2&  Matrix2::operator-=( const Matrix2& aM ) 
{
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            iX[i][j] -= aM.iX[i][j];
    return *this;
}


Matrix2&  Matrix2::operator*=( const Matrix2& aM ) 
{
    Matrix2 tmp = *this;
    for (int i=0; i<3; ++i) {         
        for (int j=0; j<3; ++j) {
            real sum = 0;
            for (int k=0; k<3; ++k)
                sum += tmp.iX[i][k] * aM.iX[k][j];
            iX[i][j] = sum;
        }
    }
    return *this;
}


Matrix2&  Matrix2::operator*=( real aS ) 
{
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            iX[i][j] *= aS;
    return *this;
}

Vector2	Matrix2::operator()(const Vector2& aR )
{
  return transformPoint(aR);
}

Matrix2	operator-( const Matrix2& aL, const Matrix2& aR ) 
{
    Matrix2 ret;
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            ret.iX[i][j] = aL.iX[i][j]-aR.iX[i][j];
    return ret;
}

 
Matrix2	operator+( const Matrix2& aL, const Matrix2& aR ) 
{
    Matrix2 ret;
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            ret.iX[i][j] = aL.iX[i][j]+aR.iX[i][j];
    return ret;
}

 
Matrix2	operator*( const Matrix2& aL, const Matrix2& aR ) 
{
    Matrix2 ret;
    for (int i=0; i<3; ++i) {         
        for (int j=0; j<3; ++j) {
            real sum = 0;
            for (int k=0; k<3; ++k)
                sum += aL.iX[i][k] * aR.iX[k][j];
            ret.iX[i][j] = sum;
        }
    }
    return ret;
}

 
Vector2	operator*( const Matrix2& aL, const Vector2& aR ) 
{
    Vector2 ret;
    ret[0] = aR[0]*aL.iX[0][0]+aR[1]*aL.iX[0][1]+aL.iX[0][2];
    ret[1] = aR[0]*aL.iX[1][0]+aR[1]*aL.iX[1][1]+aL.iX[1][2];
    return ret;
}

 
Matrix2	operator*( const Matrix2& aL, real aR ) 
{
    Matrix2 ret;
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            ret.iX[i][j] = aL.iX[i][j]*aR;
    return ret;
}

     

// Static
Matrix2	Matrix2::zero() 
{
    Matrix2 ret;
    for (int i=0; i<3; ++i) 
        for (int j=0; j<3; ++j) 
            ret.iX[i][j] = 0.0;
    return ret;
}


Matrix2	Matrix2::identity() 
{
    Matrix2 ret;
    ret.iX[0][0] = 1.0;
    ret.iX[0][1] = 0.0;
    ret.iX[0][2] = 0.0;
    ret.iX[1][0] = 0.0;    
    ret.iX[1][1] = 1.0;
    ret.iX[1][2] = 0.0;
    ret.iX[2][0] = 0.0;
    ret.iX[2][1] = 0.0;
    ret.iX[2][2] = 1.0;
    return ret;
}


Matrix2	Matrix2::translate( real aX, real aY ) 
{
    Matrix2 ret = identity();
    ret.iX[0][2] = aX;
    ret.iX[1][2] = aY;
    
    return ret;
}

Matrix2	Matrix2::translate( const Vector2& v) 
{
    Matrix2 ret = identity();
    ret.iX[0][2] = v.x();
    ret.iX[1][2] = v.y();
    
    return ret;
}

Matrix2	Matrix2::scale( real aX, real aY) 
{
    Matrix2 ret = zero();
    ret.iX[0][0] = aX;
    ret.iX[1][1] = aY;
    ret.iX[2][2] = 1.0;
    
    return ret;

}

Matrix2	Matrix2::rotate( real aAngle ) 
{
    Matrix2 ret = identity();
    
    real cosine = cos(aAngle);
    real sine = sin(aAngle);    
    
    ret.iX[0][0] = cosine;
    ret.iX[0][1] = -sine;
    ret.iX[1][0] = sine;
    ret.iX[1][1] = cosine;  
    
    return ret;
}