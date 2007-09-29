--[[
	RoadMap class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  A roadmap. Used to help navigate sprites
	
--]]

require("script/stack")
require("script/queue")
require("script/collection")

-- Helper functions
function spritesToSegments(sprites)
  segs = {}
  for i,sprite in ipairs(sprites) do
    for i,seg in ipairs(sprite:segments()) do
      table.insert(segs, seg)
    end
  end
  return segs
end

-- Constructor
RoadMap = {}

function RoadMap:new(sprites, boundingbox)
  -- Create instance
  local roadmap = {}
  setmetatable(roadmap, self)
  self.__index = self
  
  roadmap:init(sprites, boundingbox)
  
  return roadmap
end

function RoadMap:init(sprites, boundingbox)
  -- Build the roadmap
  local segs = spritesToSegments(sprites)
  self.map = TrapezoidalMap:new(segs, boundingbox)

  local trapezoids = self.map:trapezoids()

  -- Remove trapezoids inside obstacles
  local last_t = nil
  for _,t in pairs(trapezoids) do
    local b = t:bottom()
    if b.source.x < b.target.x then
      self.map:remove(t)
    else
      last_t = t
    end
  end

  local noTrapezoids = self.map:assignUniqueTags()
  
  local edges = last_t:edgeData(noTrapezoids)
  self.graph = Graph:new(noTrapezoids, edges)
end

function RoadMap:locate(point)
  return self.map:locate(point)
end

function RoadMap:trapezoids()
  return self.map:trapezoids()
end

function RoadMap:shortestPath(source, target)
  local s = self.map:locate(source)
  local t = self.map:locate(target)
  local path = self.graph:shortestPath(s, t)
  if path then 
    path[1] = source
    path[#path] = target
  end
  return path
  -- return self.graph:shortestPath(source, target)
end

function RoadMap:shortestPaths(target)
  return self.graph:shortestPaths(target)
end

function RoadMap:printGraph()
  self.graph:printGraph()
end

-- Runs in linear time
function RoadMap:insideCircle(enemy, radius)
  local traps = Collection:new(unpack(self:trapezoids()))
  local xt = enemy:position()
  
  function isInside(trap)
    return xt:distance(trap:center()) < radius
  end
  
  return traps:filter(isInside):map(Trapezoid.center)
end

-- function RoadMap:findTacticalPosition(npc, enemy)
--   local diff = npc:position() - enemy:position()
function RoadMap:findTacticalPosition(pos, enemy)
  local diff = pos - enemy:position()

  local distance = diff:length()
  local candiate_points = self:insideCircle(enemy, distance/2)
  candiate_points =  Collection:new(unpack(Geometry.convexHull(candiate_points)))

  -- Calculate goodness of NPC being placed at position Xn
  local xe = enemy:position()
  local ve = enemy:direction()
  function goodness(xn)
    local v = xe-xn
    return (1 + (ve*v)/(xe:length()*v:length()))*0.5
  end
  return candiate_points:max(goodness)
end

function RoadMap:stageAmbush(npc, enemy, chokepoint)
  pm = shortestPath(enemy:position(), chokepoint[1])
  tm = pathLength(pm)/enemy:speed() -- estimated arrival time at choke point
  dm = npc:speed()*tm               -- Distance NPC needs to travel to get to chokepoint
  a = findTacticalPosition(npc:position(), chokepoint[1], chokepoint[2])
  return fixedLengthPath(npc:position(), a, dm)
end

-- Ideas for how onPlan would work
-- enemy = selectEnemy(enemies)
-- tactical_pos = findTacticalPosition(self:position(), self:direction, enemy:position(), enemy:direction())
-- path = findPath(self:position(), tactical_pos, start_time)
-- self:setDirection(path[1]-self:position())
-- For visualizing
-- pathSprite = Sprite:new(PathView:new(path))

-- dijkstra distance between trapezoid t1 and t2
-- if there is no connection between t1 and t2 then distance will be math.huge
function length(t1, t2)
  for k,v in pairs(t1) do
    if v == t2 then
      return t1:distance(t2)
    end
  end
  return math.huge
end

-- Finds key with smalles value
function minElement(d)
  local mv = math.huge
  local mk = nil
  
  for k,v in pairs(d) do
    if v <= mv then mk = k end
  end
  return mk
end
