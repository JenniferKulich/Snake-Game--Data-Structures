#ifndef VIDEOALGORITHM_H
#define VIDEOALGORITHM_H

#include <iostream>
#include <list>
#include "playfield.h"

struct VideoPath
{
  int fcost;
  int hcost;
  int parent;
};

void FindPath(int startIndex, int targetIndex);
std::list<int> getNeightbors(int currentIndex);
void retracePath(int startIndex, int endIndex);


#endif
