--[[
  Created by Erik Engheim on 25/12/2007
  Copyright 2007 Translusion. All rights reserved.

	Functional utilities
	-------------------------
	
	functions assisting in functional programming.
--]]

function lessThan(a, b)
  return a < b
end

function greaterThan(a, b)
  return a > b
end

function lessOrEqual(a, b)
  return a <= b
end

function greaterOrEqual(a, b)
  return a >= b
end

function equal(a, b)
  return a == b
end

function notEqual(a, b)
  return a ~= b
end

-- Higher order function to make a binary function from two functions
function makeBinary(converter, operation)
  function binary(a, b)
    return operation(converter(a), converter(b))
  end
  return binary
end