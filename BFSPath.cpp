#include "BFSPath.h"

/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Constructs the BFS class and calls BFS
 *
 * @param[in] G - The playfield
 * @param[in] start - the node to start looking from
 *
 *****************************************************************************/
BFSPaths::BFSPaths(Graph *G, int start)
{
  s = start;
  bfs(G,s);
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * The BFS algorithm to find the path from the start node to the end node
 *
 * @param[in] G - The playfield
 * @param[in] s - the start node
 *
 *****************************************************************************/
void BFSPaths::bfs(Graph *G, int s)
{
  std::queue<int> frontier;
  frontier.push(s);
  info[s].marked = true;
  info[s].dist = 0;
  while(!frontier.empty())
  {
    int v = frontier.front();
    frontier.pop();
    for(int w : G->adj(v))
    {
      if(!info[w].marked)
      {
        frontier.push(w);
        info[w].marked = true;
        info[w].prev = v;
        info[w].dist = info[v].dist + 1;
      }
    }
  }
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * checks if there is a path from the start node to the end node by checking if
 * the end node has been marked
 *
 * @param[in] v - the end node
 *
 * @return - if the end node has been marked or not
 *
 *****************************************************************************/
bool BFSPaths::hasPath(int v)
{
  return info[v].marked;

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
std::list<int> BFSPaths::PathTo(int v)
{
  std::list<int>path;
  if(!hasPath(v))
    return path;

  for(int x = v; x != s; x = info[x].prev)

    path.push_front(x);
    //we don't want the head because we only want the places to move to
  //path.push_front(s);
  return path;
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Gets the distance of the path from the start node to the end node
 *
 * @param[in] v - The end node
 *
 * @return - distance from the start node to the end node
 *
 *****************************************************************************/
int BFSPaths::Distance(int v)
{
  return info[v].dist;
}
