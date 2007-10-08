/*
 *  Group.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/Group.h"
#include "Engine.h"
#include "Base/Command.h"

#include <iostream>

using namespace std;

/*!
    \class Group Group.h
    \brief Collection for Sprites. 

    Is in many ways similar to CollisionGroup. However CollisionGroup is not mutable
    Once it is created Sprites can't be added or removed. A Group on the otherhand
    can have Sprites added and removed at any time. The downside of this is that
    the structure can be as efficient in handling collision, because one can't create
    an optimized hierarchical structure.
*/

// Gloal functions
static Group gRenderGroup;

Group* renderGroup()
{
  return &gRenderGroup;
}

// Constructors
Group::Group() : iCurSprite(0)
{
  
}

Group::~Group()
{
  cout << hex << "0x" << (int)this << " group removed" << endl;  // DEBUG  
}

// Accessors
int Group::size() const
{
  return iSprites.size();
}

ShallowSpriteSet Group::sprites() const
{
  return make_pair(iSprites.begin(), iSprites.end());
}

MutableShallowSpriteSet Group::sprites()
{
  return make_pair(iSprites.begin(), iSprites.end());
}

Rect2 Group::boundingBox() const
{
  return iBBox; // TODO: Calculate the boundingbox
}

// Request
bool Group::contains(Sprite* sprite) const
{
  return iSprites.find(sprite) != iSprites.end();
}

bool Group::isSimple() const
{
  return false;
}

// Operations
void Group::add(Sprite* sprite)
{
  if (!contains(sprite)) {
    sprite->retain();
    iCurSprite = iSprites.insert(sprite);
    sprite->add(this);
  }
}

void Group::remove(Sprite* sprite)
{
  if (contains(sprite)) {
    if (*iCurSprite == sprite) nextSprite();
    iSprites.erase(sprite);
    sprite->remove(this);
    sprite->release();
  }
}

void Group::update(real start_time, real delta_time)
{
  SpriteSet::iterator sprite = iSprites.begin();
  for (;sprite != iSprites.end(); ++sprite) {
    (*sprite)->update(start_time, delta_time);
  }
}

void Group::doPlanning(real start_time, real delta_time)
{
  Sprite* sprite = nextSprite();
  if (sprite && sprite->planCommand())
    sprite->planCommand()->execute(sprite, 0, start_time, delta_time);
}
  
void Group::draw(const Rect2& r) const
{
  SpriteSet::iterator sprite = iSprites.begin();
  for (;sprite != iSprites.end(); ++sprite) {
    (*sprite)->draw(r);
  }  
}

void Group::clear()
{
  iSprites.clear();
}

/*! 
  Does a round robin for sprites in group. That means at each call it will return
  a different sprite in the group, until all the sprites have been returned at which
  point it start returning the first sprites again.
*/
Sprite* Group::nextSprite()
{
  if (iSprites.empty())
    return 0;
  Sprite* s = *iCurSprite;
  if (++iCurSprite == iSprites.end())
    iCurSprite = iSprites.begin();
  return s;
}
  

bool Group::collide(CollisionObject* other, real t, real dt, SpriteCommand* command)
{
  bool is_col = false;
  SpriteSet::iterator it;  
  for (it = iSprites.begin(); it != iSprites.end(); ++it) {
    Sprite* sprite = *it;
    if (secondsPassed() > t+dt)
      break;
      
    if (sprite == other)
      continue;

    if(sprite->collide(other, t, dt, command))
      is_col = true;
  }    
  return is_col;
}

bool Group::inside(const Point2& p, real t, real dt, SpriteCommand* command)
{
  bool is_col = false;
  SpriteSet::iterator it;  
  for (it = iSprites.begin(); it != iSprites.end(); ++it) {
    Sprite* sprite = *it;

    if (secondsPassed() > t+dt)
      break;
      
    if(sprite->inside(p, t, dt, command))
      is_col = true;
  }    
  return is_col;  
}