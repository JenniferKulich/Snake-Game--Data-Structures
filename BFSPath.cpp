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
    std::cout << "PathTo 1" << std::endl;
  //GETS HUNG UP HERE
  for(int x = v; x != s; x = info[x].prev)
  {
    std::cout<< std::endl <<std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "s: " << s << std::endl;
    std::cout << "v: " << v <<std::endl;
    std::cout << "info[x].prev: " << info[x].prev << std::endl;
    path.push_front(x);
    std::cout << "pushed!" << std::endl;

  }
  std::cout << "PathTo 2" << std::endl;
  path.push_front(s);
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
