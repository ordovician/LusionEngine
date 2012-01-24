/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <Geometry/Graph2.hpp>

/*!
    \class Graph2 Graph2.h


*/
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/astar_search.hpp>

#include <set>
#include <algorithm>
#include <iostream>
#include <Geometry/IO.hpp>

using namespace std;
using namespace boost;

EdgePair::EdgePair() : tag(0), u(0), v(0)
{
  
}

EdgePair::EdgePair(Trapezoid2* au, Trapezoid2* av, int atag, const Point2& apos)
  : u(au), v(av), tag(atag), pos(apos)
{
  
}  

EdgeData EdgePair::first() const
{
  assert(u != 0);
  
  EdgeData e;
  e.trap = u;
  e.upos = u->center();
  e.vpos = pos;
  e.utag = u->tag();
  e.vtag = tag;
  e.weight = (e.vpos - e.upos).length();
  
  return e;
}

EdgeData EdgePair::second() const
{
  assert(v != 0);
  
  EdgeData e;
  e.trap = v;
  e.upos = pos;
  e.vpos = v->center();
  e.utag = tag;
  e.vtag = v->tag();
  e.weight = (e.vpos - e.upos).length();
  
  return e;
}

EdgeData EdgePair::edge(int index) const
{
  assert(index == 0 || index == 1);
  
  return index == 0 ? first() : second();
}

bool EdgePair::operator==(const EdgePair& data) const
{
  return tag == data.tag;
}

/*! Property data stored in graph roadmap */
struct EdgeProperty
{
  EdgeProperty() : weight(0), trapezoid(0) {}
  EdgeProperty(real aweight, Trapezoid2* atrapezoid) : weight(aweight), trapezoid(atrapezoid) {}
  real weight;
  Trapezoid2* trapezoid;
};

// Define our Graph structure to have vertex and edges
// stored in vectors. 
typedef adjacency_list < vecS, vecS, undirectedS, Point2, EdgeProperty > Graph;

typedef set<Trapezoid2*>           TrapezoidSet;

typedef Graph::vertex_descriptor  Vertex;
typedef Graph::edge_descriptor    Edge;
typedef Graph::vertex_iterator    VertexIterator;
typedef Graph::edge_iterator      EdgeIterator;
typedef Graph::out_edge_iterator  OutEdgeIterator;

typedef vector<Vertex>  Vertices;
typedef vector<Edge>    Edges;
typedef vector<real>    Reals;

// Helper functions

// So definition of Graph is know we include here
void printBeginGraph(const char* name)
{
  cout << "digraph " << name << " {" << endl;  
  cout.precision(2);
  // cout << "rankdir=LR" << endl;
  // cout << "size=\"4,3\"" << endl;
  // cout << "ratio=\"fill\"" << endl;
  // cout << "edge[style=\"bold\"]" << endl;
  // cout << "node[shape=\"circle\"]" << endl;
}

void printEndGraph()
{
  cout << "}" << endl;
}


struct VizEdge 
{
  VizEdge();
  VizEdge(int asource, int atarget, real aweight);
  VizEdge(int asource, int atarget, real aweight, const string& acolor);
  VizEdge(Edge e, const Graph& g);
  VizEdge(Edge e, const Graph& g, const Vertices& p);  
  
  void print();
    
  int     u, v;
  real    weight;
  string  color;
};

VizEdge::VizEdge() : u(0), v(0), weight(0.0), color("")
{
  
}

VizEdge::VizEdge(int asource, int atarget, real aweight) 
  : u(asource), v(atarget), weight(atarget), color("grey")
{
  
}

VizEdge::VizEdge(int asource, int atarget, real aweight, const string& acolor) 
  : u(asource), v(atarget), weight(aweight), color(acolor)
{ 
  
}

VizEdge::VizEdge(Edge e, const Graph& g)
{
  u = source(e,g);
  v = target(e, g);
  weight = g[e].weight;
  color = "grey";
}

VizEdge::VizEdge(Edge e, const Graph& g, const Vertices& p)
{
  u = source(e,g);
  v = target(e, g);
  weight = g[e].weight;
  if (p[v] == u)
    color = "black";
  else
    color = "grey";    
}


void VizEdge::print()
{
  cout << u 
       << " -> " 
       << v
       << "[label=\"" 
       << weight 
       << "\", color=\"" 
       << color 
       << "\"]" << endl;  
}

typedef vector<VizEdge> VizEdges;

Trapezoid2* trapezoid(const Segment2& s)
{
  return (Trapezoid2*)s.data();
}

static bool pathFrom(const Graph& g, Vertex t, const Vertices& p, Polygon2& path)
{
  while (t != p[t]) {
    path.push_back(g[t]);
    t = p[t];
  }
  path.push_back(g[t]);  
  return true;
}

/*! Container for paths returned from dijkstra */
class PathsImp2 : public Paths2
{
public:
  PathsImp2() : iG(0) {}
  PathsImp2(size_t noVertices) : iG(0), iDistances(noVertices), iPredecessors(noVertices) {}

  bool pathFrom(Vertex t, Polygon2& path) const {
    assert(iG != 0);
    return ::pathFrom(*iG, t, iPredecessors, path);
  }
    
  bool pathFrom(Trapezoid2* target, Polygon2& path) const {
    assert(target != 0);
    assert(iG != 0);
    
    Vertex t = vertex(target->tag(), *iG);
    return pathFrom(t, path);
  }
  
  real distanceFrom(Vertex t) const {
    return iDistances[t];    
  }
  
  real distanceFrom(Trapezoid2* target) const {
    assert(target != 0);
    assert(iG != 0);
    Vertex t = vertex(target->tag(), *iG);
        
    return distanceFrom(t);
  }
  
  void  printGraph() const {
    printBeginGraph("shortestfrom");
    EdgeIterator ei, ee;
    Graph& g = *iG;
    for (tie(ei, ee) = edges(g); ei != ee; ++ei) {      
      VizEdge ge(*ei, g);
      ge.print();
    }
    printEndGraph();            
  }
  
  void printPathFrom(Trapezoid2* goal) const {
    assert(iG != 0);
    Graph& g = *iG;
    Vertex t = vertex(goal->tag(), g);

    set<Vertex> ps;
    while (t != iPredecessors[t]) {
      ps.insert(t);
      t = iPredecessors[t];
    }
    ps.insert(t);
    
    
    printBeginGraph("shortestfrom");
    
    EdgeIterator eb, ei, ee;

    tie(eb, ee) = edges(g);
    for (ei = eb; ei != ee; ++ei) {      
      Edge e = *ei;
      VizEdge ge(e, g);
      
      // If both source and target vertex is in the set of all vertices in shortest path
      // then the current edge must be part of the shortest path, and should be colored black
      if (ps.find(source(e,g)) != ps.end() && ps.find(target(e, g)) != ps.end())
        ge.color = "black";
      ge.print();
    }
    
    printEndGraph();            
  }

  Graph*    iG;
  Reals     iDistances;
  Vertices  iPredecessors;  
};

class GraphImp2 : public Graph2
{
public:
  // Constructors
  GraphImp2( size_t noVerticies, EdgePairs::iterator begin, EdgePairs::iterator end);
  virtual ~GraphImp2();

  // Accessors

  // Request

  // Operations

  // Calculations
  Paths2* shortestPaths(Trapezoid2* source) const;
  bool    shortestPath(Trapezoid2* source, Trapezoid2* target, Polygon2& path) const;
  bool    fixedLengthPath(Trapezoid2* source, Trapezoid2* target, real distance, Polygon2& path) const;
  bool    chokePoints(Trapezoid2* start_trap, const Trapezoids2& important_loc, ChokePoints& chokepoints) const;
  
  void    printGraph() const;
  
private:
  Graph *iGraph;
};

GraphImp2::GraphImp2( size_t noVerticies, EdgePairs::iterator begin, EdgePairs::iterator end)
{
  iGraph = new Graph(noVerticies);

  Graph& g = *iGraph;
  
  // Add edges to the graph and set the vertex property to the position of the vertex
  EdgePairs::iterator i;
  for (i = begin; i != end; ++i) {
    for (int j=0; j<2; ++j) {
      EdgeData d = i->edge(j);
      Vertex u = vertex(d.utag, g);
      Vertex v = vertex(d.vtag, g);

      bool found;
      Edge e;
      tie(e, found) = add_edge( u, v, EdgeProperty(d.weight, d.trap), g);
      g[u] = d.upos;
      g[v] = d.vpos;          
    }
  }  
}

GraphImp2::~GraphImp2()
{
  delete iGraph;
}

Graph2* Graph2::create( size_t noVerticies, EdgePairs::iterator begin, EdgePairs::iterator end)
{
  return new GraphImp2(noVerticies, begin, end);
}

static void dijkstra(Graph& g, Vertex s, Vertices& p, Reals& d) {
  dijkstra_shortest_paths(
      g, 
      s,
      weight_map(get(&EdgeProperty::weight, g)).
      predecessor_map(make_iterator_property_map(p.begin(), get(vertex_index, g))).
      distance_map(make_iterator_property_map(d.begin(), get(vertex_index, g)))
  );  
}

Paths2* GraphImp2::shortestPaths(Trapezoid2* source) const
{
  assert(iGraph != 0);
  assert(source != 0);
  Graph& g = *iGraph;
  
  Vertex s = vertex(source->tag(), g);

  PathsImp2* paths = new PathsImp2(num_vertices(g));
  Reals&   d = paths->iDistances;
  Vertices& p = paths->iPredecessors;
  paths->iG = iGraph;    
  
  dijkstra(g, s, p, d);
    
  return paths;
}

/*!
  Used by A* search algorithm to give an estimate of how far from the goal vertex
  a given vertex is. This functor compares the geometric distance between the vertices.
*/
struct DistanceHeuristic
{
  DistanceHeuristic(Graph* agraph, Vertex agoal) : graph(agraph), goal(agoal)
  {    
  }
  
  real operator()(Vertex v) 
  {
    Graph& g = *graph;
    return (g[goal]-g[v]).length();
  }
  Vertex goal;
  Graph* graph;
};

/*!
  Exception thrown when we find the goal
*/
struct FoundGoalException 
{
  
};

/*!
  This functor is called for every vertex visited by the A* search algorithm.
  The Boost implementation of A* is very generic. It will not terminate by itself
  when it reaches the goal destination because there is no goal. When calling the 
  A* you don't specify goal. The only thing you determine is start and a heuristic 
  function which is used to determine which vertex to visit next. 
  
  In this sense A* is no different than Dijkstra. The only difference is that
  a heuristic function is used to determine which vertex to visit next.
  
  Because we want to stop searching when the goal is reached, we specify this 
  visitor which will terminate the search by throwing an exception when the goal
  is found.
*/
struct GoalVisitor : public boost::default_astar_visitor
{
  GoalVisitor(Vertex agoal) : goal(agoal)
  {
    
  }
  
  template <class GraphType>
  void examine_vertex(Vertex u, GraphType& g) {
    if(u == goal)
      throw FoundGoalException();
  }
  
  Vertex goal;
};

bool GraphImp2::shortestPath(Trapezoid2* source, Trapezoid2* target, Polygon2& path) const
{
  assert(iGraph != 0);
  assert(source != 0 && target != 0);
  Graph& g = *iGraph;

  // Find source and target vertex in graph
  Vertex s = vertex(source->tag(), g);
  Vertex t = vertex(target->tag(), g);
  
  Vertices p(num_vertices(g));  // Predecessor map
  Reals    d(num_vertices(g));  // Distance map
  
  // A* will find ALL shortest paths like Dijkstra so we need to throw exception when goal has been found
  try {
    astar_search(
      g, 
      s,
      DistanceHeuristic(iGraph, t),
      weight_map(get(&EdgeProperty::weight, g)).
      predecessor_map(make_iterator_property_map(p.begin(), get(vertex_index, g))).
      distance_map(make_iterator_property_map(d.begin(), get(vertex_index, g))).
      visitor(GoalVisitor(t))
    );    
  }
  catch (FoundGoalException e) {
    Vertex t = vertex(target->tag(), g);
    
    // Store coordinates of shortest path
    while (t != p[t]) {
      path.push_back(g[t]);
      t = p[t];
    }
    path.push_back(g[t]);    
    reverse(path.begin(), path.end());
    
    return true;
  }
  
  return false;
}

bool GraphImp2::fixedLengthPath(Trapezoid2* source, Trapezoid2* target, real distance, Polygon2& path) const
{
  assert(iGraph != 0);
  assert(source != 0);
  Graph& g = *iGraph;

  // Variables for book keeping
  Vertex vs, vc;  // 'vertex selected' and 'vertex candidate'
  Vertex s, t;    // source vertex and target vertex
  Edge es, ec;    //  'edge selected' and 'edge candidate'
  real ds, dc;        // 'distance selected'
  
  vs = s = vertex(source->tag(), g);
  t = vertex(target->tag(), g);

  Vertices  p(num_vertices(g));
  Reals     d(num_vertices(g));  
  
  dijkstra(g, t, p, d);

  // Check if the shortest path has the desired length or is too long
  real cur_dist = d[s];
  real accum_dist = 0.0;
  
  if (cur_dist > distance)
    return false;
  if (cur_dist == distance) {
    return pathFrom(g, s, p, path);
  }
  
  OutEdgeIterator ei, ee, selected;
  while (vs != t) {
    // For each selected vertex iterate over out edges to determine next vertex in path
    tie(ei, ee) = out_edges(vs, g);
    es = *ei;      //  'es' is temporary selected next edge
    vs = ::target(es, g);  // 'vs' is temporary selected as next vertex
    
    // distance to to target through temporary selected vertex 'vs'
    ds = g[es].weight+accum_dist+d[vs];    
    
    // Loop through all neighbor vertices to last selected vertex and find
    // the vertex that will increase the distance of the path but still
    // keep it below the max distance 'distance'
    for (++ei; ei != ee; ++ei) {
      ec = *ei;           // 'ec' is candidate for selected edge
      vc = ::target(ec, g); // 'vc' is candidate for selected vertex

      // distance to target through candidate vertex 'vc'
      dc = g[ec].weight+accum_dist+d[vc];

      // Only select candidate vertex 'vc' if it can increase distance to target
      // but still be less than max distance.
      if (dc < ds || dc > distance) 
        continue;

      // Make candidate vertex the selected vertex
      ds = dc;
      es = ec;
      vs = vc;      
    }
    path.push_back(g[vs]);  
  }

  return true;
}

static bool isSmall(Vertex vi, const Graph& g)
{
  return false;
}

/*! 
  Adds a choke point to 'chokepoints' if requirements for choke points is fullfilled.
*/
static void addChokePoint(Vertex vi, const Vertices& p, const Graph& g, ChokePoints& chokepoints)
{
  // We got a choke poin if there is a change from a small trapezoid to a large
  // or vice versa
  Vertex vc, v;
  
  if (isSmall(vi, g) != isSmall(p[vi], g)) {
    if (isSmall(vi, g)) { vc = p[vi]; v = vi; }
    else             { v = p[vi]; vc = vi; }
    Point2 p = g[vc];
    Vector2 dir = (p - g[v]).unit();
    chokepoints.push_back(make_pair(p, dir));
  }  
}

/*!
  Returns a list of all the exit and entry points of narrow passage ways
  that have to be passed to reach any of the important/strategic location
  on the map specified by 'important_loc'.
  'start_trap' is the location were player of other character that needs to reach
  these important locations will start. Or rather the trapezoid in which this location exits.
*/
bool GraphImp2::chokePoints(Trapezoid2* start_trap, const Trapezoids2& important_loc, ChokePoints& chokepoints) const
{
  assert(iGraph != 0);
  Graph& g = *iGraph;

  // Variables for book keeping
  
  Vertex s = vertex(start_trap->tag(), g);

  Vertices  p(num_vertices(g));
  Reals     d(num_vertices(g));  
  
  dijkstra(g, s, p, d);

  Vertex vi;  // 'vertex important', 'vertex choke point'
  Trapezoids2::const_iterator i;
  for (i = important_loc.begin(); i != important_loc.end(); ++i) {
    vi = vertex((*i)->tag(), g);  // vi is 'vertex important' 

    while (vi != p[vi]) {
      addChokePoint(vi, p, g, chokepoints);
      vi = p[vi];
    }
    addChokePoint(vi, p, g, chokepoints);
  }
 
  return true;
}

void GraphImp2::printGraph() const
{
  printBeginGraph("mygraph");            
  EdgeIterator ei, ee;
  for (tie(ei, ee) = edges(*iGraph); ei != ee; ++ei) {
    VizEdge ge(*ei, *iGraph);
    ge.print();
  }
  printEndGraph();            
}
