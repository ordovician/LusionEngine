/*
 *  Sprite.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#include "Base/Sprite.h"
#include "Base/View.h"
#include "Base/Command.h"
#include "Base/Group.h"

#include "Algorithms.h"
#include "Types.h"
#include "Utils/PolygonUtils.h"
#include "Engine.h"


#include <assert.h>
#include <iostream>

using namespace std;

static int  gNextDepth = 0;
static bool gShowCollision = false;

static Point2 gPoints[] = {Point2(-1.0, -1.0), Point2(1.0, -1.0), Point2(1.0, 1.0), Point2(-1.0, 1.0)};

// Helper
void setShowCollision(bool shouldShow)
{
  gShowCollision = shouldShow;
}

bool showCollision()
{
  return gShowCollision;
}

#pragma mark Constructors
Sprite::Sprite() 
{
  init();
}

Sprite::Sprite(View* view) 
{
  init();
  setView(view);  
}

Sprite::Sprite(const Point2& aPos, real aDeg, real aSpeed)
{
  init(aPos, aDeg, aSpeed);
}

Sprite::~Sprite()
{
  cout << hex << "0x" << (int)this << " sprite removed" << endl;  // DEBUG  
  iUpdateCommand->release();
  iCollisionCommand->release();
  iInsideCommand->release();
  iState->release();
  
  GroupSet::iterator group = iGroups.begin();
  for (;group != iGroups.end(); ++group) {
    (*group)->remove(this);
  }
}

#pragma mark Initialization
void Sprite::init(const Point2& pos, real deg, real speed)
{
  iUpdateCommand = 0;
  iCollisionCommand = 0;
  iInsideCommand = 0;
  iPlanCommand = 0;
  iView = 0;
  iCurSubViewIndex = 0;
  iDepth = gNextDepth++;
  iVisible = true;
  iName = "noname";
  iPolygon = Polygon2(gPoints, gPoints+4); 

  iState = new MotionState(pos, deg, speed);

  updateCache();  
  renderGroup()->add(this); // all sprites should be in this since they need to be rendered    

  // cout << hex << "0x" << (int)this << " sprite created" << endl;  // DEBUG
}

#pragma mark Accessors
string Sprite::name()
{
  return iName;
}

void Sprite::setName(const char* name)
{
  if (name != 0) iName = name;
}

void Sprite::setPosition(const Point2& aPosition)
{
  iPrevPosition = iState->position();  
  iState->setPosition(aPosition);
  touch(); // indicate that collison poly needs to be recalculated
}

Point2 Sprite::position() const
{
	return iState->position();
}

Point2 Sprite::prevPosition() const
{
  return iPrevPosition;
}

Point2 Sprite::nextPosition() const
{
  return iState->nextPosition(secondsPerFrame());
}

void Sprite::setVelocity(const Vector2& aVelocity)
{
  iState->setVelocity(aVelocity);
  touch(); // indicate that collison poly needs to be recalculated
}

Vector2 Sprite::velocity() const
{
  return iState->velocity();
}

void Sprite::setSpeed(real aSpeed)
{
  iState->setSpeed(aSpeed);
}

real Sprite::speed() const
{
  return iState->speed();
}

void Sprite::setDirection(const Direction2& dir) 
{
  iState->setDirection(dir);
  touch(); // indicate that collison poly needs to be recalculated  
}

Direction2 Sprite::direction() const
{
  return iState->direction();
}

void Sprite::setRotation(real deg)
{
  if (deg != iState->rotation()) 
    touch(); // indicate that collison poly needs to be recalculated    
  iState->setRotation(deg);
}

real Sprite::rotation() const
{
  return iState->rotation();
}

void Sprite::setAngularVelocity(real aAngle)
{
  iState->setAngularVelocity(aAngle);
}

real Sprite::angularVelocity() const
{
  return iState->angularVelocity();
}

void Sprite::setAngularAcceleration(real aAngAccel)
{
  iState->setAngularAcceleration(aAngAccel);
}

real Sprite::angularAcceleration() const
{
  return iState->angularAcceleration();
}

void Sprite::setMotionState(MotionState* state)
{
  if (iState != state) {
    iState->release();
    iState = state;    
    state->retain();
    touch();
  }
}

MotionState* Sprite::motionState() const
{
  return iState;
}

void Sprite::setDepth(int aDepth)
{
	iDepth = aDepth;
}

int Sprite::depth() const
{
	return iDepth;
}

MutableShallowPoints2 Sprite::collisionPolygon()
{
  if (iNeedUpdate) {
    updateCache();
  }  
  return make_pair(iPolygon.begin(), iPolygon.end());  
}

ShallowPoints2 Sprite::collisionPolygon() const
{
  if (iNeedUpdate) {
    updateCache();
  }
  return make_pair(iPolygon.begin(), iPolygon.end());
}

void Sprite::setView(View* aView)
{
  if (iView != aView) {
    iView->release();
    iView = aView;
    aView->retain();
    
    ShallowPoints2 poly = iView->collisionPolygon();
    iPolygon.resize(poly.second-poly.first);
    std::copy(poly.first, poly.second, iPolygon.begin());
    updateCache();    
  }
}

View*  Sprite::view()
{
  return iView;
}

void Sprite::setSubViewIndex(int index)
{
  iCurSubViewIndex = index;
}

int Sprite::subViewIndex() const
{
  return iCurSubViewIndex;  
}

GroupSet Sprite::groups() const
{
  return iGroups;
}

real Sprite::radius() const
{
  return iView != 0 ? iView->radius() : 0;
}

Rect2 Sprite::boundingBox() const
{
  if (iNeedUpdate) {
    updateCache();
  }  
  return iBBox;
}

void Sprite::setCollisionCommand(SpriteCommand *command)
{
  if (command != iCollisionCommand) {
    iCollisionCommand->release();
    iCollisionCommand = command; 
    command->retain();       
  }
}

SpriteCommand* Sprite::collisionCommand()
{
  return iCollisionCommand;
}

void Sprite::setInsideCommand(SpriteCommand *command)
{
  if (command != iInsideCommand) {
    iInsideCommand->release();
    iInsideCommand = command; 
    command->retain();       
  }
}

SpriteCommand* Sprite::insideCommand()
{
  return iInsideCommand;
}


void Sprite::setUpdateCommand(SpriteCommand *command)
{
  if (command != iUpdateCommand) {
    iUpdateCommand->release();
    iUpdateCommand = command;    
    command->retain();       
  }
}

SpriteCommand* Sprite::updateCommand()
{
  return iUpdateCommand;
}

void Sprite::setPlanCommand(SpriteCommand *command)
{
  if (command != iPlanCommand) {
    iPlanCommand->release();
    iPlanCommand = command;    
    command->retain();       
  }
}

SpriteCommand* Sprite::planCommand()
{
  return iPlanCommand;
}

// Request
bool Sprite::viewCollide() const
{
  Rect2 view = worldView();
  return 
    view.right()-position().x() < radius() ||
    position().x()-view.left() < radius() ||
    view.top()-position().y() < radius() ||
    position().y() - view.bottom() < radius();
}	

void Sprite::setVisible(bool aVisible)
{
	iVisible = aVisible;
}

bool Sprite::visible() const
{
	return iVisible;
}

#pragma mark Calculations
bool Sprite::collide(Shape* other, real t, real dt, SpriteCommand* command) 
{
  if (!boundingBox().intersect(other->boundingBox()))
    return false;
  if (!other->isSimple())
    return other->collide(this, t, dt, command);

  ShallowPoints2 poly = collisionPolygon();    
  bool is_colliding = other->intersect(poly.first, poly.second);
  if (is_colliding && command != 0) 
    command->execute(this, other, t, dt);
  else if (is_colliding && iCollisionCommand != 0)
    iCollisionCommand->execute(this, other, t, dt);
  return is_colliding;
}

bool Sprite::inside(const Point2& p, real t, real dt,  SpriteCommand* command)
{
  if (!boundingBox().inside(p))
    return false;
  ShallowPoints2 col_poly = collisionPolygon();
  
  bool is_inside = ::inside(col_poly.first, col_poly.second, p);
  if (is_inside && command != 0)
    command->execute(this, 0, t, dt);
  else if (is_inside && iInsideCommand != 0)
    iInsideCommand->execute(this, 0, t, dt);
  return is_inside;
}

bool Sprite::intersect(const Circle& c) const
{
  ShallowPoints2 poly = collisionPolygon();
  return ::intersect(c, poly.first, poly.second);    
}

bool Sprite::intersect(const Rect2& r) const
{
  ShallowPoints2 poly = collisionPolygon();
  return ::intersect(r, poly.first, poly.second);  
}

bool Sprite::intersect(const Segment2& s) const
{
  ShallowPoints2 poly = collisionPolygon();
  return ::intersect(s, poly.first, poly.second);  
}

bool Sprite::intersect(ConstPointIterator2 begin, ConstPointIterator2 end) const
{
  ShallowPoints2 poly = collisionPolygon();
  return ::intersect(poly.first, poly.second, begin, end);  
}

static void draw(const Rect2& box)
{
  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_LINE_LOOP);
    gltVertex(box);
  glEnd();
}

static void draw(const Polygon2& poly)
{
  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_POLYGON);  
    gltVertex(poly.begin(), poly.end());
  glEnd();
}

void Sprite::draw(const Rect2& r) const
{
	if (iVisible && iView != 0 && r.intersect(boundingBox())) {
    glPushMatrix();
      if (gShowCollision)
        ::draw(iPolygon);    // To see collision polygon
      gltTranslate(position());
      if (gShowCollision)      
        drawCircle(radius());   // To see collision circle           
      glRotated(iState->rotation(), 0.0, 0.0, 1.0);
		  iView->draw(iCurSubViewIndex);	  
    glPopMatrix();    
	}
}

#pragma mark Operations
void Sprite::touch()
{
  iNeedUpdate = true;
}

bool Sprite::touched() const
{
  return iNeedUpdate;
}

void Sprite::update(real start_time, real delta_time)
{
  iPrevPosition = position();  
  advance(delta_time);
  touch();
  
  SpriteCommand* sc = updateCommand();
  if (sc)
    sc->execute(this, 0, start_time, delta_time);
  
}

void Sprite::advance(real dt)
{
  iState->advance(dt);
}

/*! Call when view changes */
void Sprite::updateCache() const
{
  iState->getCollisionPolygon(iView, iPolygon);
  iBBox = ::boundingBox(iPolygon.begin(), iPolygon.end());
  iNeedUpdate = false;
}

void Sprite::move(Vector2 movement)
{
  iState->move(movement);
}

void Sprite::accelerate(real acceleration)
{
  iState->accelerate(acceleration);
}

void Sprite::rotate(real deg)
{
  iState->rotate(deg);
}

void Sprite::reverse()
{
  iState->reverse();
}

void Sprite::stop()
{
  iState->stop();
}

void Sprite::kill()
{
  GroupSet::iterator group = iGroups.begin();
  // NOTE: not sure if iterator will be valid through this
  for (;group != iGroups.end(); ++group) {
    (*group)->remove(this);
  }
}
   
/*! This should only be called from Group.add(sprite) */
void Sprite::add(Group* group)
{
  iGroups.insert(group);
}

/*! This should only be called from Group.add(sprite) */
void Sprite::remove(Group* group)
{
  iGroups.erase(group);
}