require("script/timer")
require("script/geometry")
require("script/keyboard")
require("script/table")
require("script/functional")

function Engine.setFrameRate(rate)
  Engine.setTicksPerFrame(1000/rate)
end

function Engine.handleEvents()
  Engine.handleKeyClickEvents()  
  Engine.handleMouseClickEvents()
end

function Engine.registerMouseClickEvent(handler)
  local event = {}
  event.handler = handler
  event.armed = false
  Engine.mouseClickEvent = event
end

function Engine.handleMouseClickEvents()
  local event = Engine.mouseClickEvent
  if not event then return end
  if Engine.mouseButton then
    event.armed = true
  elseif event.armed then
    event.handler()
    event.armed = false
  end
end

--[[
	Group class
	-------------------------
	
	Extra methods for the sprite class.
--]]

Group = {}
ShapeGroup = {}

function Group:new()
  return Shape:newGroup()
end

function ShapeGroup:new(group)
  return Shape:newShapeGroup(group)
end

function Shape:add(sprite)
  sprite[self.__self] = self  -- Add group to sprite
  self:private_add(sprite)
end

function Shape:remove(sprite)
  sprite[self.__self] = nil -- Remove group from sprite
  self:private_remove(sprite)
end

--[[
	Sprite class
	-------------------------
	
	Extra methods for the sprite class.
--]]
function Sprite:kill()
  -- Remove all the group talbes from sprite
  for i,g in ipairs(self:groups()) do
    self[g.__self] = nil  -- remove group table from sprite
    g[self.__self] = nil  -- remove sprite table from group
  end
  
  -- Remove the sprite from in the C++ part too
  self:private_kill()
end

function Sprite:segments()
  local segs = Geometry.toSegments(self:polygon())
  for _,seg in pairs(segs) do
    seg.data = self:id()
  end
  return segs
end