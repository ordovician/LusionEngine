--[[
	Stack class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  Supports pushing and popping of values onto a stack
	
--]]

require("script/collection")

Stack = Collection:new()

function Stack:push(...)
  self:append(arg)
end

function Stack:pop()
  return table.remove(self)
end

function Stack:top()
  return self[#self]
end

function Stack:bottom()
  return self[1]
end