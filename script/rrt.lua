require("script/geometry")
require("script/queue")

--[[
  A moving object will trace a different trajectory for each angular velocity it
  starts with. Likewise there will be a different trajectory for each angular velocity
  it ends with. A higher ending angular velocity requires a higher angular acceleration which
  naturally will lead to a more curved trajectory. 
  
  Thus each pair of starting and ending angular velocity gives a unique trajectory.
  Thus function computes the final position and orientation of several trajectories
  starting at point (0,0) with orientation 0. 
  
  Starting and ending angular velocity pairs are computet from the range of angular
  velocities starting with 'angvel_min' and ending with 'angvel_max'
  
  'angvel_steps' determines how many trajectories are created. 
--]]
function Geometry.createTrajectories(s0, angvel_min, angvel_max, angvel_steps, dt, steps)  
  local t = dt*steps
  function createTrajectory(ang_vel_start, ang_vel_end)
    local ang_accel = (ang_vel_end - ang_vel_start)/t

    local s = s0:copy()
    s:setAngularVelocity(ang_vel_start)
    s:setAngularAcceleration(ang_accel)
    s:integrate(dt, steps)
    return s
  end

  local trajectories = Collection:new()
  local angvel_delta = (angvel_max-angvel_min)/angvel_steps
  for angvel_start=angvel_min,angvel_max,angvel_delta do
    trajectories[angvel_start] = Collection:new()
    for angvel_end=angvel_min,angvel_max,angvel_delta do
      trajectories[angvel_start][angvel_end] = createTrajectory(angvel_start, angvel_end)
    end
  end
  
  function interpolate(angvel)
    local angvel = math.floor((angvel/angvel_delta)+0.5)*angvel_delta
    return trajectories[angvel]
  end
  return interpolate
end

--[[ 
  Geometry.combineBehavior({weight_0, function_0}, {weight_1, function_1},...)

  Combines the value of several goodness functions. Each goodness function is weighted
  if each goodness function has value in range [0,1] then combined value will be in
  range [0,1] too.
]]--  
function Geometry.combineBehavior(...)
  local varg = {...}
  function combined(s0, s1)
    local numerator = 0
    local denominator = 0
    for _,v in ipairs(varg) do
      local w = v[1]
      local f = v[2]
      numerator = numerator+w*f(s0, s1)
      denominator = denominator+w
    end
    return numerator/denominator  -- NOTE: we do not check if denominator is zero
  end
  return combined
end

--[[ 
  A factory method that creates and returns a seek steering behavior
  function that seeks towards 'target'. The returned function 'seek(s0, s1)' will take
  two arguments 's0' and 's1' which represent the start and end state of a sub path
  which is to be evaluated for its goodness with respect to reachng 'target'
]]--
function Geometry.makeSeek(target)
  -- Goodness of trajectory from state 's0' to 's1'
  function seek(s0, s1)
    local p0 = s0:position()
    local p1 = s1:position()
    local d1 = s1:direction()
    local dir_target = (target-p1):unit()  -- direction to target
    local dir_path = (p1-p0):unit()   -- direction of from current point to next on path
    return 0.25*(1 + d1*dir_target)*(1 + dir_path * dir_target)
  end
  return seek
end

--[[
  Factory method to create a goodness function that rewards flank positions against
  'enemy' plane. The 'distance_factor' is a weight factor for importance of flanking.

  If 'd_f' is distance factor and 'd' is distance then 'd_f < d' means flanking goodness
  is taken less into consideration. When 'd_f = d' flanking goodness is taken into neutral
  consideration. For 'd_f > d' flanking is taken in excessive consideration. 
  Meaning as 'd' decrease flanking is taken increasingly into consideration. 
]]--
function Geometry.makeFlank(enemy, distance_factor)
  local xe = enemy:position()
  local ve = enemy:direction()

  function flank(s0, s1)
    local xn = s1:position()
    local d = xe-xn
    local v = d:unit()
    local flank_value = 0.5*(1 + ve*v)  -- How good NPC flanks enemy, range [0,1]
    d = d:length()
    if d > distance_factor then 
      return 0
    else
      return flank_value
    end
  end
  return flank
end

--[[
  Creates an evaluation function. The created function 'eval(s)' will eval
  the goodness of state 's' based on the supplied 'goodness' function and
  'discount' and 'optimism' factors.
  
  The factors have the following ranges
    optimism = [-1, 1] low value cause deeper exploration
    discount = (0,1)
]]--
function Geometry.makeEval(goodness, discount, optimism)
  function eval(s)
    -- calculate value 'v' of 's'
    if s.parent then
      s.value = s.parent.value+goodness(s.parent, s)*math.pow(discount, s.depth-1)    
  
      -- And effective value 'ev' of 's'
      return s.value+optimism*math.pow(discount, s.depth)/(1-discount)
    else
      return 0  -- First state has no value 
    end
  end
  return eval
end

--[[
  Uses rapidly expanding random trees to find a path to a goal.
  's_0' is start state for search.
--]]
function Geometry.rrtSearch(s_0, trajectories, eval, max_depth, start_time, delta_time)
  -- Generate the sucessor states to 's_m'
  function successors(s_m)
    local trans = s_m:transform()
    local p_start = vec(0, 0)
  
    local angvel = s_m:angularVelocity()
    local traj = trajectories(angvel)
    local succ = traj:map(function(state) 
      local s = state:copy()
      local tranform = trans*s:transform()
      local pos = tranform:transformPoint(p_start)
      
      s:setPosition(pos)
      s:rotate(s_m:rotation())
      s.parent = s_m
      
      return s
    end)
    return succ
  end
  
  local visisted = Collection:new(s_0)
  local expanded = Collection:new()
  local candidates = Collection:new(s_0)

  s_0.depth = 0
  s_0.value = 0

  -- Actual algorithm, a sort of A* search
  while Engine.seconds() < start_time+delta_time and not candidates:empty() do
    local s_m = candidates:removeMax(eval)
    local succ = nil
    if s_m.depth < max_depth then
      succ = successors(s_m)
      for _,s in pairs(succ) do s.depth = s_m.depth+1 end
      visisted:append(unpack(succ))
      candidates:append(unpack(succ))
    end
    expanded:append(s_m)        
  end
  return visisted
end

--[[
  Performs an Rapidly expanding Random Tree search and returns the 
  motion state that is best to select at moment to reach goal
]]--
function Geometry.rrtState(s_0, trajectories, eval, max_depth, start_time, delta_time)
  local visisted = Geometry.rrtSearch(s_0, trajectories, eval, max_depth, start_time, delta_time)
  local s = visisted:max(eval)
  while s and s.parent ~= s_0 do
    s = s.parent
  end
  return s  
end

--[[
  Performs an Rapidly expanding Random Tree search and returns the 
  path to the goal
]]--
function Geometry.rrtStates(s_0, trajectories, eval, max_depth, start_time, delta_time)
  local visisted = Geometry.rrtSearch(s_0, trajectories, eval, max_depth, start_time, delta_time)
  return Geometry.bestPath(s_0, visisted, eval)
end

function Geometry.bestPath(s_0, visisted, eval)
  local s = visisted:max(eval)
  local states = Queue:new()  
  while s ~= s_0 do
    states:prepend(s)    
    s = s.parent
  end
  return states  
end

-- Convert collection of states to a polygon representing path
function Geometry.statesToPath(states, dt, steps)
  local paths = Geometry.statePaths(states, dt, steps)
  return Polygon:new(unpack(table.union(unpack(paths))))
end

-- Return the path between state 's0' and 's1'
function Geometry.statePath(s0, s1, dt, steps)
  local s = s0:copy()
  s:setAngularAcceleration(s1:angularAcceleration())
  return Polygon:new(unpack(s:integratePath(dt, steps)))
end

function Geometry.statePaths(states, dt, steps)
  local paths = Collection:new()
  for _,s in pairs(states) do
    if s.parent then
      paths:append(Geometry.statePath(s.parent, s, dt, steps))
    end
  end
  return paths
end

function Geometry.showStatePaths(states, dt, steps)
  local paths = Geometry.statePaths(states, dt, steps)
  local segs = {}
  segs = paths:inject(function(path, prev) 
    return table.union(path:toOpenSegments(), prev) 
  end, segs)
  
  local view = SegmentView:new(segs)
  local sprite = Sprite:new()
  -- sprite:setView(view)
  -- return sprite
  return Sprite:new(view)
end

-- Converts a sequence of motion states to a sequence of points in space
function Geometry.localStatesToPath(states)
  local path = Polygon:new()
  local m = Matrix:identity()
  local p_start = vec(0,0)
  for _,s in pairs(states) do
    m = m*s:transform()
    local pos = m:transformPoint(p_start)
    path:append(pos)
  end
  
  return path
end