--[[
	Map class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
  Wraps a table to make it more convenient to deal with it as a Map
--]]

Map = {}

-- Creation
function Map:new(...)
  -- Create instance
  local instance = {}
  setmetatable(instance, self)
  self.__index = self

  for _,v in ipairs({...}) do
    instance[v] = v
  end
  return instance
end

-- Calculations
function Map:foreach(fn)
  for _,v in pairs(self) do
    fn(v)
  end  
end

-- Returns a table which is the result of applying fn(e) to each element 'e' of Map
function Map:map(fn)
  local result = Map:new()
  for k,v in pairs(self) do
    result[k] = fn(v)
  end
  return result
end

-- Return a sub map of self where fn(v) = true, where 'v' in self
function Map:filter(fn)
  local result = Map:new()
  for k,v in pairs(self) do
    if fn(v) then result[k] = v end
  end
  return result
end

-- Finds key for element with smallest value
function Map:minKey()
  local k_min,_ = next(self, nil)
  for k,v in pairs(self) do
    if v < self[k_min] then k_min = k end
  end
  return k_min
end

function Map:maxKey()
  local k_max,_ = next(self, nil)
  for k,v in pairs(self) do
    if v > self[k_max] then k_max = k end
  end
  return k_max
end

-- Operations
function Map:join(...)
  for _,t in ipairs({...}) do
    for k,v in pairs(t) do
      self[k] = v
    end
  end
end

function Map:toString()
  local strings = self:map(function(e)    
    if type(e) == "table" and e.toString then 
      return e:toString() 
    elseif type(e) == "string" then
      return "\""..e.."\""
    end    
    return e
  end)
  
  return "Map:new("..table.concat(strings, ", ")..")"
end

function Map:print()
  print(self:toString())
end