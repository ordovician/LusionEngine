--[[
	Queue class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  Supports queue operations
	
--]]

require("script/collection")

Queue = Collection:new()

-- Enqueue element
function Queue:push(...)
  self:append(...)
end

-- Remove element first in queue
function Queue:pop()
  return table.remove(self, 1)
end