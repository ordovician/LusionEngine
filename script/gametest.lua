require("script/roadmap")
require("script/trapezoid")
require("script/edge")

function eq(a, b)
  return math.abs(a-b) < 0.00000001
end

function testSprite1()
  print("Starting Test")
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = View:new()
  local g = Group:new()
  
  local s = {}
  -- Testing init
  for i=1,3 do
    s[i] = Sprite:new(x, y, angle, speed)
    s[i]:setView(view)
    s[i]:setName("sprite"..i)
    
    local tx,  ty = s[i]:position()
    assert(tx == x and ty == y, "Position condition failed! sprite"..i)

    local tangle = s[i]:rotation()
    assert(tangle == angle, "Angle condition failed! sprite"..i)

    local tspeed = s[i]:speed()
    assert(tspeed == speed, "Speed condition failed! sprite"..i.." "..tspeed.." == "..speed)
    g:add(s[i])
    assert(not s[i]:viewCollide(), "Screen collision test failed")
  end
  x = 5; y = 5
  s[3]:setPosition(x, y)
  
  -- Testing collision
  local start
  local ok
  local csprites
  csprites = g:spriteCollide(s[1])  
  start = Engine.ticks()  
  -- for i=1,10 do  
    csprites = g:spriteCollide(s[1])
  -- end
  print("Sprite collide millies ", Engine.ticks()-start)      
  assert(csprites[1] == s[2], "Collision test2 failed")  
  assert(g:spriteCollide(s[2])[1] == s[1], "Collision test3 failed")  
  start = Engine.ticks()
  local slist = g:groupCollide(g)
  print("Group collide millies ", Engine.ticks()-start)
  assert(slist[s[1]][1] == s[2], "Collision test4 failed")
  assert(slist[s[2]][1] == s[1], "Collision test5 failed") 
  
  assert(not s[1]:collision(s[3]), "Collision test6 failed") 
  assert(g:spriteCollide(s[3]) == nil, "Collision test7 failed")   
  assert(slist[s[3]] == nil, "Collision test8 failed") 

  -- for i=1,10 do
  start = Engine.ticks()  
  ok = s[1]:collision(s[2])
  -- end
  print("Simple collision millies ", Engine.ticks()-start)    
  assert(ok, "Collision test1 failed")
  
  -- Test movement
  local s = s[1]
  
  for i=1,4 do
    for j=1,4 do
      local dx = i
      local dy = j

      x = 0
      y = 0
      s:setPosition(x, y)
      s:setVelocity(dx, dy)

      local tdx, tdy = s:velocity()
      assert(eq(tdx,dx) and eq(tdy,dy))
      local tx, ty = s:position()
      assert(tx == x and ty == y)
  
      s:update()
      tx, ty = s:position()
      assert(eq(tx, x+dx) and eq(ty,y+dy), "Update test1: "..tx.."="..(x+dx).." "..ty.."="..(x+dy))
      s:rotate(90)
      s:update()
      tx, ty = s:position()        
      assert(eq(tx, x+dx-dy) and eq(ty, y+dy+dx), "Update test2: "..tx.."="..(x+dx-dy).." "..ty.."="..(x+dy+dx))  
      assert(not s:viewCollide(), "Screen collision test failed")      
    end
  end
  
  start = Engine.ticks()
  s:dummy(s)
  print("Dummy call time: ", Engine.ticks()-start)
  print("Ending Test")  
  -- Debug.performanceTest(50000)  

end

function testSprite2()
  print("Starting Test")
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = View:new()
  local g = Group:new()
  
  local s = {}
  -- Testing init
  for i=1,3 do
    s[i] = Sprite:new(x, y, angle, speed)
    s[i]:setView(view)
    s[i]:setName("sprite"..i)
    
    local tx,  ty = s[i]:position()
    assert(tx == x and ty == y, "Position condition failed! sprite"..i)

    local tangle = s[i]:rotation()
    assert(tangle == angle, "Angle condition failed! sprite"..i)

    local tspeed = s[i]:speed()
    assert(tspeed == speed, "Speed condition failed! sprite"..i)
    g:add(s[i])
    assert(not s[i]:viewCollide(), "Screen collision test failed")
  end
  x = 5; y = 5
  s[3]:setPosition(x, y)
  
  -- Testing collision
  local start
  local ok
  local csprites
  csprites = g:spriteCollide(s[1])  
  start = Engine.ticks()  
  -- for i=1,10 do  
    csprites = g:spriteCollide(s[1])
  -- end

end

function printPoly(poly)
  for i,p in ipairs(poly) do
    print(i,p.x, p.y)
  end
end

function collisionTest()
  -- local poly = Polygon:new(vec(1,5),vec(2,4),vec(3,3),vec(4,2),vec(5,1))
  -- sprite1 = Geometry.viewPolygonLines(poly)   
  -- local p = Debug.projectPoint(1,1, poly)
  -- 
  -- table.foreach(p, print)
  -- poly = Polygon:new(vec(0,0), vec(1,1), vec(1,2), vec(3,2))
  -- poly = Polygon:new(unpack(Debug.calcDirection(poly)))
  -- 
  -- printPoly(poly)  
  
  -- local proj = {{1,0}, {1,0.1}}
  local r1 = Polygon:rect(0, 0, 2, 2)
  local r2 = Polygon:rect(1, -1, 2, 2)
  
  local view1 = PolygonView:new(r1)
  local view2 = PolygonView:new(r2)
  view2:setColor(0,0,1)
  
  sprite1 = Sprite:new(view1)
  sprite2 = Sprite:new(view2)

  local dirs1 = Polygon:new(unpack(Debug.calcDirection(r1)))
  local dirs2 = Polygon:new(unpack(Debug.calcDirection(r2)))
  
  -- print("dirs1")
  -- dirs1:print()
  -- print("dirs2")
  -- dirs2:print()

  print(Debug.projectPolygon(dirs1, r1, r2))
  -- local p = Polygon:new(unpack(Debug.projectPoint(0,1, r1)))
  -- print(p:min())
  -- print(p:max())
  -- 
  -- p = Polygon:new(unpack(Debug.projectPoint(0,1, r2)))
  -- print(p:min())
  -- print(p:max())
  
  -- print(Debug.polygonCollision(r1, r2))
end

function testVector()
  v = vec(1, 1)
  u = vec(0, 1)

  -- print("v x", v.x, "y", v.y)
  v:print()
  u:print()
  
  v:unit():print()
end

function testSegment()
  v = vec(1, 1)
  u = vec(2, 2)
  p = vec(1.9, 1.9)
  s = seg(v, u)
  
  print(s:on(p))
  
  -- print(t.source.x, t.source.y, t.target.x, t.target.y)
end

function testNearest()
  local s3 = seg(vec(0, 0), vec(2, 1));  
  local p1 = vec(2, -2)
  local p3 = s3:nearestPoint(p1)
  segments = Geometry.viewSegments({s3})  
  points = Geometry.viewPoints({p1, p3})   
  
  -- CPTAssert((s3.nearestPoint(Vector2(1.0f, -5.0f)) - Vector2(1.0f, 1.0f)).length() < 0.01f);      
  
end

function testRect()
  r = rec(1, 1, 4, 5)
  -- r = Rect:new(1, 1, 4, 5)  
  print(r:width())
  print(r:height())    
  print(r:x())      
end

function spriteUnitTest()
  local poly = {vec(-0.6,-0.8), vec(1, 0), vec(-0.6,0.8)}
  local v = vec(0, 0)
  local u = vec(2, 2)
  local s1 = seg(v, u)
  local s2 = seg(vec(1, 0), u)  
  local segments = {s1}
  group = Group:new()
  
  local view = PolygonView:new(poly)

  local sprite = Sprite:new(u, 0, 0)
  sprite:setView(view)  
  Engine.setPlayer(sprite)
  group:add(sprite)
  
  sprite = Sprite:new(v, 0, 0)
  sprite:setView(view)
  group:add(sprite)
  
  sprite:setPosition(vec(-8,-8))
  pos = sprite:position()
  sprite:setVelocity(vec(0.1, 0.1))
  pos:print()
  -- print("Pos metatable")
  -- table.foreach(getmetatable(pos), print)
  -- print("Pos metatable")
  -- table.foreach(Vector, print)
  -- view:add(s2)
end

previous_trap = nil

function drawTrapezoids(traps)
  local segs = {}    
  for _, trap in pairs(traps) do
    for _,x in pairs({trap:left().x, trap:right().x}) do
      local vertical = seg(vec(x, ymin), vec(x, ymax))
      local bottom = trap:bottom():intersect(vertical)
      local top = trap:top():intersect(vertical)
      if bottom and top then
        table.insert(segs, seg(bottom, top))
      end
    end
    trapezoidEdges:setView(SegmentView:new(segs))
  end
end

function handleMouseClick()
  if map and Engine.mouseButton then
    local hit_trap = map:locate(Engine.mousePosition)
    if not hit_trap or hit_trap == previous_trap then return end
    previous_trap = hit_trap
    -- print("top seg:")
    -- trap:top():print()
    -- local traps = {trap:lowerLeft(), trap:lowerRight()}
    -- local traps = {trap:upperLeft(), trap:upperRight()}    
    local traps = {hit_trap}
    drawTrapezoids(traps)
    hit_trap:printNeighbors()    
    -- print("bottom "..hit_trap:bottom():tag().." top "..hit_trap:top():tag())
    print("Hit trapezoid:", hit_trap:tag()) 
    if graph then graph:setVisible(false) end
    graph = hit_trap:drawGraph()   
    -- hit_trap:printGraph()    
  end
end

function Engine.update(startime)
end

function trapezoidUnitTest()
  group = Group:new()
  local bl = vec(-4, -4)
  local br = vec(5, -4)
  local tl = vec(-5, 5)
  local tr = vec(6, 5)  
  local b = seg(bl, br)
  local t = seg(tl, tr)
  local s1 = seg(vec(-1,1), vec(3, 1))
  local s2 = seg(vec(1,2), vec(4, 4))    
  -- local segs = {b, t, s1, s2}
  local segs = {b,t, s2, s1}
  
  trapezoidEdges = Sprite:new()
  trapezoidSprites = {}

  -- map = TrapezoidalMap:new(sprite:segments(), rec(-15, -15, 15, 15))

  map = TrapezoidalMap:new(segs, rec(-15, -15, 15, 15))  
  -- map = TrapezoidalMap:new(segs, Engine.view())  

  local traps = map:trapezoids()
  local points = {}
  local segs = {}
  for _,trap in pairs(traps) do
    table.insert(trapezoidSprites, Geometry.viewTrapezoid(trap))
  end
end

function testSegs4()
  local seg1 = seg(vec(5, 3.5), vec(0.6, 1))
  local seg2 = seg(vec(0.6, 1), vec(3, -3.5))
  local seg3 = seg(vec(3, -3.5), vec(5, -3.5))  
  return {seg1, seg2, seg3}
end

function testSegs3()
  local p4 = vec(1, -2)
  local p5 = vec(-2, -2)  
  local seg3 = seg(p5, p4)  

  return {seg3}
end

function testSegs2()
  local p4 = vec(1, -2)
  local p5 = vec(-2, -1)  
  local p6 = vec(-3, -5)    
  local seg3 = seg(p5, p4)  
  local seg4 = seg(p6, p4)    

  return {seg3, seg4}
end

function testSegs1()
  local p1 = vec(0, 0)
  local p2 = vec(4, 2)
  local p3 = vec(3, -2)
  local p4 = vec(0.5, -2)
  local p5 = vec(-2, -1)  
  local p6 = vec(-3, -5)    
  local seg1 = seg(p1, p2)
  local seg2 = seg(p1, p3)
  local seg3 = seg(p5, p4)  
  local seg4 = seg(p6, p4)    

  
  return {seg1, seg2, seg3, seg4}
  -- return {seg4, seg3, seg2, seg1}  
end

function trapezoidUnitTest2()
  group = Group:new()
  trapezoidEdges = Sprite:new()
  trapezoidSprites = {}
  
  -- local w = 5;
  -- local h = 5;
  -- local w = math.random(2,10)*m;
  -- local h = math.random(2,10)*m;

    
  local params = {
    {{2, 1.5}, {-10, -6, 175,0}},
    {{3, 2}, {-15, 5, 150,0}},
    {{2.5, 4}, {-12, -1, 120,0}}
  }
  
  for _,param in pairs(params) do
    local p = Polygon:rect(unpack(param[1]))
    local view = PolygonView:new(p)
  
    local sprite = Sprite:new(unpack(param[2]))
    sprite:setView(view)
    sprite:setVisible(false)
    group:add(sprite)
  end
    
  -- view = SegmentView:new(sprite:segments())
--   view:setColor(1, 1, 0)
--   sprite = Sprite:new(0, 0, 0, 0)
--   sprite:setView(view)
--   group:add(sprite)
  
  local segs = spritesToSegments(group:sprites())
  local p1 = vec(1, 0)
  local p2 = vec(4, 0)
  local p3 = vec(-3, -4)
  -- local p4 = vec(2, -3)
  local p4 = vec(5, -4)  
  -- local seg1 = seg(p1, p2)
  -- local seg2 = seg(p3, p4)
  local seg1 = seg(p1, p2)
  local seg2 = seg(p3, p4)
  
  local seg3 = seg(vec(-5, -7), vec(6, -7))
  -- local segs = {seg1, seg2, seg3}
  -- local segs = {seg1}
  -- local segs = join({seg2}, sprite:segments(), {seg1})
  -- local segs = testSegs1()
  -- local segs = join(testSegs3(), testSegs4())  
  
  -- map = TrapezoidalMap:new(segs, rec(-15, -15, 15, 15))
  map = TrapezoidalMap:new(segs, Engine.view())  
  
  local traps = map:trapezoids()
  points = {}
  for _,trap in pairs(traps) do
    -- table.insert(points, trap:center())
    table.insert(trapezoidSprites, Geometry.viewTrapezoid(trap))
  end
  -- points = Geometry.viewPoints(points)
  
  -- local trap = traps[1]
  -- bfs(trap, segs)
  Geometry.viewSegments(segs):view():setColor(1,0,0)
end

function trapezoidUnitTest3()
  group = Group:new()

  local bl = vec(-4, -4)
  local br = vec(5, -4)
  local tl = vec(-5, 5)
  local tr = vec(6, 5)  
  local b = seg(bl, br)
  local t = seg(tl, tr)

  local t = Trapezoid:new(vec(1, 0), vec(2, 0), b, t)
  t:left():print()
  t:right():print()
  t:bottom():print()
  t:top():print()
end

function geometryUnitTest()
  group = Group:new()

  local points = {vec(1,1), vec(-1,-1), vec(1,-1), vec(0, 0), vec(0.1, 0.1), vec(0.2, 0.2), vec(-0.6, 0.8)}
  -- local p = Geometry.convexHull(points)
  -- 
  -- Geometry.viewPolygonLines(p)
  Geometry.viewPoints(points)
end

function bfs(trap, segs)
  local visisted = {}
  local parent = nil

  function visit(w)
    visisted[w:id()] = true
    -- local waypoint = {neighbors = {}, point = w:center()}
    -- waypoints[waypoint] = waypoint
  
    if (parent) then
      -- parent.neighbors[waypoint] = waypoint
      -- waypoint.neighbors[parent] = parent 
      table.insert(segs, seg(parent:center(), w:center()))
    end
  end

  local p = Stack:new()
  visit(trap)  
  p:push(trap)
  
  while not p:empty() do
    parent = p:pop()
    for _,w in pairs(parent:neighbors()) do
      if not visisted[w:id()] then
        visit(w)
        p:push(w)
      end
    end
  end
end

function lineIntersectTest()
  group = Group:new()
  -- intersection 0.5, 1
  
  -- Line intersection algo from: http://www.cgafaq.info/wiki/Intersection_Of_2D_Line_Segments
  local s1 = seg(vec(-1,1), vec(3, 1))
  local s2 = seg(vec(0.5, 0), vec(2, 3))  
    
  Geometry.viewPoints({s1:intersect(s2)}) 

  Geometry.viewSegments({s1, s2})
  -- Geometry.viewPoints({vec(div1, div2)})
  -- Geometry.viewPoints({vec(0,0), s2.source+v2:unit()*math.abs(div)})
end

function rayIntersectTest()
  group = Group:new()
  -- intersection 0.5, 1
  
  -- Line intersection algo from: http://www.cgafaq.info/wiki/Intersection_Of_2D_Line_Segments
  local p1 = vec(0.5, 0)
  local p2 = vec(2, 3)
  local s1 = seg(vec(-1,2), vec(8, 4))
  -- local s2 = seg(vec(0.5, 0), vec(2, 3))  
  local r1 = ray(p1, vec(0,1))  
  local r2 = ray(vec(3, 4), vec(0,1))  
  
  r1:toSegment():print()
  -- Geometry.viewPoints({s1:intersect(s2)}) 
  
  print(r1:segmentIntersect(s1))
  print(r2:segmentIntersect(s1))
    
  points = Geometry.viewPoints({r1:segmentIntersect(s1), r2:segmentIntersect(s1)})   

  segments = Geometry.viewSegments({s1, r1:toSegment(), r2:toSegment()})
  -- Geometry.viewPoints({vec(div1, div2)})
  -- Geometry.viewPoints({vec(0,0), s2.source+v2:unit()*math.abs(div)})
end

function circleIntersects()
  local r = Rect:new(vec(0, 0), vec(10, 10))
  local c = Circle:new(vec(5, 5), 6)
  
  print(c:rectIntersect(r))
end

function testRectIntersects()
  local r2 = Rect:new(vec(10, 10), vec(20.2, 20.2))
  r2:halfSize():print()
end

testRectIntersects()
-- circleIntersects()

-- collisionTest()

-- lineIntersectTest()
-- testNearest()
-- rayIntersectTest()

-- trapezoidUnitTest2()