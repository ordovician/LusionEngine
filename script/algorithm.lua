require("script/queue")
require("script/stack")
require("script/set")
require("script/geometry")

--[[
	GraphNode class
	-------------------------
	
	created: 25/12/2007
	author: Erik Engheim
	
	All the graph node algorithms expect a Node with
	the interface defined by this class. Meaning it should
	be possible to insert neighboring nodes, check if a node
	has an edge to another given node (hasNeighbor) and
	return a collection of neighbors which it should be possible
	to iterate over. No assumptions is made about the returned neighbors
	collection. The neighbors might be ordered in a sequence of be
	in a set. So Clients should not make assumptions about the key.
--]]

GraphNode = {}

-- Creation
function GraphNode:new()
  -- Create instance
  local obj = {}
  setmetatable(obj, self)
  self.__index = self

  obj.mNeighbors = Set:new() 
  return obj
end

function GraphNode:neighbors()
  return self.mNeighbors
end

function GraphNode:neighborSet()
  return self.mNeighbors
end


function GraphNode:insertNeighbors(...)
  self.mNeighbors:insert(...)
end

--[[
  True if there is an edge from this node to node \a n
]]--
function GraphNode:hasNeighbor(n)
  return self.mNeighbors:contains(n)
end

--[[
  Default is a graph with unweighted edges. Meaning each edge has
  length 1. This function should be overriden for graphs with
  weighted edges.
]]--
function GraphNode:distanceTo(n)
  return 1
end

--[[
	Graph class
	-------------------------
	
	created: 6/12/2007
	author: Erik Engheim
	
	Collection of function for performing operations
	on graphs
--]]

Graph = {}

--[[
  Performs a breath first search
  
  \param n_start start node in graph for breath first search  
  \param action a function which takes one argument which is node in graph
]]--
function Graph.breathFirstSearch(n_start, action)
  local visited = {}
      
  visited[n_start] = true
	local queue = Queue:new(n_start)

	while not queue:empty() do
		local n = queue:pop()
		action(n) 
		
		for _,m in pairs(n:neighbors()) do
		  if not visited[m] then
        visited[m] = true
			  queue:push(m)
		  end
		end
	end
end

--[[
  Performs a depth first search starting from node
  \a n_start and perform action \a action on each
  visited node.
]]--
function Graph.depthFirstSearch(n_start, action)
  local visited = {}
  
  visited[n_start] = true
	local stack = Stack:new(n_start)
	
	while not stack:empty() do
		local n = stack:pop()		 
	  action(n) 
			
		for _,m in pairs(n:neighbors()) do
		  if not visited[m] then
        visited[m] = true
			  stack:push(m)
		  end
		end
	end
end

--[[
  Finds shortest paths from node \a n_start to all other nodes in graph
  
  \param n_start start node of search
  \param length function where length(n, m) denotes length between
   node m and n
   connected to a node. \a edges should be on the form edges(n), which 
   returns a function that will return the next neighbor of node n
]]--
function Graph.dijkstra(n_start)
  local candidates = Collection:new(n_start) -- Nodes which will be visisted
  local distance = {} -- distance[n] is best estimate of distance from node n to n_start
  local visited  = {} -- visisted[v] == true means v has been visisted in graph traversal
  local path     = Collection:new()
  
  distance[n_start] = 0
  path[n_start] = n_start
  visited[n_start] = true
    
  -- Greedy loop
  while not candidates:empty() do
    -- Select closest neighbor, mark as visisted
    local v = candidates:removeMin(function(v, w) 
      return distance[v] < distance[w] 
    end)
    --print("remove", v.tag)
    for _,w in pairs(v:neighbors()) do
      if not visited[w] then 
        candidates:append(w) 
        distance[w] = distance[v] + v:distanceTo(w)
        path[w]     = v        
        visited[w]  = true
      end
    end
    
    -- Relax edges
    for _,w in pairs(candidates) do
      if v:hasNeighbor(w) then
        local d = distance[v] + v:distanceTo(w)
        --print("edge", v.tag, w.tag, distance[w], d)        
        if  d < distance[w] then
          distance[w] = d
          path[w] = v
        end
      end
    end
  end
  
  return distance, path
end

--[[
  Perform an A* search starting from node \a n_start

  \param n_start start node of search
  \param length function where length(n, m) denotes length between
   node m and n
  \param edges an iterator factory function which lets us iterate over elements 
   connected to a node. \a edges should be on the form edges(n), which 
   returns a function that will return the next neighbor of node n
]]--
function Graph.astar(n_start, n_goal)
  local candidates = Collection:new(n_start) -- Nodes which will be visisted
  local distance = {} -- distance[n] is best estimate of distance from node n to n_start
  local visited  = {} -- visisted[v] == true means v has been visisted in graph traversal
  local path     = Collection:new()

  distance[n_start] = 0
  path[n_start] = n_start
  visited[n_start] = true
  
  -- Greedy loop
  while not candidates:empty() do
    -- Select closest neighbor, mark as visisted
    local v = candidates:removeMin(function(v, w) 
      return distance[v]+v:distanceTo(n_goal) < distance[w]+w:distanceTo(n_goal)
    end)
    
    print("remove", v.tag)
    if v == n_goal then 
      break 
    end
    
    for _,w in pairs(v:neighbors()) do
      if not visited[w] then 
        candidates:append(w) 
        distance[w] = distance[v] + v:distanceTo(w)
        path[w]     = v        
        visited[w]  = true
      end
    end
    -- Relax edges
    for _,w in pairs(candidates) do
      if v:hasNeighbor(w) then
        local d = distance[v] + v:distanceTo(w)
        print("edge", v.tag, w.tag, distance[w], d)
        if  d < distance[w] then
          distance[w] = d
          path[w] = v
        end
      end
    end
  end
  return distance, path
end

--[[ 
  A factory method that creates and returns a seek steering behavior
  function that seeks towards the node at the end of \a path.
  
  Differs from regular seek in that it tries to seek within corridormap
  defined by path. Path nodes must be of type PrmNode, since a circle member
  is expected. 
]]--
function Geometry.makePathSeek(n_start, n_target)
  local distance, paths = Graph.astar(n_start, n_target)
  local path = Graph.createPath(n_start, n_target, paths)
  local corridor, circle_path = Graph.createCorridorShape(path)
  
  -- Goodness of trajectory from state 's0' to 's1'
  function seek(s0, s1)
    local p0 = s0:position()
    local p1 = s1:position()
    local d1 = s1:direction()
    local c_1 = corridor[p1]
    local target = circle_path[c_1].next:position()
    
    local dir_target = (target-p1):unit()  -- direction to target
    local dir_path = (p1-p0):unit()   -- direction of from current point to next on path
    return 0.25*(1 + d1*dir_target)*(1 + dir_path * dir_target)
  end
  return seek
end

--[[
  A* and Dijkstra algorithms will return paths as a hashtable were
  each node is a key into the table to the node that precedes it on the path
  to target. So the graph is actually directed from target to multiple sources.
  
  This function translates this structure into an array of all the nodes along on
  path going from node \a n_beggin to node \a n_end.
]]--
function Graph.createPath(n_begin, n_end, paths)
  local path = Collection:new()
  local n = n_end
  while n ~= nil and paths[n] ~= n_begin do
    path:prepend(n)
    n = paths[n]
  end
  path:prepend(n_begin)
  return path
end

--[[
  Attemps to find a path from node \a n_start to \a node n_target in a graph
  which is as long as possible but shorter than \a distance.
  
  This is used to time it so that an NPC reaches its enemy around the
  time when the enemy passes a choke point. If it can't reach right on time
  then it should reach before that time. As reaching later would no enable
  NPC to make any flank attacks whatsoever.
]]--
function Graph.findPathWithDistance(d_desired, n_start, n_target)
  assert(n_start)
  assert(n_target)
  
  local path_result = Polygon:new(n_start)  
  local distance, path = Graph.dijkstra(n_target)
  local d_accum = 0 -- Distance of path up to node n
  local n = n_start -- Node current visited
  function validDistance(d) return d <= d_desired end  
  
  -- NOTE: Check if actual distance and desired is almost equal
  while n ~= n_target do
    function distanceThrough(m) 
      return d_accum + n:distanceTo(m) + distance[m]
    end
    -- Finds node with edge which gives max distance lower than d_desired
    n = n:neighborSet():map(distanceThrough):filter(validDistance):maxKey()    
    path_result:append(n)    
  end
  
  return path_result
end