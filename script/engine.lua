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

--[[
	Sprite class
	-------------------------
	
	Extra methods for the sprite class.
--]]
function Sprite:segments()
  local segs = Geometry.toSegments(self:polygon())
  for _,seg in pairs(segs) do
    seg.data = self:id()
  end
  return segs
end