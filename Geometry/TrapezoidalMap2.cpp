/*
 * Copyright 2007 Translusion. All rights reserved.
 */
#include <Geometry/TrapezoidalMap2.hpp>
#include <Geometry/TrapezoidNode2.hpp>
#include <Geometry/IO.hpp>

#include <iostream>
#include <algorithm>
#include <ext/algorithm>

using namespace std;

// typedefs
typedef map<Trapezoid2*, int> TrapezoidMap;

// globas
TrapezoidMap gRemovedTraps;

/*!
    \class TrapezoidalMap2 TrapezoidalMap2.h
    \brief 

    \mainclass

*/

// Helper function
/*!
  'old_t' is a trapezoid that has just been deleted. 't' is a trapezoid
  that was created before 'old_t' was deleted so it might have one of its
  neighbors pointing to 'old_t' because it didn't "know" that 'old_t' would
  be deleted in the future. 
  
  We call this method AFTER 't' has been properly connected to any new trapezoids
  created from splitting 'old_t'.
*/
static void merge(Trapezoid2* t, Trapezoid2* old_t)
{
  assert(t != 0 && old_t != 0);
  
  cout << "MERGE: start prev " << t->tag() << " old " << old_t->tag() << endl; // DEBUG
  // iterate over the 4 neighbors of t
  for (int side = 0; side<2; ++side) {
    for (int i=0; i<2; ++i) {
      if (t->neighbor(side, i) != old_t) 
        continue;
      Trapezoid2* neighbor = old_t->neighbor(side, i);
      if (neighbor == 0)
        t->setNeighbor(side, i, 0);
      else {
        Point2 npoint = neighbor->endPoint(opposite(side));
        Point2 tpoint = t->endPoint(side);
        // Make sure that there are no possibility that a new
        // trapezoid will be created between 't' and the neighbor of 
        // 'old_t'
        if (tpoint.isMin(npoint)) 
          t->setNeighbor(side, i, 0);
        else
          t->connect(side, neighbor);        
      }
    }    
  }
  cout << "MERGE: end" << endl;  // DEBUG
}

/*! 
  Returns the index of other left or right neighbor. E.g. if index for upperRight is given then
  index for lower right is returned and visa verca. 
*/
static int peer(int index)
{
  switch (index) {
   case 0: return 1;
   case 1: return 0;
   case 2: return 3;
   case 3: return 2;
  }
  
  cerr << __func__ << " bad index" << endl;
  return -1;
}
              
/*!
  'center_ts' are trapezoids in the center, meaning 'side_ts' trapezoids
  are always placed relative to 'center_ts'. They exist on the side given by
  'side' relative to 'center_ts'. 
  
  Example: if 'side' = LEFT then 'side_ts' trapezoids would exist on the left
  border of 'center_ts' trapezoids.
*/
static void connect(int side, Trapezoids2& center_ts, Trapezoids2& side_ts)
{
  Trapezoid2 *ct, *st; // Center and Side iterators
  Trapezoids2::iterator i,j;
  for (i = center_ts.begin(); i != center_ts.end(); ++i) {
    ct = *i;
    for (j = side_ts.begin(); j != side_ts.end(); ++j) {
      st = *j;
      ct->connect(side, st);
    }
  }
}

/*! 
  Finds all trapezoids in trapezoidal map intersected by segment 'si'
  The trapezoids are filled into range 't' to returned iterator.
*/
template <typename ForwardIterator>
ForwardIterator
followSegment(const TrapezoidalMap2* map,  const Segment2& si, ForwardIterator result)
{
  // cout << "enter follow segment" << endl; // DEBUG
  Vector2 p = si.left();
  Vector2 q = si.right();
  
  Trapezoid2* t = map->locate(si);
  if (t == 0)
    return result;
    
  // cout << "segment left endpoint " << si << " in trapezoid " << t->tag() << endl; // DEBUG
  *result = t; ++result;
  
  Vector2 rightp = t->right();
  while (q.x() > rightp.x()) {
    if (si.isBelow(rightp))
      t = t->lowerRight();
    else
      t = t->upperRight();
    if (t == 0) {
      cout << __func__ << ": encountered NULL neighbor which should not happen" << endl;
      break;
    }
    // cout << "next intersected trapezoid is "  << t->tag() << endl; // DEBUG    
    *result = t; ++result;      
    rightp = t->right(); 
  }
  // cout << "exiting follow segment" << endl;   // DEBUG
  return result;
}

Rect2 calcBoundingBox(Segments2::const_iterator begin, Segments2::const_iterator end)
{
  Segments2::const_iterator i;
  real xmin, ymin, xmax, ymax;
  
  for(i = begin; i != end; ++i) {
    xmin = (*i).xmin() < xmin ? (*i).xmin() : xmin;
    ymin = (*i).ymin() < ymin ? (*i).ymin() : ymin;    
    xmax = (*i).xmax() > xmax ? (*i).xmax() : xmax;
    ymax = (*i).ymax() > ymax ? (*i).ymax() : ymax;        
  }
  
  return Rect2(xmin, ymin, xmax-xmin, ymax-ymin);
}

// Constructors
TrapezoidalMap2::TrapezoidalMap2() : iT(0), iD(0), iDummy(0)
{
  
}

TrapezoidalMap2::TrapezoidalMap2(Segments2::const_iterator begin, Segments2::const_iterator end, const Rect2& boundingBox) 
{
  init(begin, end, boundingBox);
}

TrapezoidalMap2::TrapezoidalMap2(Segments2::const_iterator begin, Segments2::const_iterator end) 
{
  Rect2 bbox = calcBoundingBox(begin, end);
  init(begin, end, bbox);
}

TrapezoidalMap2::~TrapezoidalMap2()
{
  TrapezoidNode2::beginDelete();
    delete iD; 
  TrapezoidNode2::endDelete();
  iD = 0; 
}

void TrapezoidalMap2::init(Segments2::const_iterator begin, Segments2::const_iterator end, const Rect2& bbox)
{  
  // Create pseudo trapezoid for bounding box
  Segment2 bottom = Segment2(bbox.bottomRight(), bbox.bottomLeft());
  Segment2 top = Segment2(bbox.topRight(), bbox.topLeft());  
  iT = new Trapezoid2(bbox.bottomLeft(), bbox.topRight(), bottom, top);
  iDummy = new Trapezoid2;
  iD = newNode(newNode(iT));  // We place a head node before root node, to more easily allow replacement of rootnode in algo
  
  Segments2 r(begin, end);  
  // Segments2 r(end-begin);
  // random_sample(begin, end, r.begin(), r.end()); // create a random permutaton of set of line segments
  // int index = 1;
  Segments2::iterator i;
  for(i = r.begin(); i != r.end(); ++i) {  
    // (*i).setTag(index++); // DEBUG - to find back segment
    // cout << "Segment " << (*i).tag() << " = " << *i << endl; // DEBUG
    Trapezoids2 t;
    followSegment(this, *i, back_inserter(t));
    update(*i, t.begin(), t.end());
  } 
  
  // iD->printTree();
}

// Calculations
/*! 
  Locate trapezoid which contains point 'p'. If is at a segment endpoint
  then locate will return 'NULL'. If it is on a segment then the result is not
  defined as the calculations are not exact and might deem it above or below even if it
  is on the segment.
*/
Trapezoid2* TrapezoidalMap2::locate(const Point2& p) const
{
  TrapezoidNode2* node = iD->locate(p);
  
  if (node == 0) cout << p << " not found!" << endl;
  
  return node != 0 ? node->trapezoid() : 0;
}

/*! 
  Locate trapezoid which contains segment 's'. 's' is allowed
  to have its endpoints on the border of the trapezoid.
*/
Trapezoid2* TrapezoidalMap2::locate(const Segment2& s) const
{
  TrapezoidNode2* node = iD->locate(s);
  return node != 0 ? node->trapezoid() : 0;  
}
 
void TrapezoidalMap2::getTrapezoids(Trapezoids2& out) const
{
  iD->getTrapezoids(out);
}

void TrapezoidalMap2::remove(Trapezoid2* t)
{
  assert(t != 0);
  TrapezoidMap::iterator it = gRemovedTraps.find(t);
  if (it == gRemovedTraps.end()) {
    gRemovedTraps[t] = t->tag();
    Trapezoids2 ts;
    t->getNeighbors(ts);
    t->cleanup(ts.begin(), ts.end());

    assert(t->node() != 0);
    t->node()->replaceWith(0);    
  }
  else {
    cout << (*it).second << " trapezoid already removed! " << endl; // DEBUG
  }
}

/*!
  'begin' and 'end' denote the range of trapezoids that need to be replaced by
  new trapezoids not intersected by segment 'si'
*/
void 
TrapezoidalMap2::update(const Segment2& si, Trapezoids2::iterator begin, Trapezoids2::iterator end)
{
  // First find out which case we are dealing with
  // If we have only one trapezoid intersected that is treated specially
  if (end-begin == 1) {
    updateSingle(si, *begin);
    // cout << "single update:" << endl; // DEBUG
  }
  else if (end-begin > 1) {
    updateMultiple(si, begin, end);
    // cout << "multiple update:" << endl; // DEBUG
  }
}

static TrapezoidNode2* endSplit(int side, const Segment2& si, Trapezoid2* ti, TrapezoidNode2* si_node, TrapezoidNode2* prev_node)
{
  assert(ti != 0 && prev_node != 0 && si_node != 0 && (side == 0 || side == 1));
  
  // These are previously created trapezoids that we wish to connect to.
  Trapezoids2 ts, ns;
  si_node->getTrapezoids(ts);
  assert(ts.size() == 2);
  
  // We have two cases:
  
  // 1) endpoint of 'si' intersects point defining the side of trapezoid. No new trapezoid will then
  // be split off.
  Point2 segp = si.side(side);
  if (segp == ti->endPoint(side)) {
    // 'ts' traps is where 'ti' used to be. Second argument is thus on 'side' of 'ts'.
    // E.g. if 'side' = LEFT then second arguments is to the left of first argument.
    ns = ti->neighbors(side);
    connect(side, ts, ns);
    return prev_node;    
  }
  
  // 2) We create a new trapezoid that covers area from endpoint of 'si' to end
  // of old trapezoid.
  Trapezoid2* new_t = new Trapezoid2(segp, ti->endPoint(side), ti->bottom(), ti->top());
  cout << "endSplit: " << ti->tag() << " into " << new_t->tag() << " on side " << side << endl; // DEBUG

  // Update trapezoid map 
  Trapezoids2 new_ts;
  new_ts.push_back(new_t);
  connect(side, ts, new_ts); // If 'side' = LEFT then assume 'new_ts' to the left of 'ts'
  ns = ti->neighbors(side);
  connect(side, new_ts, ns);    
  
  // Update search structure
  TrapezoidNode2* new_node = newNode(segp);
  new_node->setChild(side, newNode(new_t));
  new_node->setChild(opposite(side), prev_node);
  
  return new_node;
}

/*!
  Split on the middle
*/
static TrapezoidNode2* midSplit(const Segment2& si, Trapezoid2* t, TrapezoidNode2* prev_node = 0)
{
  assert(t != 0);  
  
  Trapezoid2  *lower, *upper, *new_t;
  
  TrapezoidNode2* si_node = newNode(si);

  Point2 leftp = t->left();
  Point2 rightp = t->right();  
  Segment2 bottom = t->bottom();
  Segment2 top = t->top();

  // Check trapezoid is crossed fully by 'si' or not
  if (leftp.isMin(si.left()) && rightp.isMax(si.right())) {
    lower = new Trapezoid2(si.left(), si.right(), t->bottom(), si);
    upper = new Trapezoid2(si.left(), si.right(), si, t->top());

    cout << "inside midSplit: " << t->tag() << " into " << lower->tag() << " and " << upper->tag() << endl; // DEBUG
    // update search structure
    si_node->setBelow(newNode(lower));
    si_node->setAbove(newNode(upper));
  }
  else if (prev_node == 0) {    
    // NOTE: righp is not set correctly. It will be updated correctly with next trapezoid contruction. If not then si.right() is correct
    // This can only happen if we are at left side, and then si.left() is the left point
    lower = t->clone(si.left(), si.right(), bottom, si);
    upper = t->clone(si.left(), si.right(), si, top);
    
    cout << "midSplit: " << t->tag() << " into " << lower->tag() << " and " << upper->tag() << endl; // DEBUG
        
    si_node->setBelow(newNode(lower));    
    si_node->setAbove(newNode(upper));
  } 
  else {
    assert(prev_node != 0);
    assert(prev_node->above() != 0); 
    assert(prev_node->below() != 0);    
    
    TrapezoidNode2* above = prev_node->above();
    TrapezoidNode2* below = prev_node->below();
            
    // Check if left point is below 'si'

    /////////// LOWER /////////// 
    if (si.isAbove(leftp)) {    
      // New node at lower part
      // NOTE: righp is not set correctly. It will be updated correctly with next trapezoid contruction. If not then si.right() is correct
      new_t = t->clone(leftp, si.right(), bottom, si); // Retains the same neighbors as 't'
      cout << "midSplit: " << t->tag() << " into lower " << new_t->tag() << " reusing " << above->trapezoid()->tag() << endl; // DEBUG
      
      new_t->connect(Trapezoid2::LEFT, below->trapezoid());
      below->trapezoid()->setRight(leftp);

      merge(above->trapezoid(), t);
      
      // Update search structure
      si_node->setBelow(newNode(new_t));    // Lower was new
      si_node->setAbove(above);  // while upper is merged with previous upper      
    }
    /////////// UPPER /////////// 
    else if (si.isBelow(leftp)) {
      // New node at upper part
      // NOTE: righp is not set correctly. It will be updated correctly with next trapezoid contruction. If not then si.right() is correct
      new_t = t->clone(leftp, si.right(), si, top);
      cout << "midSplit: " << t->tag() << " into upper " << new_t->tag() << " reusing " << below->trapezoid()->tag() << endl; // DEBUG
      new_t->connect(Trapezoid2::LEFT, above->trapezoid());
      above->trapezoid()->setRight(leftp);
  
      merge(below->trapezoid(), t);
        
      // Update search structure      
      si_node->setAbove(newNode(new_t));                // Upper was new
      si_node->setBelow(below);  // while lower is merged with previous lower  
    }
    else {
      cerr << __func__ << ": leftp has to be above or below segment 'si'" << endl;
    }   
   
    // Cleanup - fix dangling references and NULL pointers
    Trapezoids2 ts; 
    ts.push_back(below->trapezoid());
    ts.push_back(above->trapezoid());    
    ts.push_back(new_t);
    t->cleanup(ts.begin(), ts.end());     
  }
  
  return si_node;
}

void 
TrapezoidalMap2::updateMultiple(const Segment2& si, Trapezoids2::iterator begin, Trapezoids2::iterator end)
{
  cout << "entering update multipe" << endl; // DEBUG
  
  Trapezoids2 ts;
  
  TrapezoidNode2  *node, *si_node;
  Trapezoid2* ti = *begin;
  
  cout << "entering first midSplit" << endl; // DEBUG  
  si_node = midSplit(si, ti);
  cout << "exiting first midSplit and entering first endSplit" << endl; // DEBUG    
  node = endSplit(Trapezoid2::LEFT, si, ti, si_node, si_node);
  cout << "exiting first endSplit" << endl; // DEBUG  
  
  // Fix any dangling references to old trapezoid and NULL pointers.
  node->getTrapezoids(ts);  // Get trapezoids just created (want to make sure they have no NULL pointers)
  ti->getNeighbors(ts);     // Get old neighbors, want to make sure none point to 'ti' anymore
  ti->cleanup(ts.begin(), ts.end());

  ti->node()->replaceWith(node);  
  
  cout << "entering midSplit loop" << endl; // DEBUG    
  Trapezoids2::iterator i;
  for (i = begin+1; i != end-1; ++i ) {
    ti  = *i;
    si_node = midSplit(si, ti, si_node);
    
    // Fix any dangling references to old trapezoid and NULL pointers.
    ts.clear();
    si_node->getTrapezoids(ts); 
    ti->getNeighbors(ts);       
    sort(ts.begin(), ts.end());   // One of the childs of 'si_node'
    unique(ts.begin(), ts.end()); // might be reused, so we make sure that there is no
    ti->cleanup(ts.begin(), ts.end()); // overlap between 'ti' nighbors and 'si_node' children
    
    ti->node()->replaceWith(si_node);
  }
  cout << "Exiting midsplit loop" << endl; // DEBUG      
  ti = *i;
  si_node = midSplit(si, ti, si_node);
  node = endSplit(Trapezoid2::RIGHT, si, ti, si_node, si_node);
  
  // Fix any dangling references to old trapezoid and NULL pointers.
  ts.clear();
  node->getTrapezoids(ts);
  ti->getNeighbors(ts);
  ti->cleanup(ts.begin(), ts.end());
  
  ti->node()->replaceWith(node);
  cout << "exiting update multipe" << endl; // DEBUG  
}

void TrapezoidalMap2::updateSingle(const Segment2& si, Trapezoid2* t)
{
  TrapezoidNode2  *node, *si_node;
  si_node = midSplit(si, t);
  node = endSplit(Trapezoid2::RIGHT, si, t, si_node, si_node);
  node = endSplit(Trapezoid2::LEFT, si, t, si_node, node);

  // Fix any dangling references to old trapezoid and NULL pointers.
  Trapezoids2 ts;
  node->getTrapezoids(ts);
  t->getNeighbors(ts);
  t->cleanup(ts.begin(), ts.end());
  
  assert(t->node() != 0);
  t->node()->replaceWith(node);
}

int TrapezoidalMap2::assignUniqueTags()
{
  // Give a unique tag to each created trapezoid
  Trapezoids2 ts;
  getTrapezoids(ts);
  for (int i=0; i<ts.size(); ++i) {
    ts[i]->setTag(i);
  }
  
  return ts.size();  
}
