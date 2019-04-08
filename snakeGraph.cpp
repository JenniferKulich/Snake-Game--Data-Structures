#include <iostream>
#include "snakeGraph.h"

// These make sure rows and columns are valid
#define validRow(r) (r >= 0 && r < height)
#define validCol(c) (c >= 0 && c < width)
#define INVALID_VALUE 1

// Changing this macro will change which cells can start or
// end an edge in the graph
#define canEnter(r,c) (validRow(r) && validCol(c) && \
                      (grid[(r)*width+c] != INVALID_VALUE))

SnakeGraph::SnakeGraph() { } // for the derived types

// Convert a 2d array (height x width) into a graph
// This constructor assumes the graph is undirected and
// unweighted
SnakeGraph::SnakeGraph(const int *grid, int width, int height)
{
   for (int index = 0 ; index < height * width ; index++)
   {
      int row = index / width;
      int col = index % width;

      // The canEnter macro decides if a cell can originate and edge
      if (!canEnter(row, col)) continue;

      // Make sure that valid cells will show up in the vertex list
      // These cells may not have any edges incident
      if (vertices.find(index) == vertices.end())
         vertices[index] = set<int>{};

      // The grid represents a graph with possible edges in the four
      // cardinal directions {UP, DOWN, RIGHT, LEFT}  
      if (canEnter(row-1, col)) addEdge(index, index-width);
      if (canEnter(row+1, col)) addEdge(index, index+width);
      if (canEnter(row, col+1)) addEdge(index, index+1);
      if (canEnter(row, col-1)) addEdge(index, index-1);
   }

   numVertices = vertices.size();
   numEdges = 0;
   for (int v : Vertices())
      numEdges += adj(v).size();

   numEdges /= 2;
}

// Return the number of vertices in the graph
int SnakeGraph::V() const { return numVertices ; }

// Return the number of edges in the graph
int SnakeGraph::E() const { return numEdges ; }

// Return the set of vertices in the graph
// Since we are returning a set, the vertices will
// be ordered
set<int> SnakeGraph::Vertices() const
{
   set<int> retVal;
   for (auto &kv : vertices)
      retVal.insert(kv.first);
 
   return retVal;
}

// Return the adjacency set for the vertex v
set<int> SnakeGraph::adj(int v)
{
   return vertices[v];
}

void SnakeGraph::addEdge(int v, int w)
{
   vertices[v].insert(w);
   vertices[w].insert(v);
}
