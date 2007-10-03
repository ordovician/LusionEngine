#include <Geometry/Trapezoid2.hpp>

#include <iostream>

using namespace std;

// Globals
static int gTag = 0;

// Helper functions
int opposite(Trapezoid2::Side side) {
  if (side == Trapezoid2::LEFT)
    return Trapezoid2::RIGHT;
  else if (side == Trapezoid2::RIGHT)
    return Trapezoid2::LEFT;

  cerr << __func__ << " bad side" << endl;    
  return -1;
}

// Debug
static int nextTag()
{
  return gTag++;
}

/*!
    \class Trapezoid2 Trapezoid2.h
    \brief Describes a 2d line segment.

    \mainclass

*/

// Constructors
Trapezoid2::Trapezoid2()
{
  iNeighbor[0] = 0;
  iNeighbor[1] = 0;
  iNeighbor[2] = 0;
  iNeighbor[3] = 0;  
  iNode = 0;
  
  // Debug
  iTag = nextTag();
}

Trapezoid2::Trapezoid2(const Point2& p, const Point2& q, const Segment2& bottom, const Segment2& top) 
{
  if (q.isMin(p)) {
    iPoint[0] = q;
    iPoint[1] = p;    
  }
  else {
    iPoint[0] = p;
    iPoint[1] = q;    
  }
  iBottomS = bottom;
  iTopS = top;

  iNeighbor[0] = 0;
  iNeighbor[1] = 0;
  iNeighbor[2] = 0;
  iNeighbor[3] = 0;
  iNode = 0;
  
  // Debug
  iTag = nextTag();
}

Trapezoid2::~Trapezoid2() 
{
  delete iNeighbor[0];
  if (iNeighbor[1] != iNeighbor[0]) delete iNeighbor[1];
  if (iNeighbor[0] != iNeighbor[2]) delete iNeighbor[2];
  if (iNeighbor[3] != iNeighbor[2]) delete iNeighbor[3];
  
  // Debug
  cout << "releasing trapezoid: " << iTag << endl;
}

// Accessors
Point2 Trapezoid2::left() const
{
  return iPoint[0];
}

void Trapezoid2::setLeft(const Point2& p)
{
  iPoint[0] = p;
}

Point2 Trapezoid2::right() const
{
  return iPoint[1];
}

void Trapezoid2::setRight(const Point2& p)
{
  iPoint[1] = p;
}

Segment2 Trapezoid2::bottom() const
{
  return iBottomS;
}

Segment2 Trapezoid2::top() const
{
  return iTopS;
}

/*! Returns the point in the center of the trapezoid */
Point2 Trapezoid2::center() const
{
  real leftx = left().x();
  real rightx = right().x();
  real xmid = (rightx - leftx)*0.5+leftx;

  real topleftx = top().left().x();
  real toplefty = top().left().y();
  real topleftdx = top().right().x()-top().left().x();
  real topleftdy = top().right().y()-top().left().y();

  real bottomleftx  = bottom().left().x();
  real bottomlefty  = bottom().left().y();
  real bottomleftdx = bottom().right().x()-bottom().left().x();
  real bottomleftdy = bottom().right().y()-bottom().left().y();
    
  real upperLeftY = toplefty + 
    topleftdy*(xmid-topleftx)/topleftdx;
    
  real bottomLeftY = bottomlefty + 
    bottomleftdy*(xmid-bottomleftx)/bottomleftdx;
  
  real ymid = bottomLeftY+(upperLeftY-bottomLeftY)*0.5;
  
  return Point2(xmid, ymid);
}

/*! Returns the point in the center of the trapezoid */
Point2 Trapezoid2::centerLeft() const
{
  real leftx = left().x();

  real topleftx = top().left().x();
  real toplefty = top().left().y();
  real topleftdx = top().right().x()-top().left().x();
  real topleftdy = top().right().y()-top().left().y();

  real bottomleftx  = bottom().left().x();
  real bottomlefty  = bottom().left().y();
  real bottomleftdx = bottom().right().x()-bottom().left().x();
  real bottomleftdy = bottom().right().y()-bottom().left().y();
    
  real upperLeftY = toplefty + 
    topleftdy*(leftx-topleftx)/topleftdx;
    
  real bottomLeftY = bottomlefty + 
    bottomleftdy*(leftx-bottomleftx)/bottomleftdx;
  
  real ymid = bottomLeftY+(upperLeftY-bottomLeftY)*0.5;
  
  return Point2(leftx, ymid);
}

Point2 Trapezoid2::centerRight() const
{
  real rightx = right().x();

  real topleftx = top().left().x();
  real toplefty = top().left().y();
  real topleftdx = top().right().x()-top().left().x();
  real topleftdy = top().right().y()-top().left().y();

  real bottomleftx  = bottom().left().x();
  real bottomlefty  = bottom().left().y();
  real bottomleftdx = bottom().right().x()-bottom().left().x();
  real bottomleftdy = bottom().right().y()-bottom().left().y();
    
  real upperLeftY = toplefty + 
    topleftdy*(rightx-topleftx)/topleftdx;
    
  real bottomLeftY = bottomlefty + 
    bottomleftdy*(rightx-bottomleftx)/bottomleftdx;
  
  real ymid = bottomLeftY+(upperLeftY-bottomLeftY)*0.5;
  
  return Point2(rightx, ymid);
}

Point2 Trapezoid2::endPoint(int index)
{
  assert(index == 0 || index == 1);
  return iPoint[index];
}

void Trapezoid2::setEndPoint(int index, const Point2& p)
{
  assert(index == 0 || index == 1);
  iPoint[index] = p;
}

void Trapezoid2::setNeighbors(Trapezoid2 * ll, Trapezoid2 * ul, Trapezoid2 * lr, Trapezoid2 *ur)
{
  iNeighbor[0] = ll;
  iNeighbor[1] = ul;
  iNeighbor[2] = lr;
  iNeighbor[3] = ur;
}

void Trapezoid2::setRightNeighbors(Trapezoid2 * lower, Trapezoid2 * upper)
{
  iNeighbor[2] = lower;
  iNeighbor[3] = upper;
}

void Trapezoid2::setLeftNeighbors(Trapezoid2 * lower, Trapezoid2 * upper)
{
  iNeighbor[0] = lower;
  iNeighbor[1] = upper;
}

void Trapezoid2::setLeftNeighbor(Trapezoid2* t)
{
  iNeighbor[0] = t;
  iNeighbor[1] = t;
}

void Trapezoid2::setRightNeighbor(Trapezoid2* t)
{
  iNeighbor[2] = t;
  iNeighbor[3] = t;  
}

Trapezoid2 *Trapezoid2::upperLeft() const
{
  return iNeighbor[1];
}

Trapezoid2 *Trapezoid2::upperRight() const  
{
  return iNeighbor[3];
}

Trapezoid2  *Trapezoid2::lowerLeft() const
{
  return iNeighbor[0];
}

Trapezoid2  *Trapezoid2::lowerRight() const
{
  return iNeighbor[2];
}

void Trapezoid2::setLowerLeft(Trapezoid2 *t)
{
  iNeighbor[0] = t;
}

void Trapezoid2::setLowerRight(Trapezoid2 *t)
{
  iNeighbor[2] = t;
}

void Trapezoid2::setUpperLeft(Trapezoid2 *t)
{
  iNeighbor[1] = t;
}

void Trapezoid2::setUpperRight(Trapezoid2 *t)         
{
  iNeighbor[3] = t;
}

Trapezoid2* Trapezoid2::neighbor(int index) const
{
  if (index >= 0 && index < 4)
    return iNeighbor[index];
  else
    cerr << "Trapezoid2::neighbor() out of range" << endl;

  return 0;
}

void Trapezoid2::setNeighbor(int index, Trapezoid2* t)
{
  if (index >= 0 && index < 4) {
    iNeighbor[index] = t;
  }  
  else
    cerr << "Trapezoid2::setNeighbor() out of range" << endl;
  
}

void Trapezoid2::setNeighbor(Trapezoid2::Side side, int index, Trapezoid2* t)
{
  assert((index == 0 || index == 1) && (side == 0 || side == 1));
  int s = side*2;
  
  iNeighbor[s+index] = t;
}

Trapezoids2 Trapezoid2::neighbors() const
{
  Trapezoids2 ts;
  for (int i=0; i<4; ++i) {
    if (iNeighbor[i] != 0)
      ts.push_back(iNeighbor[i]);
  }
  return ts;
}

void Trapezoid2::getNeighbors(Trapezoids2& ts) const
{
  for (int i=0; i<4; ++i) {
    if (iNeighbor[i] != 0)
      ts.push_back(iNeighbor[i]);
  }
}

Trapezoids2 Trapezoid2::leftNeighbors() const
{
  return neighbors(Trapezoid2::LEFT);
}

Trapezoids2 Trapezoid2::rightNeighbors() const
{
  return neighbors(Trapezoid2::RIGHT);
}

Trapezoids2 Trapezoid2::neighbors(Trapezoid2::Side side) const
{
  Trapezoids2 ts;
  int s = side*2;
  
  if (iNeighbor[s] != 0)
    ts.push_back(iNeighbor[s]);
  if (iNeighbor[s+1] != 0 && iNeighbor[s+1] != iNeighbor[s])
    ts.push_back(iNeighbor[s+1]);
  return ts;
}

Trapezoid2 *Trapezoid2::neighbor(Trapezoid2::Side side, int index) const
{
  int s = side*2;
  return iNeighbor[s+index];
}


void Trapezoid2::setNode(TrapezoidNode2* node)
{
  iNode = node;
}
    
TrapezoidNode2* Trapezoid2::node() const
{
  return iNode;
}  

/*!
  A trapezoid might have 0, 1 or 2 neighboring trapezoids on either side. This method returns how many
  The method is not dependent on pointers to neighbors being set correctly. However
  top and bottom segment as well as left and right point must be set properly.
*/
int Trapezoid2::noLeftNeighbors() const
{
  // Test if bottom and top endpoints coincide to the left.
  if (iTopS.left() == iBottomS.left())
    return 0;
    
  if (iTopS.left() == iPoint[0] || iBottomS.left() == iPoint[0])
    return 1;
    
  // Left endpoint is neither at bottom or top segment so there must be two neighbors
  return 2;
}

/*!
  A trapezoid might have 0, 1 or 2 neighboring trapezoids on either side. This method returns how many
  The method is not dependent on pointers to neighbors being set correctly. However
  top and bottom segment as well as left and right point must be set properly.
*/
int Trapezoid2::noRightNeighbors() const
{
  // Test if bottom and top endpoints coincide to the right.
  if (iTopS.right() == iBottomS.right())
    return 0;
    
  if (iTopS.right() == iPoint[1] || iBottomS.right() == iPoint[1])
    return 1;
    
  // Right endpoint is neither at bottom or top segment so there must be two neighbors
  return 2;
}

// Trapezoid2::iterator Trapezoid2::begin()
// {
//   return Trapezoid2::iterator(this);
// }
// 
// Trapezoid2::iterator Trapezoid2::end()
// {
//   return Trapezoid2::iterator(this, 4);
// }


// Request
bool Trapezoid2::isBordering(Trapezoid2* t) const
{
  for (int i=0; i<4; ++i) {
    if (iNeighbor[i] == 0)
      continue;
    if (iNeighbor[i] == t)
      return true;
  }
  return false;
}


// Calculations
real Trapezoid2::distance(Trapezoid2* t) const
{
  return (t->center()-center()).length();
}

/*!
  Clones this trapezoid but changes its bottom and top segment and left and right point.
  All neighbor trapezoids get their reference to 'this' trapezoids changed to the clone trapezoid
*/
Trapezoid2* Trapezoid2::clone(const Point2& p, const Point2& q, const Segment2& bottom, const Segment2& top) const
{
  cout << "CLONE: begin" << endl;
  Trapezoid2* t = new Trapezoid2(p, q, bottom, top);
  cout << t->tag() << " is clone of " << this->tag() << endl;
  // t->setNeighbors(iNeighbor[0], iNeighbor[1], iNeighbor[2], iNeighbor[3]);
  // Trapezoids2 ts = t->neighbors();
  // Trapezoids2::iterator i;
  // for (i = ts.begin(); i != ts.end(); ++i) {
  //   Trapezoid2* n = *i;
  //   for (int j=0; j<4; ++j) {
  //     if (n->neighbor(j) == this) {
  //       cout << "set " << n->tag() << "'s neighbor " << j << " which was " << n->neighbor(j)->tag() << " to " << t->tag() << endl;
  //       n->setNeighbor(j, t);        
  //     }
  //   }
  // }
  for (int side = 0; side<2; ++side) {
    for (int i=0; i<2; ++i) {
      if (neighbor(side, i) != 0) {
        t->connect(side, neighbor(side, i));        
      }
    }
  }
  cout << "CLONE: end" << endl;  
  return t;
}

// operations
/*! 
  'ct' and 'st' are two neighboring trapezoids. We only know they are to
  the left and right of each other. We don't know if they are on the uppper or lower
  part. 'ct' is the center trapezoid meaning the position of 'st' (side trapezoid) is
  relative to 'ct' given by 'side'. So if 'side' = LEFT, then 'st' is on the left side of 'ct'.
*/
void Trapezoid2::connect(int side, Trapezoid2* st)
{
  Trapezoid2* ct = this;
  assert((side == 0 || side == 1) && ct != 0 && st != 0);
  
  if (ct->top() == st->top()) {
    cout << "CONNECT: " << ct->tag() << " side " << side << " upper to "  << st->tag() << endl; // DEBUG
    cout << "CONNECT: " << st->tag() << " side " << opposite(side) << " upper to "  << ct->tag() << endl; // DEBUG    
    ct->setNeighbor(side, 1, st);
    st->setNeighbor(opposite(side), 1, ct);
  }
  if (ct->bottom() == st->bottom()) {
    cout << "CONNECT: " << ct->tag() << " side " << side << " lower to "  << st->tag() << endl; // DEBUG
    cout << "CONNECT: " << st->tag() << " side " << opposite(side) << " loewr to "  << ct->tag() << endl; // DEBUG    

    ct->setNeighbor(side, 0, st);
    st->setNeighbor(opposite(side), 0, ct);
  }  
}

/*! 
  Makes sure that there are no left over references to old trapezoid 'old_t'
  among trapezoids in sequence 'begin' to 'end'. 'old_t' can legally be NULL.
*/
void Trapezoid2::cleanup(Trapezoids2::iterator begin, Trapezoids2::iterator end)
{
  Trapezoid2* old_t = this;
  // cout << "CLEANUP: start " << old_t->tag() << endl; // DEBUG    
  Trapezoids2::iterator i;
  for (i = begin; i != end; ++i) {
    Trapezoid2* t = *i;
    // cout << "checking " << t->tag() << endl;
    int j;
    for (j=0; j<4; ++j) 
      if (t->neighbor(j) == old_t) {
        // cout << "clearing neighbor " << j <<  " of " << old_t->tag() << endl; // DEBUG
        t->setNeighbor(j, 0);                
      }      

    for (j=0; j<4; j+=2)
      if (t->neighbor(j) != t->neighbor(j+1))
        if (t->neighbor(j) == 0)
          t->setNeighbor(j, t->neighbor(j+1));
        else if (t->neighbor(j+1) == 0)
          t->setNeighbor(j+1, t->neighbor(j));           
  } 
  // cout << "CLEANUP: end" << endl; // DEBUG       
}

// Debug
void Trapezoid2::setTag(int tag)
{
  iTag = tag;
}

int Trapezoid2::tag() const
{
  return iTag;
}
