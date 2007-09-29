require("script/stack")

--[[
	Trapezoid class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  A trapezoid is defined by a top and bottom segment and a left and right
  end point. A trapezoid can have at most 2 neighboring trapezoids on each side.
  
  This file defines extra methods for the trapezoid not defined in C++
	
--]]

require("script/edge")

function Trapezoid:toString()
  return toString(self:tag())
end

function Trapezoid:centerSide(index)
  if side == 0 then 
    return self:centerLeft() 
  elseif side == 1 then
    return self:centerRight()
  end
end

function Trapezoid:printNeighbors()
  if self:lowerLeft() then   print("lower left", self:lowerLeft():tag()) end
  if self:upperLeft() then   print("upper left", self:upperLeft():tag()) end
  if self:lowerRight() then   print("lower right", self:lowerRight():tag()) end
  if self:upperRight() then   print("upper right", self:upperRight():tag()) end
end

function Trapezoid:printGraph()
  local visisted = {}
  local parent = nil
  local edges = {}
  
  print("digraph trapezoids {")
  
  function visit(w)
    visisted[w:id()] = true
    
    -- if parent then
    --   print(parent:tag().." -> "..w:tag())
    --   if w:isBordering(parent) then
    --     print(w:tag().." -> "..parent:tag())      
    --   end
    -- end
  end

  local p = Stack:new()
  visit(self)  
  p:push(self)
    
  while not p:empty() do
    parent = p:pop()
    for _,w in pairs(parent:neighbors()) do
      local edge = Edge:new(parent, w)
      if not contains(edges, edge) then 
        edge:print()
        -- print(parent:tag().." -> "..w:tag())
        table.insert(edges, edge)
      end
      if not visisted[w:id()] then
        visit(w)
        p:push(w)
      end
    end
  end
  print("}")
end

function Trapezoid:drawGraph()
  local visisted = {}
  local parent = nil
  local edges = {}
    
  function visit(w)
    visisted[w:id()] = true    
    if parent then
      table.insert(edges, Edge:new(parent, w))
    end
  end
  
  local p = Stack:new()
  visit(self)  
  p:push(self)
    
  while not p:empty() do
    parent = p:pop()
    for _,w in pairs(parent:neighbors()) do
      if not visisted[w:id()] then
        visit(w)
        p:push(w)
      end
    end
  end
  
  local segs = {}
  for _,edge in pairs(edges) do
    local e1 = edge[1]
    local e2 = edge[2]
    
    local c1 = e1:center()
    local c2 = e2:center()
    local c3 = nil
    if c1 < c2 then
      if e1:right() == e1:bottom():right() or e1:right() == e1:top():right() then
        c3 = e1:centerRight()
      else
        c3 = e2:centerLeft()
      end
    elseif c1 > c2 then
      if e1:left() == e1:bottom():left() or e1:left() == e1:top():left() then
        c3 = e1:centerLeft()
      else
        c3 = e2:centerRight()
      end
    else
      print("Trapezoids should not have the same center!")
    end
    
    table.insert(segs, seg(c1, c3))
    table.insert(segs, seg(c3, c2))    
  end
  local sprite = Geometry.viewSegments(segs)
  sprite:view():setArrowVisible(true)
  sprite:view():setColor(1,1,0)
  return sprite
end

function Trapezoid:edgeData(edgeTag)
  local visisted = {}
  local parent = nil
  local edges = {}
    
  function visit(w)
    visisted[w:id()] = true    
    -- if parent then
    --   table.insert(edges, Edge:new(parent, w))
    -- end
  end
  
  local p = Stack:new()
  visit(self)  
  p:push(self)
    
  while not p:empty() do
    parent = p:pop()
    for _,w in pairs(parent:neighbors()) do
      local edge = Edge:new(parent, w)
      if not contains(edges, edge) then 
        table.insert(edges, edge)
      end

      if not visisted[w:id()] then
        visit(w)
        p:push(w)
      end
    end
  end
  
  local edgeData = {}
  for _,edge in pairs(edges) do
    local e1 = edge[1]
    local e2 = edge[2]
    
    local c1 = e1:center()
    local c2 = e2:center()
    local c3 = nil
    if c1 < c2 then
      if e1:right() == e1:bottom():right() or e1:right() == e1:top():right() then
        c3 = e1:centerRight()
      else
        c3 = e2:centerLeft()
      end
    elseif c1 > c2 then
      if e1:left() == e1:bottom():left() or e1:left() == e1:top():left() then
        c3 = e1:centerLeft()
      else
        c3 = e2:centerRight()
      end
    else
      print("Trapezoids should not have the same center!")
    end
    
    table.insert(edgeData, EdgeData:new(e1, e2, edgeTag, c3))    
    -- table.insert(edgeData, EdgeData:new(edgeTag, c3, e1))
    -- table.insert(edgeData, EdgeData:new(edgeTag, c3, e2))
    edgeTag = edgeTag+1    
  end
  return edgeData
end

function trapezoidsView(traps)
  local segs = {}      
  for _, trap in pairs(traps) do
    for _,x in pairs({trap:left().x, trap:right().x}) do
      local vertical = seg(vec(x, ymin), vec(x, ymax))
      local bottom = trap:bottom():intersect(vertical)
      local top = trap:top():intersect(vertical)
      if bottom and top then
        table.insert(segs, seg(bottom, top))
      end
    end
  end
  return SegmentView:new(segs)
end