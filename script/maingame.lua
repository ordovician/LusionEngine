require("script/roadmap")
require("script/trapezoid")

-- Define global data
function setupWorld()
  noEnemies = 0
  noObstacles = 6 
  -- local planePoly = {vec(-0.6,-0.8), vec(1, 0), vec(-0.6,0.8)}
  -- planeView = ImageView:new("Ship2Grey.png", planePoly)
  -- enemyView = ImageView:new("DeathFighter1.png", planePoly)  
  planeView = PolygonView:new()
  enemyView = PolygonView:new()  
  
  planeView:setColor(0,1,0)  
  enemyView:setColor(1,0,0)  
  
  local br = 0.2  -- Bullet radius
  -- bulletView = ImageView:new("missile.png", 2,0.5)
  bulletView = PolygonView:new({vec(-br,-br), vec(br, 0), vec(-br,br)})

  bulletView:setColor(1,1,1)
  
  -- planeView = View:new("player1.png")
  actors = Group:new()
  enemies = Group:new()
  obstacles = Group:new()  
  playerBullets = Group:new()    
  enemyBullets = Group:new()    

  Engine.update = update
end

-- Define player
function setupPlayer()
  player = Sprite:new(4, 0, 0, 0.0)
  player:setView(planeView)
  player:setName("Player")
  player.fireTimer = Timer:start()    
  actors:add(player)
  function player:onCollision(other)
    handleCollision(self)
    if enemyBullets:contains(other) then
      print("player hit!")
      self:setPosition(0, 0)
      self:setSpeed(0)
      other:kill()
    end
  end  
  Engine.setPlayer(player)  
  
  PlayerBullet = Sprite:new()

  function PlayerBullet:onCollision(other)
    self:kill()
    if enemies:contains(other) then
      other:kill()
    end
  end  
end

function handleCollision(self)
  self:setSpeed(0)
  self:setPosition(self:prevPosition())  
end

-- Define enemies
function setupEnemies()
  local Enemy = Sprite:new(0, 0, 45, 0.06)  -- Define enemy base class
  local speed = 0.07  
  
  EnemyBullet = Sprite:new()
  function EnemyBullet:onCollision(other)
    self:kill()
    if other == player then
      player:kill()
    end
  end  
  
  function Enemy:onCollision(other)
    self:setPosition(self:prevPosition())
    self:reverse()
    -- self:kill()
  end

  -- Plan enemy movements
  function Enemy:onPlan(start_ticks)
    if Engine.ticksLeft(start_ticks) > 1 then      
      local dst = player:position()
      local src = self:position()
      self:setDirection(dst-src)
    else
      print("No time for planning "..self:name())    
    end
  end

  for i=1,noEnemies do
    local angle = math.random(0,360)
    local npc = Enemy:new(0, 0, angle, speed)
    npc:setView(enemyView)
    npc:setName("NPC"..i)
    npc:setSpeed(speed)
    npc:setRotation(angle)
    npc.fireTimer = Timer:start()
    
    while actors:spriteCollide(npc) ~= nil do
      local x = math.random(xmin, xmax)
      local y = math.random(ymin, ymax)    
      npc:setPosition(x,y)
    end 
    enemies:add(npc)
    actors:add(npc)  
  end
end

function setupObstacles()
  trapezoidSprites = {} --To display the created trapezoids
  
  print("BEGIN obstacle list:")
  print("local params = {")
  for i=1,noObstacles do
    local angle = math.random(0,360)
    local speed = 0  
    local obstacle = Sprite:new()

    -- Create a random shape for view
    local m = 0.5
    local w = math.random(2,10)*m;
    local h = math.random(2,10)*m;
    -- local minx = -w/2
    -- local maxx = w/2
    -- local miny = -h/2
    -- local maxy = h/2    
    -- local obstacleView = ImageView:new("Block1.png", w, h)        
    -- local obstacleView = View:new("Block1.png")        
    -- local obstacleView = PolygonView:new()        

    local obstacleView = PolygonView:new(Polygon:rect(0,0, w, h))        
    
    
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
    until not (actors:spriteCollide(obstacle) or obstacles:spriteCollide(obstacle) )

    if i ~= noObstacles then
      print("{{"..w..", "..h.."}, {"..x..", "..y..", "..angle..",0}},")
    else
      print("{{"..w..", "..h.."}, {"..x..", "..y..", "..angle..",0}}")
    end
    obstacles:add(obstacle)
    obstacle:setVisible(false)
  end
  print("}")
  print("END obstacle list:")  
  -- local map = RoadMap:new(obstacles:sprites(), rec(-15, -15, 15, 15))
  local map = RoadMap:new(obstacles:sprites(), Engine.view())    
  -- local map = TrapezoidalMap:new(spritesToSegments(obstacles:sprites()), Engine.view())      
  -- map:printGraph()

  -- Display the trapezoids making up the obstacles
  local traps = map:trapezoids()
  for _,trap in pairs(traps) do
    table.insert(trapezoidSprites, Geometry.viewTrapezoid(trap))
  end
  
  trapezoidEdges = Sprite:new()
  
  pathSegs = nil
  local mouse_down = false
  
  function Engine.handleMouseClick()
    if Engine.mouseButton then 
      mouse_down = true
      return 
    end
    if not Engine.mouseButton and not mouse_down then return end
    mouse_down = false
  
    -- Show clicked trapezoid
    local hit_trap = map:locate(Engine.mousePosition)
    trapezoidEdges:setView(trapezoidsView({hit_trap}))
    
    -- Show tactical position to attack player from and path to it
    local pos = Engine.mousePosition
    local tactical_pos = map:findTacticalPosition(pos, player)
    local path = map:shortestPath(pos, tactical_pos)
    if pathSegs then pathSegs:setVisible(false) end
    pathSegs = Geometry.viewPath(path)                
  end
    
  local source_point = nil
  local target_point = nil
  
  
  function Engine.handleMouseClick4()
    if Engine.mouseButton then 
      mouse_down = true
      return 
    end
    if not Engine.mouseButton and not mouse_down then return end
    mouse_down = false
    
    -- Show clicked trapezoid
    local hit_trap = map:locate(Engine.mousePosition)
    trapezoidEdges:setView(trapezoidsView({hit_trap}))
    
    -- Show shortest path between two clicked trapezoids
    if not source_point then
      source_point = Engine.mousePosition
    else
      target_point = Engine.mousePosition
      local path = map:shortestPath(source_point, target_point)
      if pathSegs then pathSegs:setVisible(false) end
      pathSegs = Geometry.viewPath(path)            
      source_point = nil
    end
  end

  local previous_trap = nil
  local paths = nil
  local source_trap = nil
  
  function Engine.handleMouseClick1()
    -- First make sure we actually hit a new and relevant trapezoid
    if not Engine.mouseButton then return end
    local hit_trap = map:locate(Engine.mousePosition)
    if not hit_trap or hit_trap == previous_trap then return end 
    previous_trap = hit_trap
    print("TRAPEZOID:", hit_trap:tag())
    
    -- Display clicked trapezoid
    local traps = {hit_trap}
  
    trapezoidEdges:setView(trapezoidsView(traps))
    
    -- Then record pairs
    if not source_trap then
      source_trap = hit_trap
    else
      path = map:shortestPath(source_trap, hit_trap)
      source_trap = nil
    -- if not paths then
    --   paths = map:shortestPaths(hit_trap)
    -- else
    --   local path = paths:pathFrom(hit_trap)
    --   if not path then 
    --     print("Found no path among shortest paths!")
    --     return 
    --   end
      -- paths:printPathFrom(hit_trap)            
      if pathSegs then pathSegs:setVisible(false) end
      pathSegs = Geometry.viewPath(path)      
      -- paths = nil
    end
  end

  function Engine.handleMouseClick2()
    if not Engine.mouseButton then return end
    local hit_trap = map:locate(Engine.mousePosition)
    if not hit_trap or hit_trap == previous_trap then return end
    if graph then graph:setVisible(false) end    
    previous_trap = hit_trap

    trapezoidEdges:setView(trapezoidsView({hit_trap}))
    
    print("TRAPEZOID:", hit_trap:tag())
    graph = hit_trap:drawGraph()
    hit_trap:printNeighbors()
    -- hit_trap:printGraph()
  end
end

function handleSpriteCollision(sprite, others)
  if not others then return end    
  if not sprite.onCollision then return end
  for i,other in ipairs(others) do
    sprite:onCollision(other)
  end  
end

function handleGroupCollision(sprites)
  if not sprites then return end    
  for sprite,others in pairs(sprites) do
    handleSpriteCollision(sprite, others)
  end
end

function updatePlayer()
  if Engine.keyState(Key.SDLK_z) then
    player:setAngularVelocity(0.4)
  elseif Engine.keyState(Key.SDLK_x) then
    player:setAngularVelocity(0)
  end
end

timer = AvgTimer:new(100)

function update(start_ticks)
  updatePlayer()
  Engine.handleMouseClick()
  -- timer:start()
  -- function timeLeft()
  --   return Engine.ticksLeft(start_ticks)
  -- end

  -- Handle collision between enemies
  handleGroupCollision(enemies:groupCollide(enemies, start_ticks))
  handleGroupCollision(playerBullets:groupCollide(enemies, start_ticks))  
  handleGroupCollision(playerBullets:groupCollide(obstacles, start_ticks))  
  handleGroupCollision(enemyBullets:groupCollide(obstacles, start_ticks))  
  handleSpriteCollision(player, enemyBullets:spriteCollide(player, start_ticks))
      
  -- Handle collision between enemies and player
  sprites = enemies:spriteCollide(player, start_ticks)

  if sprites then 
    for i,enemy in ipairs(sprites) do
      if enemy.onCollision then 
        enemy:onCollision(player)
      end
    end 
  end
  handleSpriteCollision(player, sprites)
  handleGroupCollision(enemies:groupCollide(obstacles, start_ticks))
  handleSpriteCollision(player, obstacles:spriteCollide(player, start_ticks))  
  for k, sprite in pairs(actors:sprites()) do
    if sprite:viewCollide() then
      local pos = sprite:position()
      local leftSide = true
      local bottomSide = true
      local dx = pos.x-xmin
      local dy = pos.y-ymin
      if xmax-pos.x < dx  then leftSide = false;  dx = xmax-pos.x end
      if ymax-pos.y < dy  then bottomSide = false;  dy = ymax-pos.y end
      
      if dx < dy then
        dx = 3
        if leftSide then 
          sprite:setPosition(xmax-dx, pos.y) 
        else
          sprite:setPosition(xmin+dx, pos.y)         
        end
      else
        dy = 3
        if bottomSide then 
          sprite:setPosition(pos.x, ymax-dy) 
        else
          sprite:setPosition(pos.x, ymin+dy)         
        end      
      end
      -- if (enemies:contains(sprite)) then
      --   sprite:reverse()
      -- else      
      --   handleCollision(sprite)
      -- end
    end
  end
  actors:update()
  local npc = enemies:nextSprite()
  if npc and npc.onPlan then npc:onPlan(start_ticks) end

  if Engine.fire then
    if player.fireTimer:elapsed() > 1000 then
      local pos = player:position()
      local dir = player:rotation()
      local bullet = PlayerBullet:new(pos, dir, 1)
      bullet:setView(bulletView)
      playerBullets:add(bullet)
      actors:add(bullet)
      bullet:update()
      bullet:setSpeed(0.1+math.max(player:speed(), 0))
      player.fireTimer = Timer:start()
    end
  end
  
  -- Plan enemy fireing
  -- for i,enemy in ipairs(enemies:sprites()) do
  --   if enemy.fireTimer:elapsed() > 1000 then
  --     local x, y = enemy:position()
  --     local dir = enemy:rotation()
  --     local bullet = EnemyBullet:new(x,y, dir, 1)
  --     bullet:setView(bulletView)
  --     enemyBullets:add(bullet)
  --     actors:add(bullet)
  --     bullet:update()
  --     bullet:setSpeed(0.1+math.max(enemy:speed(), 0))
  --     enemy.fireTimer = Timer:start()
  --   end
  -- end
  -- timer:stop()
end

-- Run tests
-- dofile("script/gametest.lua")

-- Start Game
setupWorld()
setupPlayer()
setupEnemies()
setupObstacles()

-- for i,sprite in ipairs(actors:sprites()) do
--   print("sprite",i,sprite:name())
-- end

-- print("10 next sprites")
-- for i=1,10 do
--   print(enemies:nextSprite():name())
-- end