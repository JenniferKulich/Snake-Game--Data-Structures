/********************************************************************//**
 * @file
 ***********************************************************************/
#ifndef BICONNECTED_H
#define BICONNECTED_H
#include "snakeGraph.h"

#define NO_PARENT -1 // Dangerous since we might have negative numbers for nodes

/********************************************************************//**
 * @class Biconnected
 *
 * A class that uses the Graph API representation of a graph to
 * determine if a graph is biconnected
 **********************************************************************/
class Biconnected
{
   // Could also put these in an info structure instead of having all the
   // parallel maps
   std::map<int, bool> visited;
   std::map<int, int> time;
   std::map<int, int> loTime;
   std::map<int, int> parent;
   std::set<int> articulationPoints;
   int Time;
public:
   Biconnected(SnakeGraph *G);
    
   void dfs(SnakeGraph *, int);

   set<int> articulationNodes();
};
#endif
