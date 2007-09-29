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

// Request
bool Group::contains(Sprite* sprite) const
{
  return iSprites.find(sprite) != iSprites.end();
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
  
void Group::draw()
{
  SpriteSet::iterator sprite = iSprites.begin();
  for (;sprite != iSprites.end(); ++sprite) {
    (*sprite)->draw();
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
  
bool Group::spriteCollide(Sprite* other, real t, real dt, SpriteCommand* command)
{  
  BinarySpriteCollisionCommand scc;
  SpriteCommand* cmd = command != 0 ? command : &scc;           

  bool is_col = false;
  SpriteSet::iterator it;
  for (it = iSprites.begin(); it != iSprites.end(); ++it) {
    Sprite* sprite = *it;
    if (secondsPassed() > t+dt)
      break;
    if (sprite == other || !other->collision(sprite)) 
      continue;

    if (cmd->execute(sprite, other, t, dt))
      is_col = true;
  }    
  return is_col;
}

bool Group::groupCollide(Group* other, real t, real dt, SpriteCommand* command)
{
  bool is_col = false;
  SpriteSet::iterator it;  
  for (it = iSprites.begin(); it != iSprites.end(); ++it) {
    Sprite* sprite = *it;
    if (secondsPassed() > t+dt)
      break;
    if (sprite->groupCollide(other, t, dt, command))
      is_col = true;
  }    
  return is_col;
}

bool Group::collide(CollisionObject* other, real t, real dt, SpriteCommand* command)
{
  bool is_col = false;
  SpriteSet::iterator it;  
  for (it = iSprites.begin(); it != iSprites.end(); ++it) {
    Sprite* sprite = *it;
    if (secondsPassed() > t+dt)
      break;

    IntersectCommand scc(sprite);
    SpriteCommand* cmd = command != 0 ? command : &scc;     
    if(other->traverse(t, dt, cmd))
      is_col = true;
  }    
  return is_col;
}