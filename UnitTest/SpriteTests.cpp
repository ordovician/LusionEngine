/*
 *  SpriteTests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.11.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "SpriteTests.h"

#include "Base/Sprite.h"
#include "Base/CircleShape.h"
#include "Base/RectShape2.h"
#include "Base/SegmentShape2.h"
#include "Base/ShapeGroup.h"
#include "Base/Action.h"
#include "Base/Group.h"

#include "Core/AutoreleasePool.hpp"

#include "MockView.h"

#include <algorithm>

using namespace std;

static real t = 0.0f;
static real dt = 1.0f;

struct TestCmd : public CollisionAction {
  bool execute(Shape* me, Shape* other, Points2& points, real start_time, real delta_time) {
    CPTAssert(me == expectedMe);
    CPTAssert(other == expectedOther);
    return true;
  }
  
  Shape* expectedMe, *expectedOther;
};

SpriteTests::SpriteTests(TestInvocation *invocation)
    : TestCase(invocation)
{
}


SpriteTests::~SpriteTests()
{
}

void SpriteTests::testIntersections()
{
  AutoreleasePool::begin();

  Sprite* sprite = new Sprite(new MockView);
  
  // Test circle vs sprite
  CircleShape* c1 = new CircleShape(Circle(Vector2(0.0f, 0.0f), 4.0f));
  CircleShape* c2 = new CircleShape(Circle(Vector2(2.0f, 2.0f), 4.0f)); // overlap
  CircleShape* c3 = new CircleShape(Circle(Vector2(10.0f, 0.0f), 2.0f)); // outside
        
  CPTAssert(sprite->collide(c1, t, dt));    
  CPTAssert(sprite->collide(c2, t, dt));      
  CPTAssert(!sprite->collide(c3, t, dt));    
    
  // Test sprite with rectangles
  RectShape2* r1 = new RectShape2(Rect2(Vector2(0.0f, 0.0f), Vector2(6.0f, 6.0f)));
  RectShape2* r2 = new RectShape2(Rect2(Vector2(6.0f, 6.0f), Vector2(10.0f, 10.0f)));
    
  CPTAssert(sprite->collide(r1, t, dt));    
  CPTAssert(!sprite->collide(r2, t, dt));    

  // Test sprite with segments
  Segment2 seg1 = Segment2(Vector2(-1.0f, 0.0f), Vector2(1.0f, 0.0f));
  Segment2 seg2 = Segment2(Vector2(-1.0f, 5.0f), Vector2(1.0f, 5.0f)); 
  Segment2 seg3 = Segment2(Vector2(-0.5f, 0.0f), Vector2(-0.5f, 1.0f));   

  SegmentShape2* sshape1 = new SegmentShape2(seg1);
  SegmentShape2* sshape2 = new SegmentShape2(seg2);
  SegmentShape2* sshape3 = new SegmentShape2(seg3);
    
  CPTAssert(sprite->collide(sshape1, t, dt));    
  CPTAssert(!sprite->collide(sshape2, t, dt));    
  CPTAssert(sprite->collide(sshape3, t, dt));    
    
  sprite->release();
  sshape1->release();
  sshape2->release();
  sshape3->release();    
  
  r1->release();
  r2->release();    
  
  AutoreleasePool::end();  
}

void SpriteTests::testTrickyIntersections()
{
  AutoreleasePool::begin();
  Sprite* space_ship = new Sprite(new MockView);
  space_ship->setPosition(Vector2(0.0f, 0.0f));
  // Obstacle
  Polygon2 points;
  points.push_back(Vector2(36.714285714286f, 29.857142857143f));
  points.push_back(Vector2(40.285714285714f,	29.714285714286f));
  points.push_back(Vector2(40.142857142857f,	1.0f));
  points.push_back(Vector2(30.0f, 0.85714285714286f));
  points.push_back(Vector2(30.428571428571f, 13.285714285714f));
  
  Sprite* obstacle = new Sprite(new MockView(points));
        
  CPTAssert(!space_ship->collide(obstacle, t, dt));    
  CPTAssert(!obstacle->collide(space_ship, t, dt));    
    
  space_ship->setPosition(Vector2(9.7f, -2.7f));  

  CPTAssert(!space_ship->collide(obstacle, t, dt));    
  CPTAssert(!obstacle->collide(space_ship, t, dt));    


  space_ship->setPosition(Vector2(11.9f, 7.71f));  

  CPTAssert(!space_ship->collide(obstacle, t, dt));    
  CPTAssert(!obstacle->collide(space_ship, t, dt));    


  space_ship->setPosition(Vector2(11.99f, 7.89f));  

  CPTAssert(!space_ship->collide(obstacle, t, dt));    
  CPTAssert(!obstacle->collide(space_ship, t, dt));    

  Group* g = new Group;
  g->addKid(obstacle);
  ShapeGroup* group = new ShapeGroup(g->iterator());
  
  CPTAssert(!space_ship->collide(group, t, dt));      
  CPTAssert(!group->collide(space_ship, t, dt));      


  space_ship->setPosition(Vector2(9.7f, -2.7f));  

  CPTAssert(!space_ship->collide(group, t, dt));    
  CPTAssert(!group->collide(space_ship, t, dt));    
      
      
  space_ship->setPosition(Vector2(11.9f, 7.71f));  

  CPTAssert(!space_ship->collide(group, t, dt));    
  CPTAssert(!group->collide(space_ship, t, dt)); 
        
  g->release();
  group->release();
  space_ship->release();  
  obstacle->release();    
  AutoreleasePool::end();  
}

void SpriteTests::testMoving()
{
  AutoreleasePool::begin();  

  Sprite* sprite = new Sprite(new MockView);

  Rect2 bbox1(Vector2(-1.0f, -1.0f), Vector2(1.0f, 1.0f));    
  CPTAssert(sprite->boundingBox() == bbox1);  
    
  sprite->setSpeed(2.0f);
  sprite->update(t, 5.0f);


  Rect2 bbox2(Vector2(9.0f, -1.0f), Vector2(11.0f, 1.0f));
  
  CPTAssert(sprite->position() == Vector2(10.0f, 0.0f));
  CPTAssert(sprite->boundingBox() == bbox2);
  sprite->release();
  AutoreleasePool::end();  
}

void SpriteTests::testHierarchyIntersect()
{
  AutoreleasePool::begin();

  vector<Shape*> sprites;
  
  View* view = new MockView;
  for (int i=0; i<5; ++i) {
    Sprite* sp = new Sprite(view);
    sp->setPosition(Vector2(4.0f*i, 4.0f*i));
    sprites.push_back(sp);
  }
  
  ShapeGroup* group = new ShapeGroup(sprites.begin(), sprites.end());
  
  // To test that bounding box is correct
  Rect2 r1(Vector2(-1.0f, -1.0f), Vector2(17.0f, 17.0f));    
  
  TestCmd cmd;
  
  Sprite* sprite = new Sprite(view);
  cmd.expectedMe = sprites[4];
  cmd.expectedOther = sprite;
  
  sprite->setPosition(Vector2(16.5f, 15.5f));  
  CPTAssert(group->collide(sprite, t, dt, &cmd)); 
  sprite->setPosition(Vector2(19.0f, 19.0f));
  CPTAssert(!group->collide(sprite, t, dt));      
  CPTAssert(group->boundingBox() == r1);
  
  sprite->release();
  group->release();
  view->release();

  AutoreleasePool::end();  
}

/**
 * This was indentified as a trouble area. Numbers were produced from
 * recording points which caused trouble in simulator.
 */
void SpriteTests::testSpecialIntersect()
{
  AutoreleasePool::begin();
  Polygon2 ship_points;
  ship_points.push_back(Vector2(8.59, -3.69));
  ship_points.push_back(Vector2(10.59, -2.69));
  ship_points.push_back(Vector2(8.59, -1.69));
   
  MockView* view = new MockView(ship_points);
  Sprite* space_ship = new Sprite(view);
  space_ship->setPosition(Vector2(0.0f, 0.0f));
  
  // Obstacle
  Polygon2 points;
//  points.push_back(Vector2(16.7143, 9.85714));
//  points.push_back(Vector2(20.2857, 9.71429));
//  points.push_back(Vector2(20.1429, -19));
//  points.push_back(Vector2(10, -19.1429));
//  points.push_back(Vector2(10.4286, -6.71429));

  points.push_back(Vector2(10.4286, -6.71429));
  points.push_back(Vector2(10, -19.1429));
  points.push_back(Vector2(20.1429, -19));
  points.push_back(Vector2(20.2857, 9.71429));        
  points.push_back(Vector2(16.7143, 9.85714));

  Sprite* obstacle = new Sprite(new MockView(points));
  obstacle->setPosition(Vector2(0.0f, 0.0f));
        
  CPTAssert(!space_ship->collide(obstacle, t, dt));    
  CPTAssert(!obstacle->collide(space_ship, t, dt));    
    
  space_ship->release();  
  obstacle->release(); 
  AutoreleasePool::end();  
}

static SpriteTests test1(TEST_INVOCATION(SpriteTests, testIntersections));
static SpriteTests test2(TEST_INVOCATION(SpriteTests, testTrickyIntersections));
static SpriteTests test3(TEST_INVOCATION(SpriteTests, testMoving));
static SpriteTests test4(TEST_INVOCATION(SpriteTests, testHierarchyIntersect));
static SpriteTests test5(TEST_INVOCATION(SpriteTests, testSpecialIntersect));