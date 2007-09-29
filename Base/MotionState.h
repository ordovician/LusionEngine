/*
 *  MotionState.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */
#pragma once

#include "Types.h"

#include <Core/SharedObject.hpp>

class View;
 
class MotionState : public SharedObject
{
public:
	// Constructors
	MotionState();
  MotionState(const Point2& aPos, real aDir, real aSpeed);
	virtual ~MotionState();

  // Initialization
  void init(const Point2& pos = Point2(0.0, 0.0), real dir = 0.0, real aSpeed = 0.0);
    
	// Accessors  	
	void setPosition(const Point2& aPosition);
	Point2 position() const;

  Point2 nextPosition(real dt) const;

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
  
  // Calculations
  void  getTransform(Matrix2& trans) const;
  void  getCollisionPolygon(const View* view, Polygon2& poly) const;
  
	// Operations
  // void  integrate(real t, int steps, MotionState& s);
  void  integrate(real dt, int steps);  
  void  integratePath(real dt, int steps, Polygon2& path);
  void  advance(real dt);
  void  move(Vector2 movement);
  void  accelerate(real acceleration);
  void  rotate(real deg);
  void  reverse();
  void  stop();
  
private:
  Point2      iPosition;
  real        iRotation;        // Orientation
  real        iAngVelocity;     // Angular velocity
  real        iAngAcceleration;
  real        iSpeed;           // Velocity in direction of orientation
};