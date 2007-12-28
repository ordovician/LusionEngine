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

function GraphNode:insertNeighbors(...)
  self.mNeighbors:insert(unpack(arg))
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
    print("remove", v.tag)
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

function Graph.testDijkstra()
  print("Testing Dijkstra:")
  local a, b, c, d, e, f, g, h, i, j = Graph.defaults()
  local n = g
  local distance, path = Graph.dijkstra(a, n)
  print("distance", distance[n])
  print("node ", n.tag)
  while n ~= nil and path[n] ~= n do
    n = path[n]
    print("node ", n.tag)
  end
end

function Graph.testAStar()
  print("Testing A*:")
  local a, b, c, d, e, f, g, h, i, j = Graph.defaults()
  local n = g
  local distance, path = Graph.astar(a, n)
  print("distance", distance[n])
  print("node ", n.tag)
  while n ~= nil and path[n] ~= n do
    n = path[n]
    print("node ", n.tag)
  end
end

TagNode = GraphNode:new()

function TagNode:newNode(tag, pos)
  local obj = TagNode:new()
  obj.tag = tag
  obj.pos = pos
  return obj 
end

function TagNode:distanceTo(v)
  return (self.pos-v.pos):length()
end

--[[
  Returns a graph to use for testing.
  
  This collection is made so that a->c->g should be the shortest
  path to g. c is a the center, a and b at the bottom ang g and f at
  the top.
]]--
function Graph.defaults()
  function node(tag, pos)
    return TagNode:newNode(tag, pos)
  end

  local a = node("a", vec(4,2))
  local b = node("b", vec(2,4))
  local c = node("c", vec(5,5))
  local d = node("d", vec(7,4))
  local e = node("e", vec(2,9))
  local f = node("f", vec(7,8))
  local g = node("g", vec(6,10))  
  local h = node("h", vec(2,-20))
  local i = node("i", vec(5,3))
  local j = node("j", vec(5,4))  

  a:insertNeighbors(b,c,h)
  c:insertNeighbors(e,d,g,f,j)
  d:insertNeighbors(f)
  e:insertNeighbors(g)
  f:insertNeighbors(g)
  h:insertNeighbors(i)
  j:insertNeighbors(i)
  
  return a, b, c, d, e, f, g, h, i, j
end

function Graph.default()
  local a, b, c, d, e, f, g, h, i, j = Graph.defaults()
  return a
end
