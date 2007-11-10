/*
 *  Sprite.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */
#pragma once

#include "Types.h"
#include "Base/View.h"
#include "Base/MotionState.h"
#include "Base/Shape.h"

#include <string>

class SpriteCommand;
 
void setShowCollision(bool shouldShow);
bool showCollision();

class Sprite : public Shape
{
public:
	// Constructors
	Sprite();
  Sprite(View* view);	
  Sprite(const Point2& aPos, real aDir, real aSpeed);
	virtual ~Sprite();

  // Initialization
  void init(const Point2& pos = Point2(0.0, 0.0), real dir = 0.0, real aSpeed = 0.0);
  
  string name();
  void setName(const char* name);
  
	// Accessors  	
	void setPosition(const Point2& aPosition);
	Point2 position() const;

	Point2 prevPosition() const;
	Point2 nextPosition() const;

	void setVelocity(const Vector2& aVelocity);
	Vector2 velocity() const;
							
	void setSpeed(real aSpeed);
	real speed() const;
	
  void setDirection(const Direction2& aDir);
  Direction2 direction() const;  

  void  setRotation(real aAngle);
  real rotation() const;

  void  setAngularVelocity(real aAngle);
  real  angularVelocity() const;
    
  void setAngularAcceleration(real aAngAccel);
  real angularAcceleration() const;
    
  void setMotionState(MotionState* state);
  MotionState* motionState() const;
  
	void setDepth(int aDepth);
	int depth() const;

  MutableShallowPoints2 collisionPolygon();
  ShallowPoints2 collisionPolygon() const;   
  
  void  setView(View* aView);
  View* view();
  
  void  setSubViewIndex(int index);
  int   subViewIndex() const;
  
  GroupSet groups() const;
  real  radius() const;
  
  void  touch();
  bool  touched() const;
  
  Rect2 boundingBox() const;  
  
  void  setCollisionCommand(SpriteCommand* command);
  SpriteCommand* collisionCommand();

  void  setInsideCommand(SpriteCommand* command);
  SpriteCommand* insideCommand();
  
  void  setUpdateCommand(SpriteCommand* command);
  SpriteCommand* updateCommand();

  void  setPlanCommand(SpriteCommand* command);
  SpriteCommand* planCommand();
  
	// Request
	void  setVisible(bool aVisible);
	bool  visible() const;
  bool  viewCollide() const;
  
	// Calculations
  bool  collide(Shape* other, real t, real dt, SpriteCommand* command = 0);
  bool  inside(const Point2& p, real t, real dt, SpriteCommand* command = 0);
  
  bool  intersect(const Circle& c) const;
  bool  intersect(const Rect2& r) const;
  bool  intersect(const Segment2& s) const;
  bool  intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const;

	void	draw(const Rect2& r) const;

	// Operations
  void  update(real start_time, real delta_time);
  void  advance(real dt);	
  void  move(Vector2 movement);
  void  accelerate(real acceleration);
  void  rotate(real deg);
  void  reverse();
  void  stop();
  void  kill();
    
  // Only for internal use
  void  add(Group* group);
  void  remove(Group* group);

  // void moveTowards(Point2 pos, real speed);
private:
  void  updateCache() const;
  
private:
  string  iName;
	int     iDepth;
	bool	  iVisible;
	View*   iView;
  int     iCurSubViewIndex;

  GroupSet iGroups;

  // Commands
  SpriteCommand*  iUpdateCommand;
  SpriteCommand*  iCollisionCommand;
  SpriteCommand*  iInsideCommand;  
  SpriteCommand*  iPlanCommand;
  
  // Cached values
  Point2              iPrevPosition;
  MotionState*        iState;
  mutable Polygon2    iPolygon;     // Collision polygon
  mutable bool        iNeedUpdate;  // indicate whether collision poly needs update
  mutable Rect2       iBBox;        // Bounding box
};

struct SpriteDepth : public binary_function<Sprite*, Sprite*, bool>
{
  bool operator()(const Sprite* a, const Sprite* b) const
  { 
    return a->depth() < b->depth(); 
  }
};

typedef vector<Sprite*> SpriteList;
typedef map<Sprite*,SpriteList> SpriteMap;
typedef multiset<Sprite*, SpriteDepth> SpriteSet;

// Shallow SpriteList
typedef SpriteList::iterator SpriteListIterator;
typedef SpriteList::const_iterator ConstSpriteListIterator;
typedef pair<ConstSpriteListIterator, ConstSpriteListIterator> ConstSpriteListIteratorPair;
typedef pair<SpriteListIterator, SpriteListIterator> SpriteListIteratorPair;
typedef ConstSpriteListIteratorPair  ShallowSpriteList;
typedef SpriteListIteratorPair       MutableShallowSpriteList;

// Shallow SpriteMap
typedef SpriteMap::iterator SpriteMapIterator;
typedef SpriteMap::const_iterator ConstSpriteMapIterator;
typedef pair<ConstSpriteMapIterator, ConstSpriteMapIterator> ConstSpriteMapIteratorPair;
typedef pair<SpriteMapIterator, SpriteMapIterator> SpriteMapIteratorPair;
typedef ConstSpriteMapIteratorPair  ShallowSpriteMap;
typedef SpriteMapIteratorPair       MutableShallowSpriteMap;

// Shallow SpriteSet
typedef SpriteSet::iterator SpriteSetIterator;
typedef SpriteSet::const_iterator ConstSpriteSetIterator;
typedef pair<ConstSpriteSetIterator, ConstSpriteSetIterator> ConstSpriteSetIteratorPair;
typedef pair<SpriteSetIterator, SpriteSetIterator> SpriteSetIteratorPair;
typedef ConstSpriteSetIteratorPair  ShallowSpriteSet;
typedef SpriteSetIteratorPair       MutableShallowSpriteSet;