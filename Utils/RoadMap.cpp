/*
 *  RoadMap.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 6.1.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "RoadMap.h"

/*! Constructs a roadmap using sequence 'begin' to 'end' as a list of obstacles in space */
RoadMap::RoadMap( SpriteList::const_iterator begin, SpriteList::const_iterator end )
{
  SpriteList::const_iterator i;
  Segments2 segments;
  for (i= begin; i != end; ++i) {
    Sprite* sprite = *i;
    Polygon2 poly = s->polygon();
    for (int j = 1; j < poly.size(); ++j) {
      segments.push_back(SpriteSegment(poly[j-1], poly[j]));     
      // segments.push_back(new SpriteSegment(poly[j-1], poly[j], sprite));     
    }
  }
  iMap = TrapezoidalMap2(segments.begin(), segments.end());
}

/*! 
 Find a path from start to target point through roadmap within time defined
 by start time. If path is not found within that time, a temporary path is returned.
 The resulting path is a sequence of Point2 stored from 'output' to returned iterator
*/
Polygon2::iterator 
RoadMap::findPath(
  const Point2& start, 
  const Point2& target, 
  int start_ticks
  Polygon2::iterator output) const
{
  
}

