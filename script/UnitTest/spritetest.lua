SpriteTest = {}

function SpriteTest.testInitialization()
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = View:new()
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
  
  assert(s[1]:collide(s[2]), "Sprite collisiontest 1 failed")
  assert(s[3]:collide(s[2]), "Sprite collisiontest 2 failed")  
  assert(s[3]:collide(s[1]), "Sprite collisiontest 3 failed")    
end

function SpriteTest.testMovement()
  local x = 0
  local y = 0  
  local angle = 0
  local speed = 0
  local view = View:new()  
  local s = Sprite:new(x, y, angle, speed)
  s:setView(view)
  s:setName("sprite"..i)
  
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
    end
  end
end

return SpriteTest