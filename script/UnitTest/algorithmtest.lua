require("script/algorithm")

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

-- Short hand to create nodes
function node(tag, pos)
  return TagNode:newNode(tag, pos)
end

--[[
  Returns a graph to use for testing.
  
  This collection is made so that a->c->g should be the shortest
  path to g. c is a the center, a and b at the bottom ang g and f at
  the top.
]]--
function Graph.defaults()
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

AlgoTest = {}

function AlgoTest.breathFirstTestAcyclic()    
  local a = node("A")
  local b = node("B")
  local c = node("C")
  local d = node("D")
  local e = node("E")
  
  a:insertNeighbors(d, e) 
  e:insertNeighbors(c, b) 
  
  print("Expect: A, [D, E], [C, B]. NOTE: [] sequence not important here")
  Graph.breathFirstSearch(a, 
  function(n)
    print(" Node: ", n.tag)
  end)
    
  print("End Seach")
end

function AlgoTest.breathFirstTest()
  local a = node("A")
  local b = node("B")
  local c = node("C")
  local d = node("D")
  local e = node("E")
  
  a:insertNeighbors(d, e) 
  e:insertNeighbors(c, b)
  b:insertNeighbors(d, c)
  
  print("Expect: B, [C, D] NOTE: [] sequence not important here")  
  Graph.breathFirstSearch(b, function(n)
    print(" Node: ", n.tag)
  end)  
  print("End Seach")
end

function AlgoTest.depthFirstTest()
  local a = node("A")
  local b = node("B")
  local c = node("C")
  local d = node("D")
  local e = node("E")
  
  b:insertNeighbors(d, c) 
  a:insertNeighbors(d, e)
  e:insertNeighbors(c, b)
      
  Graph.depthFirstSearch(b, function(n)
    print(" Node: ", n.tag)
  end)  
  print("End Seach")

end

function AlgoTest.testDijkstra()
  local a, b, c, d, e, f, g, h, i, j = Graph.defaults()
  local n = g
  local distance, path = Graph.dijkstra(a, n)
  print(" Expect: g, c, a")
  print(" distance", distance[n])
  print(" node ", n.tag)
  while n ~= nil and path[n] ~= n do
    n = path[n]
    print(" node ", n.tag)
  end
end

function AlgoTest.testAStar()
  local a, b, c, d, e, f, g, h, i, j = Graph.defaults()
  local n = g
  local distance, path = Graph.astar(a, n)
  print(" Expect: g, c, a")
  print(" distance", distance[n])
  print(" node ", n.tag)
  while n ~= nil and path[n] ~= n do
    n = path[n]
    print(" node ", n.tag)
  end
end

return AlgoTest