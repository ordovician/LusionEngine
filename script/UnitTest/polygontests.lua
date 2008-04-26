PolygonTests = {}

function PolygonTests.pointOnBorder()
  local poly = {vec(5, 46.571428571429),
                vec(4.8571428571429, 26.857142857143),
                vec(10, 27),
                vec(20.857142857143, 43.285714285714),
                vec(11.857142857143, 46.142857142857)}
  local is_inside = Geometry.inside(poly, vec(14, 33))
  assert(is_inside)
end

return PolygonTests