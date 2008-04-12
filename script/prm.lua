require("script/algorithm")
require("script/timer")

--[[
	PrmNode class
	-------------------------
	
	created: 05/09/2007
	author: Erik Engheim
	
  Implements a node in a probablistic roadmap
	
--]]
PrmNode = GraphNode:new()

-- Creation
function PrmNode:newNode(pos, radius)
  -- Create instance
  local node = PrmNode:new()
  node:init(pos, radius)
  return node
end

function PrmNode:init(pos, radius)
  self.circle = Shape:newCircle(pos, radius)
end

function PrmNode:position()
  return self.circle:center()
end

function PrmNode:radius()
  return self.circle:radius()
end

function PrmNode:distanceTo(n)
  return (self:position() - n:position()):length()
end

function PrmNode:toString()
  return "pos "..self:position():toString().." radius "..self:radius()
end

--[[
	ProbablisticRoadMap class
	-------------------------
	
	created: 05/09/2007
	author: Erik Engheim
	
  Implements a probablistic roadmap
	
--]]

 -- defines

MAX_DIST			=	10.0			-- maximum distance a sample can move in search for nearest obstacle  
MAX_DIST_SQUARED	=	MAX_DIST * MAX_DIST	-- maximum distance squared
MAX_EDGE_SIZE	=		10.0			 -- maximum edge size
MAX_RADIUS		=	100.0			-- maximum sphere radius
MIN_RADIUS		=	1.0			-- minimum sphere radius
ACCURACY		=	0.5				 -- a measure for the accuracy during roadmap construction

ProbablisticRoadMap = {}

-- Creation
function ProbablisticRoadMap:new(obstacles, box)
  -- Create instance
  local instance = {}
  setmetatable(instance, self)
  self.__index = self
  
  instance:init(obstacles, box)
  return instance
end

function ProbablisticRoadMap:init(obstacles, box)
  self.bbox = box
  self.obstacles = obstacles
  self.nodes = Collection:new()
end

--[[
	Constructs a roadmap with samples number of nodes. Samples is an integer.
	retract_quotient is a number between 0-1 which indicates percentage of
	samples which should not be retracted. E.g. a quotient of 0.1 means that
	the 10% last samples are not retracted.
--]]
function ProbablisticRoadMap:construct(no_samples, retract_quotient)
  local temp_samples = Geometry.stratifiedSamples(no_samples, self.bbox)
  -- local samples = Geometry.randomSamples(no_samples, self.bbox)
  
  local samples = Collection:new()
  
  -- Remove samples inside obstacles
  for _,c in pairs(temp_samples) do
    if not self.obstacles:inside(c) then
      samples:append(c)
    else
      print("Throwing out sample ", c:toString())
    end
  end
  no_samples = #samples
	local halftime = no_samples * (1 - retract_quotient)
	  
	local sampling_time = 0
	local free_disc_time = 0
	
	while  no_samples > 0 do

    local t = Timer:start()
		-- get sample C and retract to voronoi plane
		local c = samples[no_samples]

		if no_samples > halftime then
			c = self:retractSample(c)
		end
    sampling_time = sampling_time + t:stop()
    if sampling_time > 1000 then
      print("Sampling takes too long time, sampling terminated...")
      break
    end
    
    t = Timer:start()
		-- check whether sample is outside all discs in the roadmap
    if self.bbox:inside(c) and not self:insideRoadMap(c) then      
      --  add node to roadmap
      local radius = self:largestFreeDisc(c)
      if radius > MIN_RADIUS then      
        radius = math.min(radius, MAX_RADIUS)
        local node = PrmNode:newNode(c, radius)
        self.nodes:append(node)
      end
    end
    free_disc_time = free_disc_time + t:stop()
    sampling_time = sampling_time + t:stop()
    if free_disc_time > 1000 then
      print("Find free space takes too long time, sampling terminated...")
      break
    end
    
    
		no_samples = no_samples-1
	end

  print("Total sampling time:", sampling_time)
  print("Total time spend finding collision free disc:", free_disc_time)

  local t = Timer:start()
  self:connectOverlappingCircles()
  print("Time spent connecting nodes:", t:stop())
  
  -- removes unconnected nodes
  print("cleaning up...")  
	self:cleanUp()
	
  print("Making node search structure...")  	
  self:makeNodeSearchStructure()
	return true;  
end

--[[
 Brute force algorithm to compare every circle surrounding node with each other and
 find the ones overlapping. Creates edges between the ones that overlap.
]]--
function ProbablisticRoadMap:connectOverlappingCircles()
  -- add edges between nodes with overlapping discs
  for _,n in pairs(self.nodes) do
    for _,m in pairs(self.nodes) do
      -- check whether spheres are overlapping and the nodes are not connected by an edge yet
  	  if n ~= m and n.circle:collide(m.circle) and not n:hasNeighbor(m) then
  	    -- To prevent edges through obstacles perform an additional collision check
  			if not self:lineCollision(n, m) then
          n:insertNeighbors(m)
          m:insertNeighbors(n)
  			end
			
  	  end
    end
  end
end

--[[
  Converts table 't' to a unique number
]]--
function tableToNumber(t)
  return tonumber(string.match(tostring(t), '0x[0-9a-f]+'))
end

--[[
  Creates a string representation of roadmap
  which describes how to construct roadmap as a lua program
]]--
function ProbablisticRoadMap:toString()
  local header = [[
local nodes = Collection:new()
  
]]
  local node_strings = Collection:new()
  local node_mapping = {}
  for i, n in ipairs(self.nodes) do
    local s = string.format('{%s, %f}', n:position():toString(), n:radius())
    node_strings:append(s)
    node_mapping[tableToNumber(n)] = i
  end
  local node_data = 'local node_data = {'..table.concat(node_strings, ',\n')..'}\n\n'
  local fillup = [[
for _, n in pairs(node_data) do
  nodes:append(PrmNode:newNode(n[1], n[2]))
end]]
  local neighbor_lines = '\n\n'
  for i, n in pairs(self.nodes) do
    local node_neighbors = Collection:new()
    for _, m in pairs(n:neighbors()) do
      node_neighbors:append('nodes['..node_mapping[tableToNumber(m)]..']')
    end    
    neighbor_lines = neighbor_lines..'nodes['..i..']:insertNeighbors('..table.concat(node_neighbors, ', ')..')\n'
  end
  
  return header..node_data..fillup..neighbor_lines..'\nreturn nodes'
end

--[[
  Save present roadmap to file as a lua script which when executed will
  reconstruct this roadmap
]]--
function ProbablisticRoadMap:save(filename)
  local file = io.open(filename, 'w')
  file:write(self:toString())
  file:close()
end

--[[
  'filename' is assumed to be a lua file which when executed
  constructs a roadmap and returns it as a collection of PrmNodes
  with edges.
]]--
function ProbablisticRoadMap:load(filename)
  self.nodes = dofile(filename)
  self:makeNodeSearchStructure()  
end

--[[
  Can be removed later. Only used for displaying roadmap.
]]--
function ProbablisticRoadMap:displayRoadMap()
  line_seg = nil
  for _, n in pairs(self.nodes) do
    for _, m in pairs(n:neighbors()) do
      line_seg = Shape:newSegment(n:position(), m:position())	  
    end
  end
end

--[[
  Can be removed later. Only used for displaying roadmap. without
  entering same node twice
]]--
function ProbablisticRoadMap:displayRoadPath()
  local prev = nil
  line_seg = nil
  Graph.breathFirstSearch(self.nodes[1], function(n)
    if prev then
      line_seg = Shape:newSegment(prev:position(), n:position())
      end
      prev = n    
    end)
end

--[[
  When an NPC wants to find its way through the world using a roadmap
  it will provide us with its current position and from that we should
  find a path through roadmap. 
  
  Since positions don't translate directly to a node and searching every
  node to see if its disc contains the NPC position is too time consuming
  (takes O(n) time). We need a search structure to quickly find a node give
  a query point. This method will create a search structure in self.circleGroup
  The structure is a bounding volume hierarchy of the discs surrounding the
  nodes.
]]--
function ProbablisticRoadMap:makeNodeSearchStructure()
  -- Mapping from circle object to PrmNode, so that given
  -- a circle contained within a PrmNode we can find the corresponding
  -- node
  self.circleNodeMap = {}

  -- Have to go through a few hoops because ShapeGroup
  -- can only take Group as input  
  local circles = Group:new()
  for _, node in pairs(self.nodes) do
    circles:add(node.circle)
    self.circleNodeMap[node.circle] = node
  end
  self.circleGroup = ShapeGroup:new(circles)
end

--[[
  Use a breath first search among all added nodes to find the
  largest connected component of nodes. Remove all nodes not 
  belonging to this component.
  
  Algorithm works by constantly looking for a node that hasn't been
  visistet then search through all nodes reachable from it and count
  that as a component. This is recorded as one component with start node
  and number of nodes in it.
  
  Next component is found in similar way. We only keep the largest
  component at each iteration so that at the end we are left with
  the largest component.
]]--
function ProbablisticRoadMap:cleanUp()
	local visited = {}

  -- To keep track of largest component found thus far
  -- a component is a group of connected nodes
  local comp         = {n_start = self.nodes:first(), size = 0}
  local comp_largest = {n_start = self.nodes:first(), size = 0}
  
  function largest(c1, c2)
    if c1.size > c2.size then return c1 else return c2 end
  end
  
	while comp.n_start do
    local i = 0
	  Graph.breathFirstSearch(comp.n_start, function(n)
	    visited[n] = true
	    comp.size = comp.size + 1
	    i = i+1
	  end)
	  
	  comp_largest = largest(comp, comp_largest)
	  
	  comp = { 
	    n_start = self.nodes:find(function(n) 
	      return not visited[n] 
	    end),
	    size = 0
	  }	  
	end
	
	-- Remove all nodes and only add nodes for
	-- largest component
	local tagnum = 1 -- NOTE: Remove later, just for debug
	self.nodes = Collection:new()
  Graph.breathFirstSearch(comp_largest.n_start, function(n)
    n.tag = tagnum -- NOTE: Debug to name nodes. remove later
    tagnum = tagnum+1
    self.nodes:append(n)
  end)
end


--[[
  Search hierarcial bounding volume consisting of Axis Aligned Bounding boxes where the
  leafes contain circles, to find circle center closest to query point.
 
  In a hierarcical bounding volume a search for boxes containing point will give us all
  circles which potentially have center closest to point. Because centers which dont have
  their circles containing query point can possibly be reachable from position either.
]]--
function ProbablisticRoadMap:findNode(pos)
  local shortest_dist = math.huge
  local shape_closest  = nil
  self.circleGroup:inside(pos, Engine.seconds(), 1, function(shape, t, dt)
    local dist = (shape:center() - pos):squaredLength()
    if dist < shortest_dist then
      shortest_dist = dist
      shape_closest = shape
    end
  end)
  
  return self.circleNodeMap[shape_closest]
end

 --[[
 Returns a new point, which is on the voronoi lines/edges between the obstacles.
 It is gotten by moving our sample point c, until we hit the voronoi line running
 between the obstacles.
 
 Algorithms work by using distance from sample point to nearest obstacle
 as step size and then move one step at a time in opposite direction from
 sample point c, until nearest obstacle to c1 is different from nearest obstacle
 to c.
 
 When other obstacle is found we use equidistantVertex to find the point between
 the two points with different closest obstacle to find the point right in the middle
 between the two obstacles.
 --]] 
function ProbablisticRoadMap:slowRetractSample(c)
	local c_close = self:nearestObstacle(c)

	-- init Step vector ds (stepsize may not be larget than 0.5f * maxdist)
  local ds = c - c_close
  
  if ds:squaredLength() * 2 > MAX_DIST_SQUARED then
    ds = ds:unit()*0.5*MAX_DIST
  end
  
  -- init second point to find second nearest obstacle
	local c1 = c + ds

	-- while the nearest obstacle to c' (c_near) is approx. equal to c_close, keep searching
	local c1_found = true
	local c_near = self:nearestObstacle(c1)
	while (c_near-c_close):squaredLength() < ACCURACY do
		c1 = c1 + ds;
		c_near = self:nearestObstacle(c1)

		-- break when c' has moved too far
		if (c1 - c_close):squaredLength() > MAX_DIST_SQUARED or not self.bbox:inside(c1) then
		  c1_found = false
		  break
		end
	end

	 -- search for equidistant vertex
	if c1_found then 
	  c_v = self:equidistantVertex(c, c1)							
	else  		  
	  c_v = c1 - ds
	end

	return c_v
end

function ProbablisticRoadMap:retractSample(c)
  return Engine.retractSample(self.obstacles, self.bbox, c)
end

--[[
 Performs a binary search between c1 and c2 to find the point 
 where the two nearest obstacles are equidistant.
 
 The algorithm works alternating between moving a point c_v
 towards and away from c1. At each alternation the step taken
 is smaller so that one eventually comes very close to the equividistant vertex.
 Due to numerical inaccuracies one will never get there exactly
 to it is approximated by saying we reached it when the steps
 taken are below a certain minimum length.
]]--
function ProbablisticRoadMap:slowEquidistantVertex(c1, c2)
  -- init step vector
  local ds = (c2-c1)*0.5
  
	-- init first test position and both nearest obstacles
	local c_v = c1 + ds
	local c1_obst = self:nearestObstacle(c1)
	local c2_obst = self:nearestObstacle(c_v)

	while ds:squaredLength() > ACCURACY do
		ds = ds*0.5

		-- update test position
		if (c2_obst - c1_obst):squaredLength() < ACCURACY then
			c_v = c_v + ds  -- c1 and c_v probably had the same obstacle closest
		else
			c_v = c_v - ds  -- They had different closest obstacles, so move c_v close to c1
    end  

		-- update nearest obstacle to test position
		c2_obst = self:nearestObstacle(c_v)
	end

	return c_v
end

function ProbablisticRoadMap:equidistantVertex(c1, c2)
  return Engine.equidistantVertex(self.obstacles, self.bbox, c1, c2)
end

--[[
  Use C++ code directly to do finding of nearest obstacle as opposed
  to using Lua. as in slow implementation. This is about 7 times faster
]]--
function ProbablisticRoadMap:nearestObstacle(c)
  return Engine.nearestObstacle(self.obstacles, self.bbox, c)
end

--[[
 returns the point (on an obstacle) that is nearest to pos
 if none is found nil is returned.
]]--
function ProbablisticRoadMap:slowNearestObstacle(c)
-- function ProbablisticRoadMap:nearestObstacle(c)
	local is_collision = false
	local radius
	
	local view = Engine.view()
  if view:width() < view:height() then 
    radius = view:width() 
  else 
    radius = view:height() 
  end
	local stepsize = radius
    
  local point_result = nil
  
	while stepsize > ACCURACY and radius > ACCURACY do
    local point = self:discCollision(c, radius)
    if point then
      point_result = point
      is_collision = true
      stepsize = stepsize*0.5
      radius = radius-stepsize      
    else
      if is_collision then 
        stepsize = stepsize*0.5 
      else 
        stepsize = stepsize*2
      end
      radius = radius+stepsize
    end
  end
  
  return point_result
end


 --[[
  Finds the largest circle surroudning point c. This is done by performing a binary
  search. We start with a circle covering the whole scene. If there is a collision with this
  scene we check for collision with a circle of half the radius. At any given time
  in search we have maximum 3 types of areas: 
    - An area we know if collision free
    - An area we know there is collision in
    - An area we don't know whether is collision free or not. 
  We always split the unknown area in two trying to deterimine which half is
  collision free, which one is not or which one is unknown.
 ]]--
function ProbablisticRoadMap:largestFreeDisc(c)
  local is_collision = false
  local radius
  local view = Engine.view()
  if view:width() < view:height() then 
    radius = view:width() 
  else 
    radius = view:height() 
  end
  local stepsize = radius
	while stepsize > ACCURACY and radius > ACCURACY do
		if self:discCollision(c, radius) then
      is_collision = true
			stepsize = stepsize*0.5
			radius = radius - stepsize
		else
		  if is_collision then 
		    stepsize = stepsize*0.5 
		  else 
		    stepsize = stepsize*2 
		  end
			radius = radius + stepsize
    end
	end
	return radius
end

--[[
 Checks whether the node with position c is inside any of the discs in the roadmap
]]--
function ProbablisticRoadMap:insideRoadMap(c)
  local is_inside = false
  for _,n in pairs(self.nodes) do
    if n.circle:inside(c) then return true end
  end
  return false
end


 -- 
 -- ResizeEdge
 -- 
 -- Resizes the edge (recursively) if it is larger than the maximum edge size
 -- 
function ProbablisticRoadMap:resizeEdge(n1, n2)
	local e = n2.pos - n1.pos

	if e:length() > MAX_EDGE_SIZE then
    -- create node in middle of edge
		local n = PrmNode:newNode(n1.pos+e*0.5)

		-- split edge
    self:addNeighbors(n)
    n:addNeighbors(n1, n2)
			
    -- update previous edges
		n1:removeNeighbors(n2)
		n1:addNeighbors(n)
		n2:removeNeighbors(n1)
		n2:addNeighbors(n)

		-- retract middle node to voronoi diagram
    -- ??
    --  No, dont. Retracting can cause the node to move to far 
    --  and in fact cause the resizing to go on forever
    --  Nodes[n].Pos = RetractNode(VM, &Nodes[n].Pos);
    -- ??

    -- update radius  
		n.radius = self:largestFreeDisc(n.pos)

		-- call resize edge for each new edge
		self:resizeEdge(n, n1)
		self:resizeEdge(n, n2)
	end
end

--[[
 Checks whether a line from node p1 to p2 (cylinder with a small radius) has collision.   
]]-- 
function ProbablisticRoadMap:lineCollision(n1, n2)
  local seg = Shape:newSegment(n1:position(), n2:position())
  local is_col = self.obstacles:collide(seg)
  seg:kill()
  return is_col
end

--[[
  Returnes the first point on obstacle which is intersected with circle
  at location 'c' and with radius 'radius'
]]--
function ProbablisticRoadMap:discCollision(c, radius)
  local circle = Shape:newCircle(c, radius)
  local intersection_point = nil
  self.obstacles:collide(circle, Engine.seconds(), 1, function(me, other, points, t, dt)
      if points then intersection_point = points[1] end
      return true
    end
  )
  circle:kill()
  return intersection_point
end

--[[
  Creates a GroupShape out of the corridor defined by the path, and returns it

  
  \param path is a list were first element is start node and last is target node
  Each node is a PrmNode which a circle member.
  \return shapegroup, mapping from circle to node giving previous and next circle
]]--
function Graph.createCorridorShape(path)
  local circle_path = Map:new()
  local circle_prev = nil
  
  -- Have to go through a few hoops because ShapeGroup
  -- can only take Group as input  
  local circles = Group:new()
  for i,node in ipairs(path) do
    circles:add(node.circle)
    circle_path[node.circle] = {prev = path[i-1], next = path[i+1]}
  end
  return ShapeGroup:new(circles), circle_path
end

--[[
  Search a roadmap for choke points on the way from e.g. an NPC
  or player, to key locations on the map. Key locations would be 
  places the NPC needs to reach for different objectives. E.g.
  get powerups, exit points etc.
  
  \param n_start node to start search from
  \param key_nodes collection of important nodes in roadmap
]]--
function Graph.findChokePoints(n_start, key_nodes)
  local _, path = Graph.dijkstra(n_start)
  local chokepoints = Collection:new()
  local r_max  = 1 -- nodes with radius below this is considered part of narrow passage
  
  -- Checks if n is a valid choke node and adds it if it is
  function addChokePoint(n)
    if (n:radius() < r_max) ~= (path[n]:radius() < r_max) then
      local n_choke, n_tube -- node containing chokepoint, and node inside narrow passage
      if n:radius() < r_max then 
        n_choke, n_tube = path[n], n
      else
        n_choke, n_tube = n, path[n]
      end      
      -- Adds a pair describing location of chokepoint and direction of
      -- path into chokepoint from narrow passage
      chokepoints:append( {n_choke, (n_choke - n_tube):unit()} )
    end
  end
  
  for _, n in pairs(key_nodes) do
    while path[n] ~= n do
      addChokePoint(n)
    end
    addChokePoint(n)
  end
  return chokepoints
end