#include <Geometry/TrapezoidNode2.hpp>

#include <Geometry/IO.hpp>

#include <iostream>
#include <sstream>
#include <stack>
#include <map>

#include <cassert>

using namespace std;

// typedefs
typedef map<TrapezoidNode2*, int> NodeSet;

// Globals
static int iTag = 0;
static stack<NodeSet> gDeleteStack;

// Helper

// Debug
static int nextTag()
{
  return iTag++;
}

/*!
    \class LeafNode TrapezoidNode2.h
    \brief A node in a tree used a a search tree for trapezoids in a trapezoidal map.

    A LeafNode represent nodes at the leafs of the tree. They are distinguished by
    cointaining pointers to trapezoids.
*/
class LeafNode : public TrapezoidNode2
{
public:
    // Constructors
    LeafNode(Trapezoid2* t) { assert(t != 0); iTrapezoid = t; t->setNode(this); }

    ~LeafNode()
    {
      // Before we are removed we got to inform parents that we don't exist anymore
      TrapezoidNodes2::iterator i;
      for (i = iParents.begin(); i != iParents.end(); ++i) {
        TrapezoidNode2* parent = *i;
        if (parent->left() == this)
          parent->setLeft(0);
        else if (parent->right() == this)
          parent->setRight(0);
      }
      iParents.clear();
    }

    // Request
    virtual TrapezoidNode2* find(const Point2& p)
    {
      return 0;
    } 
       
    virtual TrapezoidNode2* find(const Segment2& s)
    {
      return 0;
    }


    // Operations
    void addParent(TrapezoidNode2* parent)
    {
      iParents.push_back(parent);
    }
    
    virtual void replaceWith(TrapezoidNode2* n)
    {
      cout << "begin replace " << this->tag() << " with "; // DEBUG
      if (n)
        cout << n->tag() << endl; // DEBUG
      else
        cout << "NULL" << endl; // DEBUG
        
      TrapezoidNodes2::iterator i;
      for (i = iParents.begin(); i != iParents.end(); ++i) {
        TrapezoidNode2* parent = *i;
        if (parent->left() == this) {
          cout << "changing left of " << parent->tag() << endl; // DEBUG
          parent->setLeft(n);          
        }
        else if (parent->right() == this) {
          cout << "changing right of " << parent->tag() << endl; // DEBUG
          parent->setRight(n);              
        }
      }
      iKids[0] = 0;
      iKids[1] = 0;
      iParents.clear();
      delete this;
      cout << "end replace with" << endl; // DEBUG
    }    
    
    // Accessors
    Trapezoid2* trapezoid() const
    {
      return iTrapezoid;
    }
    
    // Debug
    virtual std::string description() const
    {
      stringstream ss;
      ss << "leaf " << tag();
      return ss.str();            
    }
    
private:
    Trapezoid2* iTrapezoid;
    TrapezoidNodes2 iParents;    
};

/*!
    \class TrapezoidNode2 TrapezoidNode2.h
    \brief A node in a tree used a a search tree for trapezoids in a trapezoidal map.

    \mainclass

*/

// Constructors
TrapezoidNode2::TrapezoidNode2() : iTag(nextTag())
{
  iKids[0] = 0;
  iKids[1] = 0;
}

TrapezoidNode2::~TrapezoidNode2()
{
  // cout << iTag << " BEGIN TrapezoidNode2 delete" << endl;
  for (int i=0; i<2; ++i) {
    if (iKids[i] == 0)
      continue;
    
    if (gDeleteStack.empty()) {
      delete iKids[i];
      continue;      
    }

    NodeSet::iterator it = gDeleteStack.top().find(iKids[i]);
    if (it == gDeleteStack.top().end()) {
      gDeleteStack.top()[iKids[i]] = iKids[i]->tag();
      delete iKids[i];
    }
  }
  
  // cout << iTag << " END TrapezoidNode2 delete" << endl; // DEBUG
}

// Accessors
void TrapezoidNode2::setLeft(TrapezoidNode2* left)
{
  setChild(0, left);
}

TrapezoidNode2*  TrapezoidNode2::left() const
{
  return iKids[0];
}

void TrapezoidNode2::setRight(TrapezoidNode2* right)
{
  setChild(1, right);
}

TrapezoidNode2*  TrapezoidNode2::right() const
{
  return iKids[1];
}

TrapezoidNodes2 TrapezoidNode2::children() const
{
  TrapezoidNodes2 tns;
  if (iKids[0] != 0)
    tns.push_back(iKids[0]);
  if (iKids[1] != 0)
    tns.push_back(iKids[1]);
  
  return tns;
}

TrapezoidNode2 *TrapezoidNode2::child(int index) const
{
  assert(index == 0 || index == 1);
  return iKids[index];
}

void TrapezoidNode2::setChild(int index, TrapezoidNode2* child)
{
  assert(index == 0 || index == 1);
  iKids[index] = child;
  if (child != 0) {
    LeafNode* lchild = dynamic_cast<LeafNode*>(child);
    if (lchild != 0)
      lchild->addParent(this);    
  }  
}


Trapezoid2*  TrapezoidNode2::trapezoid() const
{
  return 0;
}

// Calculations
TrapezoidNode2* TrapezoidNode2::locate(const Point2& p)
{
  TrapezoidNode2* node = this;
  while (node != 0 && node->trapezoid() == 0) {
    node = node->find(p);
  }
  return node;
}

TrapezoidNode2* TrapezoidNode2::locate(const Segment2& s)
{
  TrapezoidNode2* node = this;
  while (node != 0 && node->trapezoid() == 0) {
    node = node->find(s);
  }
  return node;
}

void TrapezoidNode2::getTrapezoids(Trapezoids2& out) const
{  
  // copy(beginTrapezoid(), endTrapezoid(), back_inserter(out));
  TrapezoidNodes2 visit;
  if (this->left())
    visit.push_back(this->left());
  if (this->right())
    visit.push_back(this->right());
  
  while(!visit.empty()) {
    TrapezoidNode2* n = visit.back();
    visit.pop_back();    
    if (n->left() || n->right()) {
      if (n->left())
        visit.push_back(n->left());
      if (n->right())
        visit.push_back(n->right());      
    }
    else {
      Trapezoid2* t = n->trapezoid();
      if (t)
        out.push_back(t);
    }
  }
}

// Debug
int TrapezoidNode2::tag() const
{
  return iTag;
}

static void printSubTree(const TrapezoidNode2* node)
{
  if (node->left()) {
    cout << "  \"" << node->description() << "\" -> \"" << node->left()->description() << "\"" << endl;    
    printSubTree(node->left());
  }
  if (node->right()) {
    cout << "  \"" << node->description() << "\" -> \"" << node->right()->description() << "\"" << endl;    
    printSubTree(node->right());    
  }
  
  if (node->left() == 0 && node->right() == 0 && node->trapezoid() != 0) {
    cout << "  \"" << node->description() << "\" -> \"" << node->trapezoid()->tag() << "\"" << endl;        
  }
}

void TrapezoidNode2::printTree() const
{
  cout << "digraph nodes {" << endl;
  printSubTree(this);
  cout << endl << "}" << endl;
}


// Helper classes - Internal
class SegmentNode : public TrapezoidNode2
{
public:
    // Constructors
    SegmentNode(const Segment2& s) { iSegment = s; }
    
    // Request
    virtual TrapezoidNode2* find(const Point2& p)
    {
      if (iSegment.isBelow(p))
        return left();
      return right();
    } 
       
    virtual TrapezoidNode2* find(const Segment2& s)
    {
      Point2 p = s.left();
      if (iSegment.isBelow(p))
        return left();
      if (iSegment.isAbove(p))
        return right();
      if (!iSegment.isOnEnd(p)) 
        return 0;
      if (s.slope() > iSegment.slope())
        return left();
      return right();
    }
        
    // Operations
    void replaceWith(TrapezoidNode2* n)
    {
      cerr << "SegmentNode does not support replacing!" << endl;
    }
        
    // Debug
    virtual std::string description() const
    {
      stringstream ss;
      ss << iSegment << " " << tag();
      return ss.str();            
    }
        
private:
    Segment2 iSegment;
};

class PointNode : public TrapezoidNode2
{
public:
    // Constructors
    PointNode(const Point2& p) { iPoint = p; }

    // Request
    virtual TrapezoidNode2* find(const Point2& p)
    {
      if (p.isMin(iPoint))
        return left();
      else
        return right();
    } 
       
    virtual TrapezoidNode2* find(const Segment2& s)
    {
      return find(s.left());
    }
    
    // Operations    
    void replaceWith(TrapezoidNode2* n)
    {
      cerr << "PointNode does not support replacing!" << endl;
    }
          
    // Debug
    virtual std::string description() const
    {
      stringstream ss;
      ss << iPoint << " " << tag();
      return ss.str();            
    }
            
private:
    Point2 iPoint;
};

/*!
  Hack to make sure that root node can easily be replaced. Head node will
  be right above root node.
*/
class HeadNode : public TrapezoidNode2
{
public:
    // Constructors
    HeadNode(TrapezoidNode2* node) { 
      setLeft(node);
    }

    // Accessors
    Trapezoid2* trapezoid() const
    {
      return iKids[0]->trapezoid();
    }

    // Request
    TrapezoidNode2* find(const Point2& p)
    {
      return iKids[0]->find(p);      
    }

    TrapezoidNode2* find(const Segment2& s)
    {
      return iKids[0]->find(s);      
    }
       
    // Calculations
    TrapezoidNode2* locate(const Point2& p)
    {
      return iKids[0]->locate(p);
    }

    TrapezoidNode2* locate(const Segment2& s)
    {
      return iKids[0]->locate(s);
    }
        
    void replaceWith(TrapezoidNode2* n)
    {
      cerr << "HeadNode does not support replacing!" << endl;
    }
  
    // Debug
    virtual std::string description() const
    {
      stringstream ss;
      ss << "head " << tag();
      return ss.str();            
    }
        
};

void TrapezoidNode2::beginDelete()
{
  gDeleteStack.push(NodeSet());
}

void TrapezoidNode2::endDelete()
{
  assert(!gDeleteStack.empty());
  gDeleteStack.pop();
}

// Helper functions
// Factor functions
TrapezoidNode2* newNode(const Segment2& s)
{
  return new SegmentNode(s);
}

TrapezoidNode2* newNode(const Point2& p)
{
  return new PointNode(p);
}

TrapezoidNode2* newNode(Trapezoid2* t)
{
  return new LeafNode(t);
}

TrapezoidNode2* newNode(TrapezoidNode2* n)
{
  return new HeadNode(n);
}