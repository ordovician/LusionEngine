local obstacles = Group:new()

function addObstacle(...)
  local poly = Collection:new()
  for _,v in ipairs(arg) do
    local w = v*(1/280)*40
    poly:append(w - vec(20,20))
  end

  local obstacle = Sprite:new()
  local view = PolygonView:new(poly)
  view:setColor(1, 1 , 0.3)
  obstacle:setView(view)
  obstacle:setPosition(0, 0)
  obstacles:add(obstacle)
end

--addObstacle(vec(8.00, 249.00), vec(8.00, 166.00), vec(37.00, 179.00), vec(50.00, 244.00), vec(32.00, 246.00))
--addObstacle(vec(64.00, 246.00), vec(280.00, 247.00), vec(279.00, 221.00), vec(69.00, 214.00))
addObstacle(vec(257.00, 209.00), vec(282.00, 208.00), vec(281.00, 7.00), vec(210.00, 6.00), vec(213.00, 93.00))
addObstacle(vec(104.00, 189.00), vec(200.00, 191.00), vec(185.00, 130.00), vec(97.00, 130.00), vec(96.00, 163.00))
--addObstacle(vec(14.00, 150.00), vec(50.00, 161.00), vec(61.00, 26.00), vec(22.00, 10.00), vec(9.00, 15.00))
--addObstacle(vec(77.00, 42.00), vec(205.00, 32.00), vec(199.00, 10.00), vec(91.00, 10.00))
--addObstacle(vec(116.00, 108.00), vec(117.00, 59.00), vec(162.00, 52.00), vec(168.00, 90.00), vec(152.00, 114.00))
return obstacles