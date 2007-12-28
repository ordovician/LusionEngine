--[[
	Set class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  Wraps a table to make it more convenient to deal with it as a set
--]]

Set = {}

-- Creation
function Set:new(...)
  -- Create instance
  local instance = {}
  setmetatable(instance, self)
  self.__index = self
  
  for _,v in ipairs(arg) do
    instance[v] = v
  end
  return instance
end

-- Request
function Set:size()
  return #self
end

function Set:empty()
  return #self == 0
end

function Set:contains(element)
  return self[element] == element
end

-- Calculations
function Set:foreach(fn)
  for _,v in pairs(self) do
    fn(v)
  end  
end

-- Return a set which is the result of applying fn(e) to each element 'e' of set
function Set:map(fn)
  local result = Set:new()
  for k,v in pairs(self) do
    result[k] = fn(v)
  end
  return result
end

-- Return a subset of self where fn(v) = true, where 'v' in self
function Set:filter(fn)
  local result = Set:new()
  for k,v in pairs(self) do
    if fn(v) then result[k] = v end
  end
  return result
end

-- Operations
function Set:insert(...)
  for _,v in ipairs(arg) do
    self[v] = v
  end
end

function Set:join(...)
  for _,t in ipairs(arg) do
    for k,v in pairs(t) do
      self[k] = v
    end
  end
end

function Set:toString()
  local strings = self:map(function(e)    
    if type(e) == "table" and e.toString then 
      return e:toString() 
    elseif type(e) == "string" then
      return "\""..e.."\""
    end    
    return e
  end)
  
  return "Set:new("..table.concat(strings, ", ")..")"
end

function Set:print()
  print(self:toString())
end