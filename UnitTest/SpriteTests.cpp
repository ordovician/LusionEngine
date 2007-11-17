/*
 *  SpriteTests.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.11.07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "SpriteTests.h"

#include "Base/Sprite.h"
#include "Base/CircleShape.h"
#include "Base/RectShape2.h"
#include "Base/SegmentShape2.h"
#include "Base/ShapeGroup.h"
#include "Base/Command.h"

#include "Core/AutoreleasePool.hpp"

#include "MockView.h"

static real t = 0.0f;
static real dt = 1.0f;

struct TestCmd : public SpriteCommand {
  bool execute(Shape* me, Shape* other, real start_time, real delta_time) {
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
  
}

void SpriteTests::testMoving()
{
  Sprite* sprite = new Sprite(new MockView);

  Rect2 bbox1(Vector2(-1.0f, -1.0f), Vector2(1.0f, 1.0f));    
  CPTAssert(sprite->boundingBox() == bbox1);  
    
  sprite->setSpeed(2.0f);
  sprite->update(t, 5.0f);


  Rect2 bbox2(Vector2(9.0f, -1.0f), Vector2(11.0f, 1.0f));
  
  CPTAssert(sprite->position() == Vector2(10.0f, 0.0f));
  CPTAssert(sprite->boundingBox() == bbox2);
  sprite->release();
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

static SpriteTests test1(TEST_INVOCATION(SpriteTests, testIntersections));
static SpriteTests test3(TEST_INVOCATION(SpriteTests, testMoving));
static SpriteTests test4(TEST_INVOCATION(SpriteTests, testHierarchyIntersect));