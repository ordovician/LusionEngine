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

addObstacle(vec(14.00, 413.00), vec(14.00, 527.00), vec(38.00, 526.00), vec(38.00, 414.00))
addObstacle(vec(54.00, 527.00), vec(627.00, 527.00), vec(627.00, 498.00), vec(57.00, 501.00))
addObstacle(vec(16.00, 392.00), vec(11.00, 11.00), vec(22.00, 11.00), vec(26.00, 391.00))
addObstacle(vec(29.00, 13.00), vec(626.00, 12.00), vec(626.00, 20.00), vec(29.00, 22.00))
addObstacle(vec(624.00, 36.00), vec(625.00, 401.00), vec(620.00, 401.00), vec(615.00, 38.00))
addObstacle(vec(90.00, 469.00), vec(72.00, 438.00), vec(72.00, 411.00), vec(127.00, 413.00), vec(131.00, 468.00))
addObstacle(vec(175.00, 466.00), vec(174.00, 328.00), vec(210.00, 329.00), vec(286.00, 443.00), vec(223.00, 463.00))
addObstacle(vec(264.00, 329.00), vec(334.00, 425.00), vec(404.00, 414.00), vec(405.00, 335.00), vec(361.00, 322.00))
addObstacle(vec(350.00, 475.00), vec(350.00, 462.00), vec(414.00, 448.00), vec(414.00, 466.00), vec(393.00, 471.00))
addObstacle(vec(122.00, 308.00), vec(213.00, 265.00), vec(213.00, 265.00), vec(213.00, 211.00), vec(117.00, 213.00))
addObstacle(vec(64.00, 343.00), vec(57.00, 237.00), vec(65.00, 238.00), vec(76.00, 342.00))
addObstacle(vec(251.00, 285.00), vec(248.00, 261.00), vec(354.00, 257.00), vec(355.00, 280.00))
addObstacle(vec(466.00, 318.00), vec(401.00, 283.00), vec(398.00, 252.00), vec(473.00, 194.00), vec(517.00, 252.00), vec(505.00, 319.00))
addObstacle(vec(268.00, 210.00), vec(366.00, 208.00), vec(436.00, 166.00), vec(457.00, 141.00), vec(459.00, 98.00), vec(353.00, 98.00), vec(276.00, 159.00))
addObstacle(vec(133.00, 169.00), vec(198.00, 167.00), vec(333.00, 61.00), vec(51.00, 63.00), vec(50.00, 110.00))
addObstacle(vec(545.00, 192.00), vec(513.00, 160.00), vec(513.00, 117.00), vec(540.00, 75.00), vec(585.00, 73.00), vec(587.00, 125.00))
addObstacle(vec(544.00, 333.00), vec(558.00, 280.00), vec(583.00, 309.00), vec(584.00, 431.00), vec(547.00, 438.00), vec(537.00, 401.00))
addObstacle(vec(480.00, 464.00), vec(476.00, 452.00), vec(494.00, 444.00))
return obstacles