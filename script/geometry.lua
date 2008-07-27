require("script/collection")

--[[
	Vector class
--]]

function Vector:toString()
  return "vec("..self.x..", "..self.y..")" 
end

function Vector:print()
  print(self:toString())
end

function Vector:toSegment()
  return Segment:new(Vector:new(0,0), self)
end

function Vector:distance(v)
  local diff = self-v
  return diff:length()
end

function vec(x,y)
  return Vector:new(x,y)
end

--[[
	Matrix class
--]]
function Matrix:toString()
  local lines = {}
  for row=1,3 do
    local line = string.format("%f %f %f", self:atRow(row))
    table.insert(lines, line)
  end
  return table.concat(lines, "\n")
end

function Matrix:print()
  print(self:toString())
end

--[[
	Segment class
--]]

function Segment:toString()
  return "seg("..self.source:toString()..", "..self.target:toString()..")"
end

function Segment:print()
  print(self:toString())
end

function seg(s,t)
  return Segment:new(s,t)
end

--[[
	Rect class
--]]
function Rect:print()
  print(self:toString())  
end

function Rect:toString()
  return "rec("..self.min:toString()..", "..self.max:toString()..")" 
end

function rec(...)
  return Rect:new(...)
end


function Rect:toPolygon()
  local h = self:height()
  local w = self:width()
  return Polygon:new(self.min, self.min+vec(w,0), self.max, self.min+vec(0,h))
end

--[[
	Ray class
--]]
function Ray:toSegment()
  return Segment:new(self.origin, self.origin+(self.direction*10.0))
end

function Ray:toString()
  return "ray("..self.origin:toString()..", "..self.direction:toString()..")"
end

function Ray:print()
  print(self:toString())
end

function ray(origin, direction)
  return Ray:new(origin, direction)
end

--[[
	MotionState class
--]]
function MotionState:transform()
  return Matrix:translate(self:position())*Matrix:rotate(self:rotation())
end

--[[
	Geometry functions. extends the capabilities defined in C++
--]]
function Geometry.toSegments(p)
  local n = #p
  local segs = {}
  for i=2,n do
    table.insert(segs, seg(p[i-1], p[i]))
  end
  table.insert(segs, seg(p[n], p[1]))
  return segs
end

function Geometry.toOpenSegments(p)
  local n = #p
  local segs = {}
  for i=2,n do
    table.insert(segs, seg(p[i-1], p[i]))
  end
  return segs
end

function Geometry.viewSegments(segments)
  local view = SegmentView:new(segments)
  view:setArrowVisible(false)
  view:setColor(0,0,1)  
  local sprite = Sprite:new()
  sprite:setView(view)
  return sprite
end

function Geometry.viewPolygonLines(poly)
  local view = SegmentView:new(Geometry.toSegments(poly))
  local sprite = Sprite:new()
  sprite:setView(view)
  return sprite
end

function Geometry.viewPath(poly)
  local view = SegmentView:new(Geometry.toOpenSegments(poly))
  local sprite = Sprite:new()
  sprite:setView(view)
  view:setArrowVisible(true)
  view:setColor(1,1,0)  
  return sprite
end

function Geometry.viewPolygon(poly)
  local view = PolygonView:new(poly)
  local sprite = Sprite:new()
  sprite:setView(view)
  return sprite
end

function Geometry.viewPoints(points)
  local view = PointsView:new(points)
  view:setColor(1,1,1)
  local sprite = Sprite:new()
  sprite:setView(view)
  return sprite
end

function Geometry.viewTrapezoid(trap)
  local view = TrapezoidView:new(trap)
  view:setColor(1,0,1)
  local sprite = Sprite:new()
  sprite:setView(view)
  return sprite
end

-- Used by minkowski sum to calculate angle between vector pq and positive x axis
function angle(p, q)
  -- if not p or not q then print(debug.traceback("angle")); debug.debug() end  
  local v = q-p
  local ang = math.atan2(v.y, v.x)
  if ang < 0 then ang = 2.0*math.pi+ang end
  return ang
end

function Geometry.simpleMinkowskiSum(p, r)
  tmp = Polygon:new() -- to store the minkowski sum
  
  for _,v in pairs(p) do
    for _,w in pairs(r) do
      tmp:append(v+w)
    end
  end
  
  return tmp:convexHull()
end

-- minkowski sum algorithm from page 279 in Computational Geometry
-- NOTE: there is a bug in the book, which is described in the code.
function Geometry.minkowskiSum(v, w)
  result = Polygon:new() -- to store the minkowski sum
  
  i = 1
  j = 1
  n = #v
  m = #w
  v[n+1] = v[1]
  w[m+1] = w[1]
    
  v[n+2] = v[2] -- Book bug: Must cover second since when j=m+1 or i=n+1, 
  w[m+2] = w[2] -- the other one might not have been reached.
                -- j+1 and i+1 will be accesses which might
                -- potentially be m+2 or n+2.
  print("dumping v:", n)
  for k, val in pairs(v) do
   print(val:toString())
  end
  
  print("dumping w:", m)
  for k, val in pairs(w) do
   print(val:toString())
  end
  
  local ang, ang_v, ang_w
  repeat
    result:append(v[i]+w[j])

    -- We need to do these two following checks to make sure
    -- that angle always increases in value as we trail around polygon.
    print("ang = angle(v[i], v[i+1])", i, v[i]:toString(), v[i+1]:toString())
    ang = angle(v[i], v[i+1])
    if ang_v and ang < ang_v then ang = 2.0*math.pi+ang end
    ang_v = ang
    
    print("ang = angle(w[j], w[j+1])", j, w[j]:toString(), w[j+1]:toString())    
    ang = angle(w[j], w[j+1])
    if ang_w and ang < ang_w then ang = 2.0*math.pi+ang end    
    ang_w = ang
        
    if  ang_v < ang_w then
      i = i+1
    elseif ang_v > ang_w then
      j = j+1
    else
      i = i+1
      j = j+1
    end
  until i == n + 1 and j == m + 1
  return result
end

--[[
  Sample n random numbers stratified. This means that
  we imagine a grid with sqrt(n) number of rows and 
  columns. Within each cell we sample a point, until we have
  n random samples. This gives more even distribution
  
  \param n number of samples should be able to sqare it
  \param box bounding box or rectangle describing area to sample points in
]]--
function Geometry.stratifiedSamples(n, box)
  local rows = math.floor(math.sqrt(n))
  local cell_width  = (box.max.x - box.min.x)/rows
  local cell_height = (box.max.y - box.min.y)/rows
  local samples = Collection:new()
  local y1 = box.min.y 
  local y2 = y1 + cell_height 
  for row = 1,rows do    
    local x1 = box.min.x
    local x2 = x1 + cell_width
    
    for col = 1,rows do
      samples:append(vec(math.random(x1, x2), math.random(y1, y2)))
      x1 = x2
      x2 = x2 + cell_width
    end
    
    y1 = y2    
    y2 = y2 + cell_height    
  end
  return samples
end

--[[
  Picks n random point within box
]]--
function Geometry.randomSamples(n, box)
  local points = Collection:new()
  for i=1,n do
    local pos = vec(0,0)
    pos.x = math.random(box.min.x, box.max.x)
    pos.y = math.random(box.min.y, box.max.y)  
    points:append(pos)
  end
  return points
end

--[[
	Polygon class
--]]
Polygon = Collection:new()

function Polygon:toSegments()
  return Geometry.toSegments(self)
end

function Polygon:toOpenSegments()
  return Geometry.toOpenSegments(self)
end

function Polygon:minkowskiSum(poly)
  return Geometry.minkowskiSum(self, poly)
end

function Polygon:__add(poly)
  return Geometry.minkowskiSum(self, poly)
end

function Polygon:rect(x, y, w, h)
  if w and h then
    return Polygon:new(vec(x,y), vec(x+w,y), vec(x+w, y+h), vec(x, y+h))
  else
    w = x
    h = y
    -- return Polygon:new(vec(0,0), vec(w,0), vec(w, h), vec(0, h))
    local minx = -w/2
    local maxx = w/2
    local miny = -h/2
    local maxy = h/2    
    
    return Polygon:new(vec(minx, miny), vec(maxx, miny), vec(maxx, maxy), vec(minx, maxy))        
  end
end

function Polygon:__unm()
  local tmp = self:map(function(x) return -x end)
  local index = tmp:minIndex(
    function(u, v) 
      return u.y < v.y or (u.y == v.y and u.x < v.x) 
    end
  )
  result = Polygon:new()
  for i=1,#self do
    result:append(tmp[index])
    index = index+1
    if index > #self then index = 1 end
  end
  return result
end

function Polygon:convexHull()
  return Geometry.convexHull(self)
end

function Polygon:print()
  self:foreach(function(v) v:print() end)
end

--[[
  Converts a collection of lua shapes to a Group.
  Group has some advantages in that it allows for collision detection
  of a shape against every member and it can be used for input
  to a GroupShape. A collection on the other hand has the advantage
  of supporting random access.
]]--
function Collection:toGroup()
  local group = Group:new()
  for _, shape in pairs(self) do
    group:add(shape)
  end
  return group
end

function Collection:toShapeGroup()
  return ShapeGroup:new(self:toGroup())
end

--[[
  Returns a function which will take a sprite
  as argument and produce the minkowski sum of the
  sprite and 'kernel' polygon
  
  You also set the color of all the sprites
  created
]]--
function Geometry.minkowskiAdder(kernel, red, green, blue)
  function adder(sprite)
    local poly = sprite:polygon()
    poly = Geometry.minkowskiSum(poly, kernel)
    local view = PolygonView:new(poly)
    view:setColor(red, green, blue)
    local minkowski_sprite = Sprite:new(view)
    minkowski_sprite:setPosition(sprite:position())
    return minkowski_sprite
  end
  
  return adder
end