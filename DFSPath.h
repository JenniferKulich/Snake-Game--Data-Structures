#ifndef __DFSPATH_H
#define __DFSPATH_H
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <queue>
#include "graph.h"

struct DFSInfo
{
  bool marked;
  int dist;
  int prev;
};

class DFSPaths
{
  std::map<int,DFSInfo> DFSinfo;
//  std::map<int,bool> marked;
//  std::map<int,int> prev;
  int s; //start

public:
    DFSPaths(Graph *G, int s);
    void dfs(Graph *G, int s);
    bool hasPathTo(int v);
    std::list<int> PathTo(int v);
};

#endif
