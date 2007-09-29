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

// Constructors
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

// Initialization
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

// Accessors
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
  if (iView) {
    real r = iView->radius();
    Point2 s = Point2(r,r);
    return Rect2(position()-s, position()+s);
  }
  return Rect2();
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

bool Sprite::collision(const Sprite* other) const
{
  if (other->radius()+radius() > (position()-other->position()).length()) {
    ShallowPoints2 otherPoly = other->collisionPolygon();
    ShallowPoints2 thisPoly = collisionPolygon();
    return ::collision(otherPoly.first, otherPoly.second, thisPoly.first, thisPoly.second);
  }
  return false;
}

bool Sprite::inside(const Point2& p) const
{
  ShallowPoints2 col_poly = collisionPolygon();
  return ::inside(col_poly.first, col_poly.second, p);
}

// Calculations
bool Sprite::groupCollide(Group* other, real t, real dt, SpriteCommand* command)
{
  BinarySpriteCollisionCommand scc;
  SpriteCommand* cmd = command != 0 ? command : &scc;           
  
  ShallowSpriteSet sprites = other->sprites();
  bool is_col = false;
  ConstSpriteSetIterator it;  
  for (it = sprites.first; it != sprites.second; ++it) {
    Sprite* sprite = *it;
    if (secondsPassed() > t+dt)
      break;
    if (!collision(sprite))
      continue;

    if (cmd->execute(this, sprite, t, dt))
      is_col = true;
  }    
  return is_col;
}

bool Sprite::collide(CollisionObject* other, real t, real dt, SpriteCommand* command) 
{
  IntersectCommand scc(this);
  SpriteCommand* cmd = command != 0 ? command : &scc;
  return other->traverse(t, dt, cmd);
}

bool Sprite::intersect(const Circle& c) const
{
  if (c.intersect(boundingBox())) {
    ShallowPoints2 poly = collisionPolygon();
    return ::collision(c, poly.first, poly.second);    
  }
  return false;
}

bool Sprite::intersect(const Rect2& r) const
{
  if (r.intersect(boundingBox())) {  
    ShallowPoints2 poly = collisionPolygon();
    return ::collision(r, poly.first, poly.second);  
  }
  return false;
}

bool Sprite::intersect(const Segment2& s) const
{
  if (s.intersect(boundingBox())) {  
    ShallowPoints2 poly = collisionPolygon();
    return ::collision(s, poly.first, poly.second);  
  }
  return false;
}

bool Sprite::traverse(real t, real dt, SpriteCommand* command)
{
  assert(command != 0);
  return command->execute(this, 0, t, dt);
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

static void drawCircle(double r)
{
  glPushMatrix(); 

  glColor3d(1.0, 0.0, 1.0);      
  glBegin(GL_LINE_LOOP);
  for (double angle=0.0; angle<360.0; angle+=15.0) {
    glVertex2d(cos(rad(angle))*r, sin(rad(angle))*r);
  }
  glEnd();
  glPopMatrix();       
}

void Sprite::draw() const
{
	if (iVisible && iView != 0) {
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

// Operations
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