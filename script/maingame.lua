--[[
  Defines the code for the main game logic. Loads level
  and sets up players character and enemy airplaines. While
  running game there are a number of hotkeys that can be pressed
  to load roadmap, save it or do other things which is mainly intended
  for debugging puproses
  
  List of hotkeys:
  p - RRT path from active NPC to other
  z - switch player
  v - center view
  k - construct roadmap
  l - load roadmap
  s - save roadmap
  c - Probablistic roadmap path visualized
  h - Find choke points
]]--

-- Sprite.setShowCollision(true)
require("script/rrt")
require("script/queue")
require("script/stack")
require("script/prm")
require("script/algorithm")

function Engine.setPlayer(player)
  Engine._player = player
end

function Engine.player()
  return Engine._player
end

function Engine.updatePlayer()
  local player = Engine.player()
  local keystates = Engine.keyStates()

  if keystates[Key.UP]    then player:accelerate(0.4) end
  if keystates[Key.DOWN]  then player:accelerate(-0.4) end
  if keystates[Key.RIGHT] then player:rotate(-3.0) end
  if keystates[Key.LEFT]  then player:rotate(3.0) end
  
end

function addObstacle(poly)
  local obstacle = Sprite:new()
  local view = PolygonView:new(poly)
  view:setColor(1, 1 , 0.3)
  obstacle:setView(view)  
  obstacle:setPosition(0, 0) 
  obstacles:add(obstacle)
end

-- Creates a random number of obstacles and return the sprite group containing them
function createRandomObstacles(noObstacles)  
  local obstacles = Group:new()
  local obstacle_collection = Collection:new()
  
  for i=1,noObstacles do
    local angle = math.random(0,360)
    local speed = 0  
    local obstacle = Sprite:new()

    -- Create a random shape for view
    local m = 0.6
    local w = math.random(2,10)*m;
    local h = math.random(2,10)*m; 

    local obstacleView = PolygonView:new(Polygon:rect(w, h))        

    obstacleView:setColor(1,1,0.3)
      
    obstacle:setView(obstacleView)
    obstacle:setName("Obstacle"..i)
    obstacle:setSpeed(speed)
    obstacle:setRotation(angle)    

    local x,y
    repeat       
      x = math.random(xmin+5, xmax-5)
      y = math.random(ymin+5, ymax-5)    
      obstacle:setPosition(x,y) 

    until not obstacles:collide(obstacle, Engine.seconds(), 1)

    obstacles:add(obstacle)
    obstacle_collection:append(obstacle)
  end
  return obstacle_collection
end

function Shape:addNPC(planeView)
-- function Shape:addNPC(planeView, x, y)
  local npc = Sprite:new(planeView)
  
  function place()
    local x = math.random(xmin+5, xmax-5)
    local y = math.random(ymin+5, ymax-5)    
    npc:setPosition(x,y) 
    return true
  end
  
  place()
  npc:setCollisionHandler(place)
  for i=1,15 do
    if not npc:collide(obstacles, Engine.seconds(), 1) then
      break
    end
  end
  
  npc:setCollisionHandler(nil)
  
  npc:setName("NPC")
  self:add(npc)
  Engine.setPlayer(npc)  

  return npc
end

-- Switch between NPCs using z key
function makePlayerSwitch()
    if selectedNPC == 2 then selectedNPC = 1 else selectedNPC = 2 end
    Engine.setPlayer(npcs[selectedNPC])
end

function currentNPC()
  return npcs[selectedNPC]
end

function otherNPC()
  local other = 2
  if selectedNPC == 2 then other = 1 end
  return npcs[other]
end

function drawTrailingLine()
  local npc = currentNPC()
  local prev = npc:prevPosition()
  local pos = npc:position()
  if prev ~= pos then
    trailingLine:view():add(seg(prev, pos))
  end
end

function update(start_time)
  local delta_time = Engine.secondsPerFrame()
  Engine.handleEvents()
  Engine.updatePlayer()
  actors:update(start_time, delta_time)
  Engine.lookAt(currentNPC():position())
  -- drawTrailingLine()
  actors:doPlanning(start_time, delta_time)
  -- actors:collide(obstacles, start_time, delta_time, function(self, other, t, dt) print("collision!") end)
  actors:collide(obstacles, start_time, delta_time)
  -- if obstacles:collide(currentNPC(), start_time, Engine.secondsPerFrame()) then print("collision") end
end

function setupRRTSearch()
  local pos = vec(0, 0)
  local speed = 10
  local dir = 0
  local steps = 10
  local dt = Engine.secondsPerFrame()
  local t = dt*steps
  local s0 = MotionState:new(pos, dir, speed)

  local trajectories = Geometry.createTrajectories(s0, -300, 300, 2, dt, steps)  

  local sprite_state = Sprite:new()
  sprite_state:setVisible(false)
  sprite_state:setName("sprite state")
  
  -- Generate a path to other NPC when p is clicked
  Engine.registerKeyClickEvent(Key.p, function()
    local npc = currentNPC()
    s0 = npc:motionState():copy()
    s0:setSpeed(10)

    local start_time = Engine.seconds()
    local delta_time = 0.1

    sprite_state:setView(npc:view())
    
    function avoidCollision(s0, s1)
      sprite_state:setMotionState(s1)
      if sprite_state:collide(obstacles, start_time, delta_time) then
        return -1
      end
      return 0
    end
    
    local flank = Geometry.makeFlank(otherNPC(), 10)
    local seek = Geometry.makeSeek(otherNPC():position())
    -- local combo = Geometry.combineBehavior({1,flank})
    -- local combo = Geometry.combineBehavior({1,flank}, {0.001,seek})
    local combo = Geometry.combineBehavior({0.001,seek}, {1,flank}, {1,avoidCollision})
    
    local eval = Geometry.makeEval(combo, 0.5, 0)
    -- Create a comparator that will return true if eval(a) > eval(b)
    local larger = makeBinary(eval, greaterThan)
    local states = Geometry.rrtSearch(s0, trajectories, larger, 20, start_time, delta_time)

    if state_paths then state_paths:setVisible(false) end
    state_paths = Geometry.showStatePaths(states, dt, steps)   
    state_paths:view():setColor(0,0,1)
    
    states = Geometry.bestPath(s0, states, eval)    
    if best_path then best_path:setVisible(false) end
    best_path = Geometry.showStatePaths(states, dt, steps)   
    best_path:view():setColor(1,0,0)
  end) 
end

function setupSeek()
  local pos = vec(0, 0)
  local speed = 10
  local dir = 0
  -- local steps = 40
  local steps = 10

  local angvel_min, angvel_max, angvel_steps = -300, 300, 2
  local angvel_delta = (angvel_max - angvel_min)/angvel_steps
  local dt = Engine.secondsPerFrame()
  local t = dt*steps
  
  local npc = otherNPC()  
  npc:setSpeed(speed)
  npc:setRotation(dir)

  local s0 = npc:motionState():copy()
  s0:setPosition(pos)
  
  
  local trajectories = Geometry.createTrajectories(s0, angvel_min, angvel_max, angvel_steps, dt, steps)  
  
  -- local trajectories = Geometry.createTrajectories(s0, -400, 400, 10, dt, steps)  

  path_sprite = Sprite:new(PathView:new())
  path_sprite:view():setPolygon({vec(0,0), vec(1,0), vec(1,1)})
  path_sprite:view():setColor(1,1,1)
  
  npc.currentStep = steps 
  npc.statePath = nil
  npc.nextState = npc:motionState():copy()
  -- npc.nextState:integrate(dt, steps)
  
  local sprite_state = Sprite:new()
  sprite_state:setVisible(false)
  sprite_state:setName("sprite state")
  sprite_state:setView(npc:view())
  
  npc:setUpdateHandler(function(self, start_time, dt)  
    self.currentStep = self.currentStep+1
    if self.currentStep <= steps then return end
    self.currentStep = 1

    -- Only change movement if there is a next state
    if self.statePath and not self.statePath:empty() then
      self.nextState = self.statePath:pop()
      local s = self.nextState
      local ang_accel = (s:angularVelocity()-self:angularVelocity())/t
      self:setAngularAcceleration(ang_accel) 
    else
      npc:setAngularAcceleration(0)
      self.nextState = npc:motionState():copy()
      self.nextState:integrate(dt, steps)
    end
  end)
  
  
  npc:setPlanningHandler(function(self, start_time, dt)
    -- Return low value for collision paths
    function avoidCollision(s0, s1)
      sprite_state:setMotionState(s1)
      if sprite_state:collide(obstacles, start_time, dt) then
        return -1
      end
      return 0
    end

    local flank = Geometry.makeFlank(currentNPC(), 12)
    local seek = Geometry.makeSeek(currentNPC():position())
    -- local combo = Geometry.combineBehavior({1,flank}, {0.001,seek})
    -- local combo = Geometry.combineBehavior({1,seek})
    -- local combo = Geometry.combineBehavior({0.1,seek}, {1,avoidCollision})    
    local combo = Geometry.combineBehavior({0.001,seek}, {1,flank}, {1,avoidCollision})
    
    local eval = Geometry.makeEval(combo, 0.5, 0)
    -- Create a comparator that will return true if eval(a) > eval(b)
    local larger = makeBinary(eval, greaterThan)

    local states = Geometry.rrtStates(self.nextState, trajectories, larger, 15, start_time, dt)
    if states then 
      self.statePath = states
      local path = Geometry.statesToPath(states, dt, steps) 
      path_sprite:view():setPolygon(path)            
    end
  end)

end

function displayTrajectories(angvel_min, angvel_max, angvel_steps, dt, steps)  
  local pos = vec(0, 0)
  local speed = 10
  local dir = 0
  local t = dt*steps

  local s0 = MotionState:new(pos, dir, speed)
  
  local trajectories = Geometry.createTrajectories(s0, angvel_min, angvel_max, angvel_steps, dt, steps)  
  
  local s1 = trajectories[0][-240]
  local s2 = trajectories[-240][0]
  local s3 = trajectories[0][0]
  local s4 = trajectories[0][320]

  local s5 = trajectories[320][0]
    
  local seq = Collection:new(s0, s1, s2, s3, s4, s5, s3)
  
  local npc = currentNPC()
  npc:setMotionState(s0)
  npc.states = seq
  npc.currentStateIndex = 1
  npc.currentStep = steps 
  function npc:onUpdate()  
    self.currentStep = self.currentStep+1
    if self.currentStep <= steps then return end
    self.currentStep = 1
    local s = self:nextState()
    if s then
      self:setAngularAcceleration(s:angularAcceleration())
    else
      self:stop()
    end
  end
  
  function npc:nextState()
    self.currentStateIndex = self.currentStateIndex+1
    if self.currentStateIndex > self.states:size() then return nil end
    return self.states[self.currentStateIndex]
  end
    
  seq = Collection:new(s1, s2, s3, s4, s5, s3)
    
  local path = Polygon:new()
  local points = Polygon:new()
  
  local s_prev = s0:copy()
  for _,s in pairs(seq) do
    s_prev:setAngularAcceleration(s:angularAcceleration())    
    for step=1,steps do
      s_prev:advance(dt)
      path:append(s_prev:position())    
    end    
  end
  
  local pathSegs = Geometry.toOpenSegments(path) 
  local view = SegmentView:new(pathSegs)
  pathSprite = Sprite:new()
  pathSprite:setView(view)

  local seq = Collection:new(s0, s1, s2, s3, s4, s5, s3)
    
  points = Geometry.localStatesToPath(seq)
  
  pointsSprite = Sprite:new()
  local pointsView = SegmentView:new(Geometry.toOpenSegments(points))  
  -- local pointsView = PointsView:new(points)    
  pointsView:setColor(0,1,1)
  pointsSprite:setView(pointsView)
  
end

function setupKeypoints()
  local poly = Polygon:rect(-0.5, -0.5, 0.5, 0.5)
  local choke_poly = Polygon:new(vec(-0.5, -0.5), vec(0.5, -0.5), vec(0, 0.5))
  
  key_point_view = PolygonView:new(poly)
  key_point_view:setColor(0.6, 0.3 , 0.3)
  keypoints = Collection:new() -- Where we store all the created keypoints
  
  choke_point_view = SegmentView:new({seg(vec(0, 0), vec(1, 0))})
  choke_point_view:setArrowVisible(true)
  choke_point_view:setColor(0.3, 0.3, 0.8)
  
  -- Register event for finding choke points
  Engine.registerKeyClickEvent(Key.h, function()
    local npc = currentNPC()
    local keynodes = keypoints:map(function(sprite) 
      return roadmap:findNode(sprite:position()) 
    end)
    print("Found keynodes:", keynodes:toString())
    local n_start = roadmap:findNode(npc:position())
    if not n_start then 
      print("No node found so we could no continue looking for choke points")
      return
    end
    print("Start searching for choke points from", n_start:toString())
    local choke_points = Graph.findChokePoints(n_start, keynodes)
    
    choke_point_sprites = Collection:new()
    for _, choke_point in pairs(choke_points) do
      print("processing choke point:", choke_point.node:toString(), choke_point.direction:toString())
      local choke_sprite = Sprite:new(choke_point_view)
      choke_sprite:setPosition(choke_point.node:position())
      choke_sprite:setDirection(choke_point.direction)
      choke_point_sprites:append(choke_sprite)
    end
  end)  
end

function setupWorld()
  --obstacles = createRandomObstacles(10):toGroup()
  obstacle_collection = dofile("script/levels/level2.lua") -- loads sprites from level file
  obstacles = ShapeGroup:new(obstacle_collection:toGroup())
  
  actors = Group:new()

  local view = PolygonView:new()
  npcs = {}

  npcs[1] = actors:addNPC(view)
  npcs[2] = actors:addNPC(view)  


  selectedNPC = 2
  -- trailingLine = Sprite:new()
  -- trailingLine:setView(SegmentView:new())
  
  local npc = currentNPC()
  Engine.registerKeyClickEvent(Key.z, makePlayerSwitch)  
  Engine.update = update  

  Engine.registerKeyClickEvent(Key.v, function()
    Engine.setView(Rect:new(-20,-20, 20, 20))
  end)  

  -- minkowski_obstacles = obstacle_collection:map(
  --      Geometry.minkowskiAdder(Polygon:rect(-1, -1, 2, 2), 1, 0, 0))

end

function setupCollisionHandling()
  local npc = currentNPC()
  npc:setCollisionHandler(function(self, other, points, t, dt)
    npc:setPosition(npc:prevPosition())
    npc:stop()
  end)  
end

function setupRoadMap()
  roadmap = ProbablisticRoadMap:new(obstacles, obstacles:boundingBox())
  -- roadmap = ProbablisticRoadMap:new(obstacles, Engine.view())

  Engine.registerKeyClickEvent(Key.k, function()
    roadmap:construct(25*25, 1)
    roadmap:displayRoadMap()    
    print("roadmap constructed")    
  end)

  -- Visualize contained circle somehow when c is hit
  Engine.registerKeyClickEvent(Key.c, function()    
    local me = currentNPC()
    local other = otherNPC()    
    local n_src = roadmap:findNode(me:position())
    local n_dst = roadmap:findNode(other:position())    
    if n_src and n_dst then 
      local distance, path = Graph.astar(n_src, n_dst)

      local poly = Graph.createPath(n_src, n_dst, path):map(function(n) 
        return n.circle:center()
      end)
      poly:prepend(me:position())
      poly:append(other:position())

      Geometry.viewPath(poly)
    end
  end)
  
  -- Load roadmap
  Engine.registerKeyClickEvent(Key.l, function()
    roadmap = ProbablisticRoadMap:new(obstacles, Engine.view())
    roadmap:load('script/RoadMaps/roadmap1.lua')
    print("roadmap loaded")
    roadmap:displayRoadMap()    
        
  end)
  
  -- Store current roadmap to file 
  Engine.registerKeyClickEvent(Key.s, function()    
    roadmap:save('script/RoadMaps/roadmap3.lua')
    print("roadmap saved")
  end)
  
  -- For debugging bad point
  Engine.registerKeyClickEvent(Key.g, function()    
    print("Segment to badpoint")
    local me = currentNPC()
    local other = otherNPC()
    
    other:setPosition(vec(-18, 44))
    -- bad_points = {seg(me:position(), vec(-18, 44))}
    -- bad_points = Geometry.viewSegments(bad_points)
  end)
  
  -- Color obstacle clicked
  Engine.registerMouseClickEvent(function()
    local pos = Engine.mousePosition
    local hit_poly = false
    obstacles:inside(pos, Engine.seconds(), 1, function(shape, t, dt)
      hit_poly = true
      shape:view():setColor(math.random(), math.random(), math.random())
      print("Position of hit", pos:toString())
      local poly = shape:polygon()
      print("Polygon hit")
      for _,p in pairs(poly) do
        p:print()
      end
    end)
    
    -- place a key point, to use for choke point finder
    if not hit_poly then
      local keypoint = Sprite:new(key_point_view)
      keypoint:setPosition(pos)
      keypoints:append(keypoint)
    end
  end)
end

function testStratifiedSampling()  
  local samples = Geometry.stratifiedSamples(120*120, obstacles:boundingBox())
  --local samples = Geometry.randomSamples(100, obstacles:boundingBox())  
  Geometry.viewPoints(samples)
end

setupKeypoints()
setupWorld()
setupCollisionHandling()
--testStratifiedSampling()
setupRoadMap()
--setupSeek()
--setupRRTSearch()