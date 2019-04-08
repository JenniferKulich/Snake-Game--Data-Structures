#ifndef VIDEOALGORITHM_H
#define VIDEOALGORITHM_H

#include <iostream>
#include <list>
#include <vector>
#include "playfield.h"
#include "graph.h"
#include "BFSPath.h"

using namespace std;


std::vector<int> FindPath( const int *grid, int startIndex, int targetIndex);
std::vector<int> getNeighbors(const int *grid, int currentIndex);
std::vector<int> retracePath(vector<int> parent, int startIndex, int endIndex);
int findGCost(const int *grid, int currentSpot, int startSpot);
int findHCost(const int *grid, int currentSpot, int endSpot);
int getDistance(int startIndex, int endIndex);




#endif
