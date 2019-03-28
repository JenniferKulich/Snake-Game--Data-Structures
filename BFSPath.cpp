#include "BFSPath.h"

BFSPaths::BFSPaths(Graph *G, int start)
{
  s = start;
  bfs(G,s);
}

int BFSPaths::Distance(int v)
{
  return info[v].dist;
}

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


bool BFSPaths::hasPath(int v)
{
  return info[v].marked;

}
