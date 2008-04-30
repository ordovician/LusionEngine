/*
 *  QuadNode.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 25.9.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include <Geometry/QuadNode.h>

using namespace std;

// Constructors
QuadNode::QuadNode()
{
  
}

QuadNode::~QuadNode()
{
  
}

// Accessors
ShallowCircles QuadNode::discs() const
{
  return make_pair(iDiscs.begin(), iDiscs.end());
}

// Request
/*!
 True if \a pos is inside node. This does not say anything about whether
 it is inside a child node or not, simple that position exist within this
 node or a childnode.
*/
bool QuadNode::inside(const Vector2& pos) const
{
  return iBBox.inside(pos);
}

/*! 
  Locate child node containing position \a pos. Child node
  is returned in \a node. Method returns false if no node containing
  \a pos was found.
*/
bool QuadNode::findNode(const Vector2& pos, QuadNode& node) const
{
  if (!inside(pos))
    return false;
    
  vector<QuadNode>::const_iterator it;
  for (it = iKids.begin(); it != iKids.end(); ++it) {
    if (it->findNode(pos, node))
      return true;
  }
  return false;
}


