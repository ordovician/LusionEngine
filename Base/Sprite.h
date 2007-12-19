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

class CollisionAction;
 
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
  std::string typeName() const;	
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
  
  real  radius() const;
  
  void  touch();
  bool  touched() const;
  
  Rect2 boundingBox() const;  
  
  void  setCollisionAction(CollisionAction* command);
  CollisionAction* collisionAction();

  void  setInsideAction(Action* command);
  Action* insideAction();
  
  void  setUpdateAction(Action* command);
  Action* updateAction();

  void  setPlanAction(Action* command);
  Action* planAction();
  
	// Request
	void  setVisible(bool aVisible);
	bool  visible() const;
  
	// Calculations
  bool  collide(Shape* other, real t, real dt, CollisionAction* command = 0);
  bool  inside(const Point2& p, real t, real dt, Action* command = 0);
  
  bool  intersection(const Circle& c, Points2& points) const;
  bool  intersection(const Rect2& r, Points2& points) const;
  bool  intersection(const Segment2& s, Points2& points) const;
  bool  intersection(ConstPointIterator2 begin, ConstPointIterator2 end, Points2& points) const;

	void	draw(const Rect2& r) const;

	// Operations
  void  update(real start_time, real delta_time);
  void  advance(real dt);	
  void  move(Vector2 movement);
  void  accelerate(real acceleration);
  void  rotate(real deg);
  void  reverse();
  void  stop();
  
  void  handleCollision(Shape* other, Points2& points, real t, real dt);
  void  doPlanning(real t, real dt); 
  
private:
  void  updateCache() const;
  
private:
  string  iName;
	int     iDepth;
	bool	  iVisible;
	View*   iView;
  int     iCurSubViewIndex;

  // Actions
  Action*  iUpdateAction;
  CollisionAction*  iCollisionAction;
  Action*  iInsideAction;  
  Action*  iPlanAction;
  
  // Cached values
  Point2              iPrevPosition;
  MotionState*        iState;
  mutable Polygon2    iPolygon;     // Collision polygon
  mutable bool        iNeedUpdate;  // indicate whether collision poly needs update
  mutable Rect2       iBBox;        // Bounding box
};