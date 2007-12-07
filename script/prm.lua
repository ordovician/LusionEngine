require("script/algorithm")

--[[
	PrmNode class
	-------------------------
	
	created: 05/09/2007
	author: Erik Engheim
	
  Implements a node in a probablistic roadmap
	
--]]
PrmNode = {}

-- Creation
function PrmNode:new(pos, radius)
  -- Create instance
  local instance = {}
  setmetatable(instance, self)
  self.__index = self
  
  instance:init(pos, radius)
  return instance
end

function PrmNode:init(pos, radius)
  -- self.pos = pos
  -- self.radius = radius
  self.circle = Shape:newCircle(pos, radius)
  self.edges = {}
end

function PrmNode:toString()
  return "pos "..self.circle:center().." radius "..self.circle:radius()
end

--[[
	ProbablisticRoadMap class
	-------------------------
	
	created: 05/09/2007
	author: Erik Engheim
	
  Implements a probablistic roadmap
	
--]]

 -- defines
MAX_PATHDIST	= 1000000000.0		-- maximum path distance 
MAX_DIST			=	20000.0			-- maximum distance a sample can move in search for nearest obstacle  
MAX_DIST_SQUARED	=	MAX_DIST * MAX_DIST	-- maximum distance squared
MAX_EDGE_SIZE	=		100000.0			 -- maximum edge size
MAX_RADIUS		=	50000.0			-- maximum sphere radius
MIN_RADIUS		=	500.0			-- minimum sphere radius
ACCURACY		=	0.1				 -- a measure for the accuracy during roadmap construction

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
function ProbablisticRoadMap:construct(samples, retract_quotient)
	local halftime = samples * (1 - retract_quotient)
	
	while  samples > 0 do

		-- get sample C and retract to voronoi plane
		local c = self:pickSample()

		if samples > halftime then
			c = self:retractSample(c)
		end

		-- check whether sample is outside all discs in the roadmap
		if not self:inside(c) and self.bbox:inside(c) then

      --  add node to roadmap
      local radius = self:largestFreeDisc(c)
      if radius > MIN_RADIUS then      
        radius = math.min(radius, MAX_RADIUS)
        local node = PrmNode:new(c, radius)
        self.nodes[node] = node
        print("Added node: ", node:toString())
      end
    end
		samples = samples-1
	end

  print("Number of nodes after sampling:", self.nodes:size())

  -- add edges between nodes with overlapping spheres
  for _,n in pairs(self.nodes) do
    for _,m in pairs(self.nodes) do
      -- check whether spheres are overlapping and the nodes are not connected by an edge yet
		  if n.circle:collide(m.circle) and self:checkNodesForEdge(n, m) then
		    -- To prevent edges through obstacles perform an additional collision check
				if not self:lineCollision(n, m) then
				  n.edges[m] = m
				  m.edges[n] = n
				end
		  end
    end
  end
  
  -- removes unconnected nodes
	self:cleanUp()

	return true;  
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
  local comp_largest = {n_start = n_start, size = 0}
  
  function largest(c1, c2)
    if c1.size > c2.size then return c1 else return c2 end
  end
  
	while comp.n_start do
	  Graph.breathFirstSearch(comp.n_start, function(n)
	    visited[n] = true
	    comp.size = comp.size + 1
	  end)
	  
	  comp_largest = largest(comp, comp_largest)
	  
	  comp = { 
	    n_start = self.nodes:find(function(n) 
	      not visited[n] 
	    end),
	    size = 0
	  }	  
	end
	
	-- Remove all nodes and only add nodes for
	-- largest component
	self.nodes = Collection:new()
  Graph.breathFirstSearch(comp_largest.n_start, function(n)
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
function ProbablisticRoadMap:findDisc(pos)
  local closestShape = nil
  function locateClosestDisc(shape, other, t, dt)
    if not closestShape or shape:
  end
  
	self.nodes:inside(pos, locateClosestDisc)
	
	int ret = -1;
	Vector3 v;

  -- find node containing pos
  local quad_node = self.quadRoot:findNode(pos)
  if not quad_node then return false;
  
	for _,n in pairs(quad_node:spheres()) do
	  if (n.pos-pos):length() <= n.radius then return n end
	end
end

 -- 
 -- UpdateSphere
 -- 
 -- Uses the previous nearest sphere to find the new nearest
 --
function ProbablisticRoadMap:updateSphere(pos, ) 

end
int RoadMap::UpdateSphere(lua_State *VM, const Vector3 *pos, const int prevnearest, const int *path, const int pcount ) const
{
	int ret = -1;

	if(prevnearest >= 0)
	{
		// check whether the position is inside the previous sphere
		float len = (float)((Vector3)((Vector3)(*pos) - Nodes[prevnearest].Pos)).GetLengthSquared();
		if(len <= Nodes[prevnearest].Radius * Nodes[prevnearest].Radius)
		{
			ret = prevnearest;
		}
		else
		{
			// not inside previous sphere, check neighboring spheres:
			int size = (int)Nodes[prevnearest].Edges.size();

			for( int i=0; i<size; ++i )
			{
				len = (float)((Vector3)((Vector3)(*pos) - Nodes[Nodes[prevnearest].Edges[i]].Pos)).GetLengthSquared();
				if(len <= Nodes[Nodes[prevnearest].Edges[i]].Radius * Nodes[Nodes[prevnearest].Edges[i]].Radius)
				{
					// inside this sphere
					ret = Nodes[prevnearest].Edges[i];

					// if no roadmap path: any sphere will do
					if( path == NULL )				
						 break;

					// if inside roadmap path: return this sphere
  				    if( path )
					{
						for( int j=0; j<pcount; ++j )
							if( path[j] == ret )
								return ret;
					}
				}
			}
		}
	}
	else
	{
		// find whether (and in which sphere) the position is in the roadmap
		ret = FindSphere(VM, pos);
	}

	return ret;
}

--[[
  Picks a random sample. The sample is a point within the boundaries
  of the world. 
  
  Sampling is done with random function. This can be changed
  to use regular grid for sampling, jitter etc instead.
]]--
function ProbablisticRoadMap:pickSample()
  local pos = vec(0,0)
  pos.x = math.random(self.bbox.min.x, self.bbox.max.x)
  pox.y = math.random(self.bbox.min.y, self.bbox.max.y)
  
  return pos
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
function ProbablisticRoadMap::retractSample(c)
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
	while (c_near-c_close):squareLength() < ACCURACY do
		c1 = c1 + ds;
		c_near = self:nearestObstacle(c1)

		-- break when c' has moved too far
		if (c1 - c_close):squareLength() > MAX_DIST_SQUARED or not self.bbox:inside(c1) then
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
function ProbablisticRoadMap:equidistantVertex(c1, c2)
  -- init step vector
  local ds = (c2-c1)*0.5
  
	-- init first test position and both nearest obstacles
	local c_v = c1 + ds
	local c1_obst = self:nearestObstacle(c1)
	local c2_obst = self:nearestObstacle(c_v)

	while ds:squareLength() > ACCURACY do
		ds = ds*0.5

		-- update test position
		if (c2_obst - c1_obst):squareLength() < ACCURACY )
			c_v = c_v + ds  -- c1 and c_v probably had the same obstacle closest
		else
			c_v = c_v - ds  -- They had different closest obstacles, so move c_v close to c1
    end  

		-- update nearest obstacle to test position
		c2_obst = self:nearestObstacle(c_v)
	end

	return c_v
end

 --[[
 returns the point (on an obstacle) that is nearest to pos
 if none is found nil is returned.
]]--
function ProbablisticRoadMap:nearestObstacle(c)
	local is_collision = false
	local radius
	
	local view = Engine.view()
  if view:width() < view:height() then radius = view:width() else radius = view:height() end
	local stepsize = radius

  
	while stepsize > ACCURACY and radius > ACCURACY do
    local point = self:discCollision(c, radius)
    if point then
      is_collision = true
      stepsize = stepsize*0.5
      radius = radius-stepsize      
    else
      if is_collision then stepsize = stepsize*0.5 else stepsize = stepsize*2
      radius = radius+stepsize
    end
  end
  
  return point
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
  if view:width() < view:heigth() then radius = view:width() else radius = view:height() end
  local stepsize = radius

	while stepsize > ACCURACY and radius > ACCURACY do
		if self:discCollision(c, radius) then
      is_collision = true
			stepsize = stepsize*0.5
			radius = radius - stepsize
		else
		  if is_collision then stepsize = stepsize*0.5 else stepsize = stepsize*2
			radius = radius + stepsize
    end
	end

	return radius
end

 -- 
 -- InsideRoadMap
 -- 
 -- Checks whether the node with position C is inside any of the spheres in the roadmap
 -- 
function ProbablisticRoadMap:insideRoadMap(c)
  local is_inside = false
  for _,n in pairs(self.nodes) do
    if (n.pos - c):squareLength() <= n.radius*n.radius
      is_inside = true
      break
    end
  end
  return is_inside
end


 -- 
 -- ResizeEdge
 -- 
 -- Resizes the edge (recursively) if it is larger than the maximum edge size
 -- 
function RoadMap:resizeEdge(n1, n2)
	local e = n2.pos - n1.pos

	if e:length() > MAX_EDGE_SIZE then
    -- create node in middle of edge
		local n = PrmNode:new(n1.pos+e*0.5)

		-- split edge
		self.nodes[n] = n
		n.edges[n1] = n1
		n.edges[n2] = n2
			
    -- update previous edges
		n1.edges[n2] = nil
		n1.edges[n] = n
		n2.edges[n1] = nil
		n2.edges[n] = n

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
  Checks whether two nodes are connected by an edge
]]--
function ProbablisticRoadMap:checkNodesForEdge(n1, n2)
  return n1.edges[n2] == n2
end

 -- 
 -- lineCollision(p1, p2 )
 -- 
 -- Checks whether a line from p1 to p2 (cylinder with a small radius) has collision. 
 -- 
function ProbablisticRoadMap:lineCollision(p1, p2)
  local seg = Shape:newSegment(p1, p2)
  return self.obstacles:collide(seg)
end

--[[
  Returnes the first point on obstacle which is intersected with circle
  at location 'c' and with radius 'radius'
]]--
function ProbablisticRoadMap:discCollision(c, radius)
  local circle = Shape:newCircle(c, radius)
  local intersection_point = nil
  self.obstacles:collide(circle, Engine.seconds(), 1, function(me, other, points, t, dt)
      if points then intersection_point = points[1]
      return true
    end
  )
  return intersection_point
end