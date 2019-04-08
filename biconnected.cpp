/********************************************************************//**
 * @file
 ***********************************************************************/
#include "biconnected.h"
#include <iostream>

Biconnected::Biconnected(SnakeGraph *G) : Time(0)
{
   // Set all the parents to be NO_PARENT
   for (int v : G->Vertices())
      parent[v] = NO_PARENT;

   // arbitrarily start at the first vertex in the vertex list
   dfs(G, *((G->Vertices()).begin())); 
}

// Return the set of articulation nodes
set<int> Biconnected::articulationNodes() { return articulationPoints ; }

// Mark the visited and low times for each vertex in the graph using
// DFS.  Using the rules for articulation points, mark any vertex
// that is an articulation node and add it to the articulationPoints
// set
// @param[in] : G - An unweighted graph
// @param[in] : v - The vertex to perform DFS from
void Biconnected::dfs(SnakeGraph *G, int v)
{
   int childCount = 0;
   bool isArticulation = false;

   // Set visited, time and loTime.  loTime may be changed later
   visited[v] = true;
   time[v] = Time;
   loTime[v] = Time++;
   for (int w : G->adj(v))
   {
      if (w == parent[v]) continue; // parents are not back edges
      if (!visited[w])
      {
         parent[w] = v;
         childCount++;
         dfs(G, w);
         if (time[v] <= loTime[w])
            isArticulation = true;
         else
         {
            loTime[v] = (loTime[v] < loTime[w]) ? loTime[v] : loTime[w];
         }
      }
      else // back edges
      {
         loTime[v] = (time[w] < loTime[v]) ? time[w] : loTime[v];
      }
   }
   // Root nodes with 2 or more independent children or
   // Nodes with visit times equal or before adjacent node
   //  lo times are articulation points
   if ((parent[v] == NO_PARENT && childCount >= 2) ||
       (parent[v] != NO_PARENT && isArticulation))
      articulationPoints.insert(v);
}
