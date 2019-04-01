#include "DFSPath.h"


DFSPaths::DFSPaths(Graph *G, int start)
{
  s = start;
  dfs(G,s);
}

bool DFSPaths::hasPathTo(int v)
{
  return DFSinfo[v].marked;
}

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

void DFSPaths::dfs(Graph *G, int s)
{
  DFSinfo[s].marked = true;
  for(int w : G->adj(s))
  {
    if(!DFSinfo[w].marked)
    {
      dfs(G,w);
      DFSinfo[w].prev = s;
    }
  }
}
