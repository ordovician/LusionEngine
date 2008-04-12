--[[
	Utility functions.
--]]

-- joins to tables into one table
function table.union(...)
  local c = {}
  for _,t in ipairs({...}) do
    for _,v in pairs(t) do
      table.insert(c, v)
    end
  end
  return c
end

-- Check if a table contains element
function table.contains(array, element)
  for _,v in pairs(array) do
    if v == element then return true end
  end
  return false
end

--[[
   table.bsearch(t, value [, fcompval] [, reversed])

   Searches the table t through binary search for value. If the
     value is found, it returns the index
     and the corresponding value in the table.
   If fcompval is given then it must be a function that takes
     one value and returns a second value2
     to be compared with the input value, e.g.
     compvalue = function( value ) return value[1] end
   If reverse is set to true then the search assumes that the table
     is sorted in reverse order (largest value at position 1)
   Return value:
     on success: the index and corresponding value, e.g. i, t[i]
     on failure: nil

   Lua 5.1 compatible
]]--

do
  -- Avoid heap allocs per function call.
  local default_fcompval = function(value) return value end
  local fcompf = function(a, b) return a < b end
  local fcompr = function(a, b) return a > b end
function table.bsearch(t, value, fcompval, reversed)

  -- initialise Functions
  local fcompval = fcompval or default_fcompval
  local fcomp = reversed and fcompr or fcompf

  --  Initialise Numbers
  local iStart, iEnd, iMid = 1, #t, 1

  -- Binary Search
  while iStart <= iEnd do

    -- calculate middle
    iMid = math.floor((iStart + iEnd) / 2)

    -- get compare value
    local value2 = fcompval(t[iMid])

    if value == value2 then
      return iMid, t[iMid]
    end

    if fcomp(value , value2) then
      iEnd = iMid - 1
    else
      iStart = iMid + 1
    end
  end
end end

--[[
   table.binsert( table, value [, comp] )

   Inserts a given value through BinaryInsert into the table
     sorted by [,fcomp].
   If fcomp is given, then it must be a function that receives
     two table elements, and returns true when the first is less
     than the second, e.g. fcomp = function(a, b) return a > b end ,
     will give a sorted table, with the biggest value on position 1.
     [, comp] behaves as in table.sort(table, value [, comp])
   returns the index where 'value' was inserted

   Lua 5.1 compatible
]]--
 
-- from http://lua-users.org/wiki/BinaryInsert
do
  -- Avoid heap allocs for performance
  local fcomp_default = function(a, b) return a < b end
function table.binsert(t, value, fcomp)

  -- Initialise Compare function
  local fcomp = fcomp or fcomp_default

  --  Initialise Numbers
  local iStart, iEnd, iMid, iState = 1, #t, 1, 0

  -- Get Insertposition
  while iStart <= iEnd do
    
    -- calculate middle
    iMid = math.floor((iStart + iEnd) / 2)

    -- compare
    if fcomp(value , t[iMid]) then
      iEnd = iMid - 1
      iState = 0
    else
      iStart = iMid + 1
      iState = 1
    end
  end
  table.insert(t, (iMid + iState), value)
  return (iMid + iState)
end end