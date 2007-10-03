#include <Geometry/IO.hpp>

std::ostream& operator<< ( std::ostream & aOut, const Vector2& aV )
{
  aOut << "(" << aV.x() << ", " << aV.y() << ")";
  return aOut;
}

std::ostream& operator<< ( std::ostream & aOut, const Segment2& aS )
{
  aOut << "(" << aS.source() << ", " << aS.target() << ")";
  return aOut;
}

std::ostream& operator<< ( std::ostream & aOut, const Rect2& aR )
{
  aOut << "left: " << aR.left() << " right: " << aR.right();
  aOut << " bottom: " << aR.bottom() << " top: " << aR.top();  
  return aOut;
}
