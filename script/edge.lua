--[[
	Edge class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  Defines and edge in the geometric sense. An edge is simply a pair
  of nodes. A node can be a number, a string or anything else. 
	
--]]

Edge = {}

function Edge:new(source, target)
  obj = {source, target} 
  setmetatable(obj, self)
  self.__index = self
  return obj
end

-- Check if two edges are he same. It consideres edges going in opposite direction
-- to be equal
function Edge:__eq(edge)
  return self[1] == edge[1] and self[2] == edge[2] or (self[1] == edge[2] and self[2] == edge[1])
end

function Edge:print()
  print(self[1]:tag().." -> "..self[2]:tag())
end