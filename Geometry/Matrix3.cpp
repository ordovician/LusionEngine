#include <Geometry/Matrix3.hpp>

#include <cmath>

/*!
    \class Matrix3 Matrix3.h
    \brief 4 by 4 matrix for doing transformations on homogenous coordinates.

    \mainclass

    Class for matrix calculations typically used in geometric
    calculations like rotate, scale, transform etc. This is a
    4x4 matrix.  
*/


// Constructors
Matrix3::Matrix3() 
{

}

/*!
  Copy constructor.
*/
Matrix3::Matrix3( const Matrix3 &aOrigin ) 
{
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            iX[i][j] = aOrigin.iX[i][j];
}


Matrix3::~Matrix3() 
{

}



// Calculations
Matrix3	Matrix3::getInvert() const 
{
    Matrix3 ret = *this;
    ret.invert();
    return ret;
}


Matrix3	Matrix3::getTranspose() const 
{
    Matrix3 ret = *this;
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            ret.iX[i][j] = iX[j][i];
    return ret;
}

/*!
  Transforms point \a aVec using this vector.
*/    
Vector3	Matrix3::transformLoc( const Vector3& aVec ) const 
{
    return (*this)*aVec;
}

/*!
  Transforms vector \a aVec using this vector.
*/
Vector3	Matrix3::transformVec( const Vector3& aVec ) const 
{
    Vector3 ret;
    const Matrix3& m = *this;
    ret[0] = aVec[0]*m.iX[0][0]+aVec[1]*m.iX[0][1]+aVec[2]*m.iX[0][2];
    ret[1] = aVec[0]*m.iX[1][0]+aVec[1]*m.iX[1][1]+aVec[2]*m.iX[1][2];
    ret[2] = aVec[0]*m.iX[2][0]+aVec[1]*m.iX[2][1]+aVec[2]*m.iX[2][2];    
    return ret;
}

/*!
  Calculates the determinant of the matrix.
*/
real	Matrix3::det() const 
{
    real d;
    d = iX[0][0]*det3(iX[1][1], iX[1][2], iX[1][3],
                      iX[2][1], iX[2][2], iX[2][3],
                      iX[3][1], iX[3][2], iX[3][3]);
    d -= iX[0][1]*det3(iX[1][0], iX[1][2], iX[1][3],
                       iX[2][0], iX[2][2], iX[2][3],
                       iX[3][0], iX[3][2], iX[3][3]);
    d += iX[0][2]*det3(iX[1][0], iX[1][1], iX[1][3],
                       iX[2][0], iX[2][1], iX[2][3],
                       iX[3][0], iX[3][1], iX[3][3]);
    d -= iX[0][3]*det3(iX[1][0], iX[1][1], iX[1][2],
                       iX[2][0], iX[2][1], iX[2][2],
                       iX[3][0], iX[3][1], iX[3][2]);
    return d;
}


    
// Operations
void	Matrix3::invert() 
{
    real d = det();
    Matrix3 inverse;
    inverse.iX[0][0]	= det3(iX[1][1], iX[1][2], iX[1][3],
                               iX[2][1], iX[2][2], iX[2][3],
                               iX[3][1], iX[3][2], iX[3][3]) / d;    
    inverse.iX[0][1]	= -det3(iX[0][1], iX[0][2], iX[0][3],
                               iX[2][1], iX[2][2], iX[2][3],
                               iX[3][1], iX[3][2], iX[3][3]) / d;        
    inverse.iX[0][2]	= det3(iX[0][1], iX[0][2], iX[0][3],
                               iX[1][1], iX[1][2], iX[1][3],
                               iX[3][1], iX[3][2], iX[3][3]) / d;                
    inverse.iX[0][3]	= -det3(iX[0][1], iX[0][2], iX[0][3],
                               iX[1][1], iX[1][2], iX[1][3],
                               iX[2][1], iX[2][2], iX[2][3]) / d;            
    
    inverse.iX[1][0]	= -det3(iX[1][0], iX[1][2], iX[1][3],
                               iX[2][0], iX[2][2], iX[2][3],
                               iX[3][0], iX[3][2], iX[3][3]) / d;    
    inverse.iX[1][1]	= det3(iX[0][0], iX[0][2], iX[0][3],
                               iX[2][0], iX[2][2], iX[2][3],
                               iX[3][0], iX[3][2], iX[3][3]) / d;        
    inverse.iX[1][2]	= -det3(iX[0][0], iX[0][2], iX[0][3],
                               iX[1][0], iX[1][2], iX[1][3],
                               iX[3][0], iX[3][2], iX[3][3]) / d;                
    inverse.iX[1][3]	= det3(iX[0][0], iX[0][2], iX[0][3],
                               iX[1][0], iX[1][2], iX[1][3],
                               iX[2][0], iX[2][2], iX[2][3]) / d;            
    
    
    inverse.iX[2][0]	= det3(iX[1][0], iX[1][1], iX[1][3],
                               iX[2][0], iX[2][1], iX[2][3],
                               iX[3][0], iX[3][1], iX[3][3]) / d;    
    inverse.iX[2][1]	= -det3(iX[0][0], iX[0][1], iX[0][3],
                               iX[2][0], iX[2][1], iX[2][3],
                               iX[3][0], iX[3][1], iX[3][3]) / d;        
    inverse.iX[2][2]	= det3(iX[0][0], iX[0][1], iX[0][3],
                               iX[1][0], iX[1][1], iX[1][3],
                               iX[3][0], iX[3][1], iX[3][3]) / d;                
    inverse.iX[2][3]	= -det3(iX[0][0], iX[0][1], iX[0][3],
                               iX[1][0], iX[1][1], iX[1][3],
                               iX[2][0], iX[2][1], iX[2][3]) / d;            
    
    
    inverse.iX[3][0]	= -det3(iX[1][0], iX[1][1], iX[1][2],
                               iX[2][0], iX[2][1], iX[2][2],
                               iX[3][0], iX[3][1], iX[3][2]) / d;    
    inverse.iX[3][1]	= det3(iX[0][0], iX[0][1], iX[0][2],
                               iX[2][0], iX[2][1], iX[2][2],
                               iX[3][0], iX[3][1], iX[3][2]) / d;        
    inverse.iX[3][2]	= -det3(iX[0][0], iX[0][1], iX[0][2],
                               iX[1][0], iX[1][1], iX[1][2],
                               iX[3][0], iX[3][1], iX[3][2]) / d;                
    inverse.iX[3][3]	= det3(iX[0][0], iX[0][1], iX[0][2],
                               iX[1][0], iX[1][1], iX[1][2],
                               iX[2][0], iX[2][1], iX[2][2]) / d;            
    *this = inverse;
}


void	Matrix3::transpose() 
{
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) { 
            real tmp = iX[i][j];
            iX[i][j] = iX[j][i];
            iX[j][i] = tmp;
        }
}



// Operators
Matrix3&    Matrix3::operator+=( const Matrix3& aM ) 
{
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            iX[i][j] += aM.iX[i][j];
    return *this;
}


Matrix3&    Matrix3::operator-=( const Matrix3& aM ) 
{
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            iX[i][j] -= aM.iX[i][j];
    return *this;
}


Matrix3&    Matrix3::operator*=( const Matrix3& aM ) 
{
    Matrix3 tmp = *this;
    for (int i=0; i<4; ++i) {         
        for (int j=0; j<4; ++j) {
            real sum = 0;
            for (int k=0; k<4; ++k)
                sum += tmp.iX[i][k] * aM.iX[k][j];
            iX[i][j] = sum;
        }
    }
    return *this;
}


Matrix3&    Matrix3::operator*=( real aS ) 
{
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            iX[i][j] *= aS;
    return *this;
}

    

Matrix3	    operator-( const Matrix3& aL, const Matrix3& aR ) 
{
    Matrix3 ret;
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            ret.iX[i][j] = aL.iX[i][j]-aR.iX[i][j];
    return ret;
}

 
Matrix3	    operator+( const Matrix3& aL, const Matrix3& aR ) 
{
    Matrix3 ret;
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            ret.iX[i][j] = aL.iX[i][j]+aR.iX[i][j];
    return ret;
}

 
Matrix3	    operator*( const Matrix3& aL, const Matrix3& aR ) 
{
    Matrix3 ret;
    for (int i=0; i<4; ++i) {         
        for (int j=0; j<4; ++j) {
            real sum = 0;
            for (int k=0; k<4; ++k)
                sum += aL.iX[i][k] * aR.iX[k][j];
            ret.iX[i][j] = sum;
        }
    }
    return ret;
}

 
Vector3	    operator*( const Matrix3& aL, const Vector3& aR ) 
{
    Vector3 ret;
    real temp;
    ret[0] = aR[0]*aL.iX[0][0]+aR[1]*aL.iX[0][1]+aR[2]*aL.iX[0][2]+aL.iX[0][3];
    ret[1] = aR[0]*aL.iX[1][0]+aR[1]*aL.iX[1][1]+aR[2]*aL.iX[1][2]+aL.iX[1][3];
    ret[2] = aR[0]*aL.iX[2][0]+aR[1]*aL.iX[2][1]+aR[2]*aL.iX[2][2]+aL.iX[2][3];    
    temp   = aR[0]*aL.iX[3][0]+aR[1]*aL.iX[3][1]+aR[2]*aL.iX[3][2]+aL.iX[3][3];    
    return ret;

}

 
Matrix3	    operator*( const Matrix3& aL, real aR ) 
{
    Matrix3 ret;
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            ret.iX[i][j] = aL.iX[i][j]*aR;
    return ret;

}

     

// Static
Matrix3	Matrix3::zero() 
{
    Matrix3 ret;
    for (int i=0; i<4; ++i) 
        for (int j=0; j<4; ++j) 
            ret.iX[i][j] = 0.0;
    return ret;
}


Matrix3	Matrix3::identity() 
{
    Matrix3 ret;
    ret.iX[0][0] = 1.0;
    ret.iX[0][1] = 0.0;
    ret.iX[0][2] = 0.0;
    ret.iX[0][3] = 0.0;
    ret.iX[1][0] = 0.0;    
    ret.iX[1][1] = 1.0;
    ret.iX[1][2] = 0.0;
    ret.iX[1][3] = 0.0;
    ret.iX[2][0] = 0.0;
    ret.iX[2][1] = 0.0;
    ret.iX[2][2] = 1.0;
    ret.iX[2][3] = 0.0;    
    ret.iX[3][0] = 0.0;
    ret.iX[3][1] = 0.0;
    ret.iX[3][2] = 0.0;
    ret.iX[3][3] = 1.0;    
    return ret;
}


Matrix3	Matrix3::translate( real aX, real aY, real aZ ) 
{
    Matrix3 ret = identity();
    ret.iX[0][3] = aX;
    ret.iX[1][3] = aY;
    ret.iX[2][3] = aZ;
    
    return ret;
}


Matrix3	Matrix3::scale( real aX, real aY, real aZ ) 
{
    Matrix3 ret = zero();
    ret.iX[0][0] = aX;
    ret.iX[1][1] = aY;
    ret.iX[2][2] = aZ;
    ret.iX[3][3] = 1.0;
    
    return ret;

}


Matrix3	Matrix3::rotate( const Vector3& aAxis, real aAngle ) 
{
    Vector3 axis = aAxis.unit();
    Matrix3 ret;
    real x = axis.x();
    real y = axis.y();
    real z = axis.z();    
    real cosine = cos(aAngle);
    real sine = sin(aAngle);
    real t = 1 - cosine;
    
    ret.iX[0][0] = t*x*x+cosine;
    ret.iX[0][1] = t*x*y+sine*y;
    ret.iX[0][2] = t*x*z+sine*y;
    ret.iX[0][3] = 0.0;
    
    ret.iX[1][0] = t*x*y+sine*z;
    ret.iX[1][1] = t*y*y+cosine;
    ret.iX[1][2] = t*y*z+sine*x;
    ret.iX[1][3] = 0.0;
    
    ret.iX[2][0] = t*x*z+sine*y;    
    ret.iX[2][1] = t*y*z+sine*x;    
    ret.iX[2][2] = t*z*z+cosine;    
    ret.iX[2][3] = 0.0;
    
    ret.iX[3][0] = 0.0;
    ret.iX[3][1] = 0.0;
    ret.iX[3][2] = 0.0;
    ret.iX[3][3] = 0.0;    
    
    return ret;
}


Matrix3	Matrix3::rotateX( real aAngle ) 
{
    Matrix3 ret = identity();
    
    real cosine = cos(aAngle);
    real sine = sin(aAngle);    
    
    ret.iX[1][1] = cosine;
    ret.iX[1][2] = -sine;
    ret.iX[2][1] = sine;
    ret.iX[2][2] = cosine;    
    
    return ret;
}


Matrix3	Matrix3::rotateY( real aAngle ) 
{
    Matrix3 ret = identity();
    
    real cosine = cos(aAngle);
    real sine = sin(aAngle);    
    
    ret.iX[0][0] = cosine;
    ret.iX[0][2] = sine;
    ret.iX[2][0] = -sine;
    ret.iX[2][2] = cosine;  
    
    return ret;
}


Matrix3	Matrix3::rotateZ( real aAngle ) 
{
    Matrix3 ret = identity();
    
    real cosine = cos(aAngle);
    real sine = sin(aAngle);    
    
    ret.iX[0][0] = cosine;
    ret.iX[0][1] = -sine;
    ret.iX[1][0] = sine;
    ret.iX[1][1] = cosine;  
    
    return ret;
}


Matrix3	Matrix3::view( const Vector3& aEye, const Vector3& aGaze, const Vector3& aUp ) 
{
    Matrix3 ret = identity();
    
    // create an orthoginal basis from parameters
    Vector3 w = -aGaze.unit();
    Vector3 u = aUp.cross(w).unit();
    Vector3 v = w.cross(u);
    
    // rotate orthogonal basis to xyz basis
    ret.iX[0][0] = u.x();
    ret.iX[0][1] = u.y();
    ret.iX[0][2] = u.z();
    ret.iX[1][0] = v.x();
    ret.iX[1][1] = v.y();
    ret.iX[1][2] = v.z();
    ret.iX[2][0] = w.x();
    ret.iX[2][1] = w.y();
    ret.iX[2][2] = w.z();
    
    // translate eye to xyz origin
    Matrix3 move = identity();
    move.iX[0][3] = -(aEye.x());
    move.iX[1][3] = -(aEye.y());
    move.iX[2][3] = -(aEye.z());    
    
    ret = ret*move;
 
    return ret; 
}


