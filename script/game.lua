-- dofile("script/unittester.lua")
-- dofile("script/gametest.lua")
-- Sprite.setShowCollision(true)

-- dofile("script/maingame.lua")

-- medial axis or skeleton
require("script/rrt")
require("script/queue")
require("script/stack")

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

-- Creates a random number of obstacles and return the sprite group containing them
function createRandomObstacles(noObstacles)  
  local obstacles = Group:new()

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

    -- obstacle:setVisible(false)
  end
  return obstacles
end

function Shape:addNPC(planeView)
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
  -- Engine.lookAt(currentNPC():position())
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

function setupWorld()
  obstacles = createRandomObstacles(10)
  obstacles = ShapeGroup:new(obstacles)
  -- boxes = Collection:new(unpack(col_objs:boundingBoxes()))
  -- boxes = boxes:map(function(x) return Sprite:new(OutlineView:new(x:toPolygon())) end)

  actors = Group:new()

  local view = PolygonView:new()
  npcs = {}
  npcs[1] = actors:addNPC(view)
  npcs[2] = actors:addNPC(view)  

  selectedNPC = 2
  -- trailingLine = Sprite:new()
  -- trailingLine:setView(SegmentView:new())
  
  local npc = currentNPC()
  -- npc:setPosition(vec(0, 0))
  Engine.registerKeyClickEvent(Key.z, makePlayerSwitch)  
  Engine.update = update  

  Engine.registerKeyClickEvent(Key.v, function()
    Engine.setView(Rect:new(-20,-20, 20, 20))
  end)  

end

function setupCollisionHandling()
  local npc = currentNPC()
  npc:setCollisionHandler(function(self, other, points, t, dt)
    npc:setPosition(npc:prevPosition())
    npc:stop()

  end)
end

setupWorld()
setupCollisionHandling()
setupSeek()
-- setupRRTSearch()