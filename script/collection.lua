--[[
	Collection class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	
	Wraps table and allows you to use the table in a more convenient manner.
	Attempts to duplicate some of the handy ways you can handle collections
	in Ruby and Python.
	
--]]

Collection = {}

-- Creation
function Collection:new(...)
  -- Create instance
  local instance = {}
  setmetatable(instance, self)
  self.__index = self

  instance:append(...)
  return instance
end

-- Accessors
function Collection:first()
  return self[1]
end

function Collection:last()
  return self[#self]
end

-- Request
function Collection:size()
  return #self
end

function Collection:empty()
  return #self == 0
end

function Collection:contains(element)
  for _,v in pairs(self) do
    if v == element then return true end
  end
  return false
end

-- Calculations
function Collection:foreach(fn)
  for _,v in pairs(self) do
    fn(v)
  end  
end

-- Return a set which is the result of applying fn(e) to each element 'e' of collection
function Collection:map(fn)
  local result = Collection:new()
  for _,v in pairs(self) do
    table.insert(result, fn(v))
  end
  return result
end

--[[
  The result is a combination of all elements of collection. Elements are combined
  by calling 'prev = fn(e, prev)' for each element 'e' where 'prev' is initially set
  to 'init' and final result is last call of 'fn'
]]--

function Collection:inject(fn, init)
  local result = init
  for _,v in pairs(self) do
    result = fn(v, result)
  end
  return result
end

-- Return a subset of self where fn(v) = true, where 'v' in self
function Collection:filter(fn)
  local result = Collection:new()
  for _,v in pairs(self) do
    if fn(v) then table.insert(result, v) end
  end
  return result
end

-- Returns element 'e' in self for which fn(e) is largest
function Collection:max(comp)
  local index = self:maxIndex(comp)
  return self[index]
end

function Collection:removeMax(comp)
  local index = self:maxIndex(comp)
  local max_value = self[index]
  table.remove(self, index)
  return max_value
end

function Collection:maxIndex(comp)
  local max_index = 1
  if #self > 1 then
    if comp then
      for i,v in ipairs(self) do
        if comp(v, self[max_index]) then max_index = i end
      end
    else
      for i,v in ipairs(self) do
        if v > self[max_index] then max_index = i end
      end
    end
  end
  return max_index
end

-- Returns element 'e' in self for which fn(e) is smallest
function Collection:min(comp)
  local index = self:minIndex(comp)
  return self[index]
end

function Collection:removeMin(comp)
  local index = self:minIndex(comp)
  local min_value = self[index]
  table.remove(self, index)
  return min_value
end

function Collection:minIndex(comp)
  local min_index = 1
  if #self > 1 then  
    if comp then
      for i,v in ipairs(self) do
        if comp(v, self[min_index]) then min_index = i end
      end
    else
      for i,v in ipairs(self) do
        if v < self[min_index] then min_index = i end
      end
    end
  end
  return min_index
end

function Collection:find(fn)
  return self[self:findIndex(fn)]
end

--[[
  Return the index 'i' of 'self' where 'fn(self[i]) == true'
  If 'fn' is not a function then collection will return 'i' where
  'self[i] == fn'
--]]
function Collection:findIndex(fn)
  if type(fn) == "function" then
    for i,v in ipairs(self) do
      if fn(v) then return i end
    end
  else
    for i,v in ipairs(self) do
      if fn == v then return i end
    end  
  end
  return nil
end

-- Operations
function Collection:append(...)
  for _,v in ipairs({...}) do
    table.insert(self, v)
  end
end

function Collection:prepend(...)
  for i = arg.n, 1, -1 do
    table.insert(self, 1, arg[i])
  end
end

function Collection:popFront()
  return table.remove(self, 1)
end

function Collection:popBack()
  return table.remove(self)
end

function Collection:join(...)
  for _,t in ipairs({...}) do
    for _,v in pairs(t) do
      table.insert(self, v)
    end
  end
end

function Collection:sort(comp)
  table.sort(self, comp)
end

function Collection:toString()
  local strings = self:map(function(e)    
    if type(e) == "table" and e.toString then 
      return e:toString() 
    elseif type(e) == "string" then
      return "\""..e.."\""
    end    
    return e
  end)
  
  return "Collection:new("..table.concat(strings, ", ")..")"
end

function Collection:print()
  print(self:toString())
end