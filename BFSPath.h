#ifndef __BFSPATH_H
#define __BFSPATH_H
#include <iostream>
#include <map>
#include <set>
#include "graph.h"


struct Info
{
  bool marked;
  int dist;
  int prev;
}

class BFSPaths
{
  map<int,Info> info;
  int s; //start node

public:
    BFSPath(Graph *G, int s);
    void bfs(Graph *G, int s);
    bool hasPath(int v);
    std::list<int> PathTo(int v);
    int Distance(int v);
};

#endif
