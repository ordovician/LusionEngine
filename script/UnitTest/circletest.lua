CircleTest = {}

function CircleTest.circleSegmentIntersection()
  local c = Circle:new(vec(10, 0), 2)
  local p1 = vec(-1, -1)
  local p2 = vec(1, 0)
  local p3 = vec(-1, 1)
  
  local seg1 = seg(p1, p2)
  local seg2 = seg(p2, p3)
  local seg3 = seg(p3, p1)
        
  assert(not c:segmentIntersect(seg1))
  assert(not c:segmentIntersect(seg2))
  assert(not c:segmentIntersect(seg3))  
end

return CircleTest