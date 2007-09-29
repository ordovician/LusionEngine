/*
 *  MotionState.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#include "Base/MotionState.h"
#include "Base/View.h"
#include "Algorithms.h"
#include "Utils/PolygonUtils.h"
#include "Types.h"
#include "Engine.h"

#include <assert.h>
#include <iostream>

using namespace std;

// Helper


/*!
    \class MotionState MotionState.h
    \brief Stores the state of a moving object.

    The object has position, orientation, angular velocity and a velocity in direction
    of orientation. MotionStates have this state. The state can be used when exploring search
    trees for navigating a free space. For instane the RRT algorithm explores a state space
    consisting of 'MotionState' state space. 
    
    MotionState is also able to integrate movement over time. By that we mean that we can calculate
    final position and orientation after a specif time interval, by adding up multiple small
    changes in position and orientation caused by velocity and angular velocity.
*/

// Constructors
MotionState::MotionState() 
{
  init();
}


MotionState::MotionState(const Point2& aPos, real aDeg, real aSpeed)
{
  init(aPos, aDeg, aSpeed);
}

MotionState::~MotionState()
{
  // cout << hex << "0x" << (int)this << " motion state removed" << endl;  // DEBUG    
}

// Initialization
void MotionState::init(const Point2& pos, real deg, real speed)
{
  iSpeed = speed;
  setPosition(pos);
  setRotation(deg);
  iAngVelocity = 0.0;
  iAngAcceleration = 0.0;
}

// Accessors
void MotionState::setPosition(const Point2& aPosition)
{
  iPosition = aPosition;
}

Point2 MotionState::position() const
{
	return iPosition;
}

/*! 
  Gives next position after 'dt' time (seconds) has elapsed, assuming angular velocity is zero.
  Since angular velocity is often NOT zero. This will only be "correct" for very short time intervals
*/
Point2 MotionState::nextPosition(real dt) const
{
  return iPosition+velocity()*dt;
}

void MotionState::setVelocity(const Vector2& aVelocity)
{
  iSpeed = aVelocity.length();
  iRotation = deg(aVelocity.angle());
}

Vector2 MotionState::velocity() const
{
  return iSpeed*direction();
}

void MotionState::setSpeed(real aSpeed)
{
  iSpeed = aSpeed;
}

real MotionState::speed() const
{
  return iSpeed;
}

void MotionState::setDirection(const Direction2& dir) 
{
  iRotation = deg(dir.angle());
}

Direction2 MotionState::direction() const
{
  return Vector2(rad(iRotation));
}

void MotionState::setRotation(real deg)
{
  if (deg != iRotation) {
    iRotation = deg;
  }
}

real MotionState::rotation() const
{
  return iRotation;
}

void MotionState::setAngularVelocity(real aAngle)
{
  iAngVelocity = aAngle;
}

real MotionState::angularVelocity() const
{
  return iAngVelocity;
}

void MotionState::setAngularAcceleration(real aAngAccel)
{
  iAngAcceleration = aAngAccel;
}

real MotionState::angularAcceleration() const
{
  return iAngAcceleration;
}

// Calculations
void MotionState::getTransform(Matrix2& transform) const
{
  Matrix2 trans = Matrix2::translate(position());
  Matrix2 rot = Matrix2::rotate(rad(rotation()));
  transform = trans*rot;  
}

void MotionState::getCollisionPolygon(const View* view, Polygon2& poly) const
{
  if (view != 0) {  
    Matrix2 trans; getTransform(trans);
    ShallowPoints2 viewPoly = view->collisionPolygon();
    poly.resize(viewPoly.second - viewPoly.first);
    transform(viewPoly.first, viewPoly.second, poly.begin(), trans);
  }  
}


// Operations
/*! 
  integrates state over time 't' given in seconds using 'steps' number of steps.
  More steps is slower but more accurate. This affects number of times 'advance'
  is called.  
*/
void MotionState::integrate(real dt, int steps)
{
  for (int i=0; i<steps; ++i)
    advance(dt);
}

/*! 
  Integrate like pervious method but also create path from start to finish.
*/
void MotionState::integratePath(real dt, int steps, Polygon2& path)
{
  path.push_back(position());  
  for (int i=0; i<steps; ++i) {
    advance(dt);
    path.push_back(position());
  }
}

/*! Advance state by time interval 'dt' given in seconds */
void MotionState::advance(real dt)
{
  iRotation += iAngVelocity*dt+0.5*iAngAcceleration*dt*dt;
  iAngVelocity += iAngAcceleration*dt;  
  setPosition(nextPosition(dt));
}

void MotionState::move(Vector2 movement)
{
  setPosition(iPosition + movement);
}

void MotionState::accelerate(real acceleration)
{
  iSpeed += acceleration;
}

void MotionState::rotate(real deg)
{
  iRotation += deg;
}

void MotionState::reverse()
{
  iSpeed = -iSpeed;
}

void MotionState::stop()
{
  iSpeed = 0.0;
  iAngVelocity = 0.0;
  iAngAcceleration = 0.0;  
}
