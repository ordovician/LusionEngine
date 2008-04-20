/*
 *  BspTree.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 13.4.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

class BspNode
{
public:
  // Constructors
  BspNode(const Segment2& seg);
  BspNode(Segments2::iterator begin, Segments2::iterator end);  
  virtual ~BspNode();

  // Accessors

  // Request

  // Operations

private:
  /* data */
};


class BspTree
{
public:
  BspTree();
  ~BspTree();
  
  // Operators
  BspNode* createNode(Segments2::iterator begin, Segments2::iterator end);
};