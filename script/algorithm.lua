require("script/queue")
require("script/stack")

--[[
	Graph class
	-------------------------
	
	created: 6/12/2007
	author: Erik Engheim
	
	Collection of function for performing operations
	on graphs
--]]

Graph = {}

-- Sample edge iterator
function Graph.edges(node)
  if not node.edges then
    return function() end
  end
  
  local i = 0
  local n = #node.edges
  return function()
    i = i+1
    if i <= n then return node.edges[i] end
  end
end

--[[
  Performs a breath first search
  
  \param n_start start node in graph for breath first search
  
  \param edges an iterator factory function which lets us iterate over elements 
  connected to a node. \a edges should be on the form edges(n), which 
  returns a function that will return the next neighbor of node n
  
  \param action a function which takes one argument which is node in graph
]]--
function Graph.breathFirstSearch(n_start, action, edges)
  local visited = {}
  
  if not edges then
    edges = Graph.edges
  end
    
  visited[n_start] = true
	local queue = Queue:new(n_start)

	while not queue:empty() do
		local n = queue:pop()
		action(n) 
		
		for m in edges(n) do
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
function Graph.depthFirstSearch(n_start, action, edges)
  local visited = {}

  if not edges then
    edges = Graph.edges
  end
  
  visited[n_start] = true
	local stack = Stack:new(n_start)
	
	while not stack:empty() do
		local n = stack:pop()		 
	  action(n) 
			
		for m in edges(n) do
		  if not visited[m] then
        visited[m] = true
			  stack:push(m)
		  end
		end
	end
end