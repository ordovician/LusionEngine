SpriteTest = {}

function eq(a, b)
  return math.abs(a-b) < 0.0000000001
end

function SpriteTest.hierarchyTests()
  Debug.testHierarchyIntersect()
end

function SpriteTest.testBoundinBox()
  local sprite = Sprite:new(PolygonView:new())

  local bbox1 = Rect:new(vec(-1, -1), vec(1, 1))
  assert(sprite:boundingBox() == bbox1)
  
  sprite:setSpeed(2)
  sprite:update(0, 5)

  local bbox2 = Rect:new(vec(9, -1), vec(11, 1))

  assert(sprite:position() == vec(10, 0))
  assert(sprite:boundingBox() == bbox2)

end

function SpriteTest.testInitialization()
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = PolygonView:new()
  local s = {}
  
  -- Testing init
  for i=1,3 do
    s[i] = Sprite:new(x, y, angle, speed)
    s[i]:setView(view)
    s[i]:setName("sprite"..i)
  
    local pos =  s[i]:position()
    assert(pos.x == x and pos.y == y, "Position condition failed! sprite"..i)

    local tangle = s[i]:rotation()
    assert(tangle == angle, "Angle condition failed! sprite"..i)

    local tspeed = s[i]:speed()
    assert(tspeed == speed, "Speed condition failed! sprite"..i.." "..tspeed.." == "..speed)
  end

end

function SpriteTest.testCollision()
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = PolygonView:new()
  local g = Group:new()

  local s = {}
  
  for i=1,3 do
    s[i] = Sprite:new(x, y, angle, speed)
    s[i]:setView(view)
    s[i]:setName("sprite"..i)
  end
  
  local t  = Engine.seconds()
  local dt = 1
  
  assert(s[1]:collide(s[2], t, dt), "Sprite collisiontest 1 failed")
  assert(s[3]:collide(s[2], t, dt), "Sprite collisiontest 2 failed")  
  assert(s[3]:collide(s[1], t, dt), "Sprite collisiontest 3 failed")    
end

function SpriteTest.testMovement()
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = PolygonView:new()  
  local s = Sprite:new(x, y, angle, speed)
  s:setView(view)
  s:setName("sprite")
  
  for i=1,4 do
    for j=1,4 do
      local dx = i
      local dy = j

      x = 0
      y = 0
      s:setPosition(x, y)
      s:setVelocity(dx, dy)

      local dv = s:velocity()
      assert(eq(dv.x, dx) and eq(dv.y, dy))
      local pos = s:position()
      assert(pos.x == x and pos.y == y)

      s:update(Engine.seconds(), 1)
      pos = s:position()
      assert(eq(pos.x, x+dx) and eq(pos.y, y+dy), "Update test1: "..pos.x.."="..(x+dx).." "..pos.y.."="..(x+dy))
      s:rotate(90)
      s:update(Engine.seconds(), 1)
      local pos = s:position()        
      assert(eq(pos.x, x+dx-dy) and eq(pos.y, y+dy+dx), "Update test2: "..pos.x.."="..(x+dx-dy).." "..pos.y.."="..(x+dy+dx))  
    end
  end
end

function SpriteTest.testTrickyCollision()
  local obstacles = dofile("script/levels/level1.lua") -- loads sprites from level file
  
  local view = PolygonView:new()    
  local npc1 = Sprite:new(view)
  local npc2 = Sprite:new(view)  
  npc1:setPosition(9.59, -2.69)
  npc2:setPosition(11.9, 7.89)      
    
  local t  = Engine.seconds()
  local dt = 5
  
  assert(not npc1:collide(obstacles, t, dt), "tricky collisiontest 1 failed")
  assert(not npc2:collide(obstacles, t, dt), "tricky collisiontest 2 failed")  
  
  obstacles = ShapeGroup:new(obstacles)

  assert(not npc1:collide(obstacles, t, dt, function(me, other, points, t, dt)
      if points then 
        Geometry.viewPoints(points)
        print("Point values: ", #points)
        for k,v in pairs(points) do print(k, v) end       
      end
      return true
    end), "tricky collisiontest 3 failed")
  assert(not npc2:collide(obstacles, t, dt), "tricky collisiontest 4 failed")  
  
end

return SpriteTest