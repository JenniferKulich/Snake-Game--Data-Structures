#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <map>
#include <set>
#include "playfield.h"
typedef std::map<int, std::set<int> > map_t;

class Graph
{
   map_t vertices;
   int numVertices;
   int numEdges;
public:
   Graph();
   Graph(const int *grid, int width, int height);

   std::set<int> adj(int);
   virtual void addEdge(int, int);
   int V() const;
   int E() const;
   std::set<int> Vertices() const;
};

#endif
