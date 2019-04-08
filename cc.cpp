#include <iostream>
#include "cc.h"

// Constructor which uses BFS to count connected components
// Could also use dfs for this purpose
CC::CC(SnakeGraph *G) : ccount(0)
{
   for (int v : G->Vertices())
      if (!marked[v])
      {
         bfs(G, v);
         ccount++;
      }
}

// Perform a breadth-first-search on the graph and identify
// Nodes in the same connected component
void CC::bfs(SnakeGraph *G, int s)
{
   queue<int> q;
   q.push(s);
   marked[s] = true;
   while (!q.empty())
   {
      int v = q.front() ; q.pop();
      cc[s] = ccount;
      for (int w : G->adj(v))
      {
         if (!marked[w])
         {
            q.push(w);
            marked[w] = true;
         }
      }
   }
}
  
// Return whether or not two nodes are in the same
// connected component 
bool CC::connected(int v, int w)
{
   return cc[v] == cc[w];
}

// Return the number of connected components found
int CC::count() { return ccount; }

// Return which connected component a vertex is in
int CC::id(int v) { return cc[v] ; }
