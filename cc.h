#include <queue>
#include "snakeGraph.h"

/***************************************************************************//**
 * @brief Calculate the connected components in a graph
 *
 * @par Description
 *    Equivalence relation
 *    * Reflexive: v is connected to v
 *    * Symmetric: if v is connected to w then w is connected to v
 *    * Transitive: if v is connected to w and w connected to u then v is 
 *       connected to u
 *
 * A connected component is a maximal set of connected vertices.
 * This is a client class of the Graph API we've talked about in class
 ******************************************************************************/
class CC
{
   map<int, bool> marked;
   map<int, int> cc;
   int ccount;

public:
   CC(SnakeGraph *G);

   bool connected(int v, int w);
   int count();
   int id(int v);
   void bfs(SnakeGraph *, int);
};
