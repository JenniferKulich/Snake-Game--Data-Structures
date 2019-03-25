#include "BFSPath.h"

BFSPaths::BFSPaths(Graphs *G, int s)
{
  bfs(G,s);
}

BFSPaths::Distance(int v)
{
  return info[v].distance;
}

std::list<int> BFSPaths::pathTo(int v)
{
  std::list<int>path;
  if(!hasPath(v))
    return path;
  for(int x = v; x != s; x = info[x].prev)
    path.push_front(x);
  path.push_front(s);
  return path;
}

void BFSPaths::bfs(Graph *G, int s)
{
  std::queue<int> frontier;
  frontier.push(s);
  intfo[s].marked = true;
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


bool BFS::hasPath(int v)
{
  return info[v].marked;

}
