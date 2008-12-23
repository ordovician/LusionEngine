--[[
  Test module for probablistic roadmap
  Testing if the individual parts of the roadmap
  algorithms work
]]--

require("script/prm")

PrmTests = {}

function PrmTests.makeWorld()
  -- Create two obstacles
  local w = 10
  local h = 15
  local obstacleView = PolygonView:new(Polygon:rect(w, h))        

  obstacleView:setColor(1,1,0.3)
  
  obstacles = Collection:new()
  local obstacle = Sprite:new()
  obstacle:setView(obstacleView)
  obstacle:setName("Obstacle1")
  obstacle:setSpeed(0)
  obstacle:setRotation(30)
  obstacle:setPosition(vec(-6, -4))
  obstacles:append(obstacle)
  
  obstacle = Sprite:new()
  obstacle:setView(obstacleView)
  obstacle:setName("Obstacle2")
  obstacle:setSpeed(0)
  obstacle:setRotation(0)
  obstacle:setPosition(vec(10, 3))
  obstacles:append(obstacle)
  
  -- Create a sample point
  local point_poly = Polygon:rect(-0.3, -0.3, 0.3, 0.3)
  
  local point_view = PolygonView:new(point_poly)
  local retract_view = PolygonView:new(point_poly)  
  point_view:setColor(0.6, 0.3 , 0.3) 
  retract_view:setColor(1, 0 , 0) 
    
  local sample_point = vec(0, 0)
  point_sprite = Sprite:new()
  point_sprite:setView(point_view)
  point_sprite:setPosition(sample_point)
  
  -- Setup the roadmap
  -- convert regular collection of obstacles to ShapeGroup
  obstacles = ShapeGroup:new(obstacles:toGroup())
  roadmap = ProbablisticRoadMap:new(obstacles, Rect:new(-10, -10, 10, 10))

  -- local retracted_point = roadmap:retractSample(sample_point)
  local c_v = roadmap:slowRetractSample(sample_point)  
  local radius = roadmap:largestFreeDisc(c_v)
  node = PrmNode:newNode(c_v, radius)
  
  retracted_sprite = Sprite:new()
  retracted_sprite:setView(retract_view)
  retracted_sprite:setPosition(c_v)

  -- debug
  -- print("DEBUG:", roadmap:slowNearestObstacle(c_v):toString())
  -- print("DEBUG:", roadmap:slowNearestObstacle(c1):toString())
  
  -- Show line to nearest point on obstacle
  -- local c_near = roadmap:nearestObstacle(sample_point)
  -- local near_seg = seg(c_v, c_near)
  -- print("near seg distance: ", near_seg:toVector():length())
  -- local near_view = SegmentView:new({near_seg})  
  -- near_sprite = Sprite:new()
  -- near_sprite:setView(near_view)
  -- 
  -- local close_seg = seg(c_v, c_close)
  -- print("close seg distance: ", close_seg:toVector():length())  
  -- local close_view = SegmentView:new({close_seg})  
  -- close_sprite = Sprite:new()
  -- close_sprite:setView(close_view)
  
end

return PrmTests