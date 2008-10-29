--[[
	Timer class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim
	Copyright 2007 Translusion. All rights reserved.
  
	Timer class is used to time the number of miliseconds it takes to perform an operation.
	So this class can be used for performance testing

	usage:
	t = Timer:start()
	
	-- Do some time consuming operation
  
  local elapsed_millies = t:stop()
	
--]]


Timer = {startTime = 0, stopTime = 0}
function Timer:start()
  local timer = {startTime = Engine.ticks()}
  setmetatable(timer, self)
  self.__index = self
  return timer
end

-- Time elapsed since start
function Timer:elapsed()
  return Engine.ticks() - self.startTime
end

function Timer:time()
  return self.stopTime - self.startTime
end

function Timer:stop()
  self.stopTime = Engine.ticks()
  return self:time()
end

--[[
	AvgTimer class
	-------------------------
	
	created: 14/02/2007
	author: Erik Engheim

	This is similar to Timer class but is used when one needs to calculate the average
	time of an operation that is performed multiple times.

	usage:
	t = AvgTimer:start()
	
	-- We perform our time consuming operation n times
	for i=1,n do
	  t:start()
	  -- Do some time consuming operation
	  t:stop()
  end
  	
--]]

AvgTimer = {}
function AvgTimer:new(times)
  local avgTimer = {totalTime = 0, count = 0, dispTimes = times, prevTime = 0}
  setmetatable(avgTimer, self)
  self.__index = self
  return avgTimer  
end

function AvgTimer:start()
  self.timer = Timer:start() 
end

function AvgTimer:avgTime()
  return self.totalTime/self.count
end

function AvgTimer:stop()
  local t = self.timer:stop()
  self.totalTime = self.totalTime + t
  self.count = self.count + 1
  local avg = self.totalTime/self.count
  if self.count % self.dispTimes == 0 or (t > 6*avg and t > self.prevTime) then
    print("Average time is", avg, "after",self.count,"iterations")
    print("Time for this lap", t)
  end
  self.prevTime = t
  return avg
end