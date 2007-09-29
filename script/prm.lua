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
  self.pos = pos
  self.radius = radius
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
		  if (n.pos - m.pos):length() < n.radius + m.radius and self:checkNodesForEdge(n, m) then
		    -- To prevent edges through obstacles perform an additional collision check
				if not self:lineCollision(n.pos, m.pos) then
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

-- 
-- CleanUp
-- 
-- Uses BFSearches to find the largest connected component in the roadmap, and removes all other edges and nodes.
-- 
function ProbablisticRoadMap:cleanUp()
	local componentsizes = Collection:new()
	local visited = {}
	
	function visit(n)
	  visited[n] = true
	  n.componentIndex = #componentsizes+1
	end
	
	local n_start = self.nodes:first()

	-- while there are unmarked nodes in the roadmap
	while n_start do
    visit(n_start)

		local queue = Queue:new()
    local componentsize = 0		
		queue.push( n_start )

		-- do a depth first search from 'n_start' node and count up all nodes it is connected to
		while not queue:empty() do
			local n = queue:pop()
      componentsize = componentsize+1
			 
			for _,m in pairs(n.edges) do
			  if not visisted[m] then
          visit(m)
				  queue.push(m)
			  end
			end
		end
    
    componentsizes.append(componentsize)
    
    -- check whether there are nodes that haven't been visised (marked)
    -- select first "unvisited" node as next one to determine component size of
		function isNotVisited(n) return not visited[n] end
		n_start = self.nodes:find(isNotVisited)
	end

  print("Components: ", componentsizes:size())

  local component_maxindex = componentsizes:maxIndex()

	print("Max Component Size:", componentsizes[component_maxindex])
		
	-- remove all nodes and edges not belonging to the maximum connected component
	local old_nodes = self.nodes
	self.nodes = Collection:new()
	
	local no_removed = 0
	for _,n in pairs(old_nodes) do
	  if n.componentIndex == component_maxindex then 
	    self.nodes[n]
	  else
	    no_removed = no_removed+1
	  end
	end
	
  print( "Nr of unconnected nodes removed:", no_removed)

	return true
end



 -- 
 -- FindDisc
 -- 
 -- Uses brute force search to find the sphere in the roadmap closest to position: pos
 -- NOTE: Need to implement a quad tree 
function ProbablisticRoadMap:findDisc(pos)
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

 -- 
 -- PickSample
 -- 
 -- Returns a sample for roadmap construction. Uses random sampling.
 -- TODO: Implement Gaussian Sampling?
 -- TODO: Implement uniform (grid based) sampling (maybe even by using octtree)?
function ProbablisticRoadMap:pickSample()
  local pos = vec(0,0)
  pos.x = math.random(self.bbox.min.x, self.bbox.max.x)
  pox.y = math.random(self.bbox.min.y, self.bbox.max.y)
  
  return pos
end


 -- 
 -- RetractSample
 -- 
 -- Moves the node to a plane in the voronoi diagram, returns the new position
 -- 
function ProbablisticRoadMap::retractSample(c)
	local c_close = self:nearestObstacle(c)

	-- init Step vector ds (stepsize may not be larget than 0.5f * maxdist)
  local ds = c - c_close
  
  if step:squaredLength() * 2 > MAX_DIST_SQUARED then
    step = step:unit()*0.5*MAX_DIST
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

 -- 
 -- ComputeEquidistantVertex
 -- 
 -- Performs a binary search between c1 and c2 to find the point 
 -- where the two nearest obstacles are equidistant.
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
			c_v = c_v + ds
		else
			c_v = c_v - ds
    end  

		-- update nearest obstacle to test position
		c2_obst = self:nearestObstacle(c_v)
	end

	return c_v
end

 -- 
 -- nearestObstacle
 -- 
 -- returns the point (on an obstacle) that is nearest to pos
 -- 
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

-- 
--  checkNodesForEdge(n1, n2)
-- 
--  Checks whether two nodes are connected by an edge
-- 
function ProbablisticRoadMap:checkNodesForEdge(n1, n2)
  return n1.edges[n2] == n2
end

 -- 
 -- lineCollision(p1, p2 )
 -- 
 -- Checks whether a line from p1 to p2 (cylinder with a small radius) has collision. 
 -- 
function ProbablisticRoadMap:lineCollision(p1, p2)


end

--[[
  Returnes the first point on obstacle which is intersected with circle
  at location 'c' and with radius 'radius'
]]--
function ProbablisticRoadMap:discCollision(c, radius)
  -- pseudocode
  return self.obstacles:intersect(Circle:new(c, radius))
end