#include "DFSPath.h"

/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Constructs the DFS paths and calls DFS
 *
 * @param[in] G - The playfield
 * @param[in] start - the node to start looking from
 *
 * @return - set of coordinates for the value that's being looked for
 *
 *****************************************************************************/
DFSPaths::DFSPaths(Graph *G, int start)
{
  s = start;
  dfs(G,s);
}

/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Checks if there is a path to the end node
 *
 * @param[in] v - the end node
 *
 * @return - if the end node is marked or not
 *
 *****************************************************************************/
bool DFSPaths::hasPathTo(int v)
{
  return DFSinfo[v].marked;
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Will get the path from the end node to the start node and store the indexes
 * in a list
 *
 * @param[in] v - The end node
 *
 * @return - a linked list of the path from the start node to the end node
 *
 *****************************************************************************/
std::list<int>DFSPaths::PathTo(int v)
{
  std::list<int>path;
  if(!hasPathTo(v))
    return path;

  for(int x = v; x != s; x = DFSinfo[x].prev)

    path.push_front(x);
    //we don't want the head because we only want the places to move to
  //path.push_front(s);
  return path;
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * The DFS algorithm to find the path from the start node to the end node
 *
 * @param[in] G - The playfield
 * @param[in] s - the start node
 *
 *****************************************************************************/
void DFSPaths::dfs(Graph *G, int v)
{
  DFSinfo[v].marked = true;
  for(int w : G->adj(v))
  {
    if(!DFSinfo[w].marked)
    {
      dfs(G,w);
      DFSinfo[w].prev = v;
    }
  }
}
