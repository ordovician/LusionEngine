require("script/geometry")

SpecialTest = {}

function SpecialTest.oddCollisionProblem()
  local small_poly = Collection:new()
  small_poly:append(vec(8.59, -3.69))
  small_poly:append(vec(10.59, -2.69))
  small_poly:append(vec(8.59, -1.69))
      
  local small_sprite = Sprite:new()
  local small_view = PolygonView:new(small_poly)
  small_view:setColor(0, 0 , 1)
  small_sprite:setView(small_view)
  small_sprite:setPosition(0, 0)
  

  local large_poly = Collection:new()
  large_poly:append(vec(16.7143, 9.85714))
  large_poly:append(vec(20.2857, 9.71429))
  large_poly:append(vec(20.1429, -19))
  large_poly:append(vec(10, -19.1429))
  large_poly:append(vec(10.4286, -6.71429))
        
  local large_sprite = Sprite:new()
  local large_view = PolygonView:new(large_poly)
  large_view:setColor(1, 1 , 0.3)
  large_sprite:setView(large_view)
  large_sprite:setPosition(0, 0)
        
  local t  = Engine.seconds()
  local dt = 5
  
  small_poly:append(small_poly[1])
  large_poly:append(large_poly[1])  
  print("Small directions")
  local i=2
  while i <= #small_poly do
    (small_poly[i] - small_poly[i-1]):unit():print()
    i = i+1
  end
  
  print("Large directions")
  local i=2
  while i <= #large_poly do
    (large_poly[i] - large_poly[i-1]):unit():print()
    i = i+1
  end
  
  -- (vec(10.59, -2.69) - vec(8.59, -3.69)):unit():print()
  -- (vec(8.59, -1.69) - vec(10.59, -2.69)):unit():print()
  -- (vec(8.59, -3.69) - vec(8.59, -1.69)):unit():print()
  
  -- (vec(20.2857, 9.71429) - vec(16.7143, 9.85714) ):unit():print()      
  -- (vec(20.1429, -19) - vec(20.2857, 9.71429)):unit():print()
  -- (vec(10, -19.1429) - vec(20.1429, -19) ):unit():print()
  -- (vec(10.4286, -6.71429) - vec(10, -19.1429)):unit():print()
  -- (vec(16.7143, 9.85714) - vec(10.4286, -6.71429)):unit():print()        
  -- assert(not npc1:collide(obstacles, t, dt), "tricky collisiontest 1 failed")
  -- assert(not npc2:collide(obstacles, t, dt), "tricky collisiontest 2 failed")  
    
end

return SpecialTest