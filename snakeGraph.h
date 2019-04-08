#ifndef SNAKEGRAPH_H
#define SNAKEGRAPH_H
#include <map>
#include <set>
#include <string>
#include <fstream>

using namespace std;

typedef map<int, set<int> > snakeMap_t;

// This is the Graph API we have talked about.  The name of
// the class is changed so that it does not conflict with
// student code
class SnakeGraph
{
   snakeMap_t vertices;

protected:
   int numVertices, numEdges;

public:
   SnakeGraph();
   SnakeGraph(const int *, int, int);

   set<int> Vertices() const;
   set<int> adj(int v);
   virtual void addEdge(int, int);
   int V() const;
   int E() const;
};

#endif
