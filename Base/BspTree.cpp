/*
 *  BspTree.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 13.4.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "BspTree.h"

// Operators
BspNode* BspTree::createNode(Segments2::iterator begin, Segments2::iterator end) {
  int no_segs = end - begin;
  
  if (no_segs <= 1) 
    return new BspNode(begin, end);

  Segments2::iterator it = begin;
  Segments2 segs_positive, segs_negative, segs_intersect;
  Line2 l(it->origin(), it->direction());
  
  for (it = begin; it != end; ++it) {
    if (l.isPositive(*it)) {
      segs_positive.push_back(*it);
      BspNode* t_positive = createNode(segs_positive);
    }
    else if (l.isNegative(*it)) {
      segs_negative(*it);
      BspNode* t_negative = createNode(segs_negative);            
    }
    else
      segs_intersect.push_back(*it);
  }
  BspNode* t = new BspNode(t_positive, t_negative, segs_intersect);
  
  return t;
}
