--[[
  Created by Erik Engheim on 25/12/2007
  Copyright 2007 Translusion. All rights reserved.
]]--

--[[
	Stack class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
  Copyright 2007 Translusion. All rights reserved.
  	
  Supports pushing and popping of values onto a stack
	
--]]

require("script/collection")

Stack = Collection:new()

function Stack:push(...)
  self:append(...)
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