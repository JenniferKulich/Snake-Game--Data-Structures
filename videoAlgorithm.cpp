#include "videoAlgorithm.h"


std::vector<int> FindPath( const int *grid, int startIndex, int targetIndex)
{

  int openGCost, openHCost, openFCost;
  int currentGCost, currentHCost, currentFCost;
  int neighborGCost;

  //construct a grid
  //construct a graph to be able to pass into g, h, and f cost to use BFS
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);


  std::vector<int> open;
  std::vector<int> closed;
  std::vector<int> parents;

  //add the start node to the open list
  open.insert(open.begin(),startIndex); /*open.push_front(startIndex);*/

  //loop through to find the path- this will be slow until I implement the heap
  //while the open list is not empty, try to find paths
  while(!open.empty())
  {
    //set the current index equal to the first node in open
    int currentIndex = open[0];

    //loop through all of the open nodes in the list
    for(int i = 0; i < (int)open.size(); i++)
    {
      //if the f costs of the open are less than or equal to the current index,
      //AND the open h cost is less than the current h cost, then pick the open
      //index we're checking as the one that's current- ie it is the lowest
      //f cost node
      //int findGCost(Graph *G, int currentSpot, int startSpot);
      //int findHCost(Graph *G, int currentSpot, int endSpot);

      openGCost = findGCost(grid/*&graph*/, open[i], startIndex);
      openHCost = findHCost(grid/*&graph*/, open[i], targetIndex);
      openFCost = openGCost + openHCost;

      currentGCost = findGCost(grid/*&graph*/, currentIndex, startIndex);
      currentHCost = findHCost(grid/*&graph*/, currentIndex, targetIndex);
      currentFCost = currentGCost + currentHCost;

      if((openFCost < currentFCost || openFCost == currentFCost) && openHCost < currentHCost)
        currentIndex = open[i];

      /*if(open[i].fcost < currentIndex.fcost || open[i].fcost == currentIndex.fcost
        && open[i].hcost < currentIndex.hcost)
        currentIndex = open[i];
      */
    }

    //take the lowest picked node off of the open list and add it to the closed
    //list
    open.erase(open.begin()/*currentIndex*/); //should only be the first element I think
    closed.insert(closed.begin(),currentIndex);

    //check if the current index is the target index- if so, we've found the path
    if(currentIndex == targetIndex)
    {
      //retracePath(startIndex, targetIndex);
      return retracePath(parents, startIndex, targetIndex);
    }

    //go through each neightboring node and check if it's able to be accessed
    //or not. If it is in the closed list or not walkable, continue
    std::vector<int> neighbors = getNeighbors(grid, currentIndex);
    for(int i = 0; i < (int)neighbors.size(); i++)
    {
      //see if neighbor is in closed list, if it is, move on to next neighbor
      bool inClosed = false;
      for(int j = 0; j < (int)closed.size(); i++)
      {
        if(closed[j] == neighbors[i])
          inClosed = true;
      }
      if(inClosed)
        continue;

      //get how much it would cost to move to that neightbor
      currentGCost = findGCost(grid /*&graph*/, currentIndex, startIndex);
      neighborGCost = findGCost(grid/*&graph*/, neighbors[i], startIndex);
      int newMovementCostToNeighbor = currentGCost/*currentIndex.gcost*/ + getDistance(currentIndex, neighbors[i]);
      //see if open contains neighbor
      bool inOpen = false;
      for(int i = 0; i < (int)open.size(); i++)
      {
        if(open[i] == neighbors[i])
          inOpen = true;
      }

      if(newMovementCostToNeighbor < neighborGCost/*neighbors[i].gcost*/ || !inOpen/*!open.contains(neighbor[i])*/)
      {
        /*
        //set the f cost of the neighbor
        neighbor.gcost = newMovementCostToNeighbor;
        neighbor.hcost = getDistance(neighbor[i], targetIndex);

        //set the neighbor parent as the currentIndex
        neighbor.parent = currentIndex;
        */
        //add the currentIndex to the parents vector
        parents.insert(parents.begin(), currentIndex);

        //add neighbor to the beginning of the open list since it'll be the
        //next currentIndex
        //if open does not contain neighbor, add it
        inOpen = false;
        for(int i = 0; i < (int)open.size(); i++)
        {
          if(open[i] == neighbors[i])
            inOpen = true;
        }
        if(!inOpen)
        open.insert(open.begin(),neighbors[i]);
        /*
        if(!open.constains(neighbor[i]))
          open.add(neighbor);
        */
      }
    }

  }

  std::vector<int> finalPath = retracePath(parents, startIndex, targetIndex);
  return finalPath;
}


std::vector<int> retracePath(vector<int> parent, int startIndex, int endIndex)
{
    std::vector<int> path;
    int currentIndex = endIndex;
    while(currentIndex != startIndex)
    {
      path.insert(path.begin(), currentIndex);
      currentIndex = /*currentIndex.parent*/parent[0];
      //take the current index off of parent
      parent.erase(parent.begin());
    }

    // std::reverse(path.begin(),path.end());

    //reverse(path.begin(), path.end());
    return path;
}



//will get the distance between two indices
int getDistance(int startIndex, int endIndex)
{
  //convert the indices to rows and columns
  int startRow = startIndex / PLAYFIELD_WIDTH;
  int startColumn = startIndex % PLAYFIELD_WIDTH;
  int endRow = endIndex / PLAYFIELD_WIDTH;
  int endColumn = endIndex % PLAYFIELD_WIDTH;

  //get dX by doing the x startIndex - endIndex
  int dX = startRow + endRow;
  //get the dY by doing the y startIndex - endIndex
  int dY = startColumn + endColumn;
  //add the dx and dY together to get the distance
  int total = dX + dY;
  return total;
}


//returns the index of each possible neightbor
std::vector<int> getNeighbors( const int *grid, int currentIndex)
{
  std::vector<int> neighborIndices;

  //check if in top left corner
  if(currentIndex == (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - PLAYFIELD_WIDTH)
  {
    //check if the down block is a valid neighbor- clear or food
    if(grid[currentIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex - PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - PLAYFIELD_WIDTH);
    //check if the right block is a valid neightbor- clear or food
    if(grid[currentIndex + 1] == CLEAR_VALUE || grid[currentIndex + 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + 1);
  }

  //check if in top right corner
  else if(currentIndex == (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1)
  {
    //check if the down block is valid neighbor- clear or food
    if(grid[currentIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex - PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - PLAYFIELD_WIDTH);
    //check if left block is a valid neightbor- clear or food
    if(grid[currentIndex - 1] == CLEAR_VALUE || grid[currentIndex - 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - 1);
  }

  //check if in bottom left corner
  else if(currentIndex == 0)
  {
    //check if the up block is valid neighbor- clear or food
    if(grid[currentIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex + PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + PLAYFIELD_WIDTH);
    //check if right block is a valid neightbor- clear or food
    if(grid[currentIndex + 1] == CLEAR_VALUE || grid[currentIndex + 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + 1);
  }

  //check if in bottom right corner
  else if(currentIndex == PLAYFIELD_WIDTH - 1)
  {
    //check if the up block is valid neightbor- clear or food
    if(grid[currentIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex + PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + PLAYFIELD_WIDTH);
    //check if left block is a valid neighbor- clear or food
    if(grid[currentIndex - 1] == CLEAR_VALUE || grid[currentIndex - 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - 1);
  }

  //check if on top edge
  else if(currentIndex >= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH - PLAYFIELD_WIDTH) &&
  currentIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1)
  {
    //check if right block valid neighbor- clear or food
    if(grid[currentIndex + 1] == CLEAR_VALUE || grid[currentIndex + 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + 1);
    //check if left block valid neightbor- clear or food
    if(grid[currentIndex - 1] == CLEAR_VALUE || grid[currentIndex - 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - 1);
    //check if down block valid- clear or food
    if(grid[currentIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex - PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - PLAYFIELD_WIDTH);
  }

  //check if on bottom edge
  else if(currentIndex >= 0 && currentIndex <= PLAYFIELD_WIDTH - 1)
  {
    //check if right block valid neighbor- clear or food
    if(grid[currentIndex + 1] == CLEAR_VALUE || grid[currentIndex + 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + 1);
    //check if left block valid neightbor- clear or food
    if(grid[currentIndex - 1] == CLEAR_VALUE || grid[currentIndex - 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - 1);
    //check if up block valid- clear or food
    if(grid[currentIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex + PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + PLAYFIELD_WIDTH);
  }

  //check if on left wall
  else if(currentIndex % PLAYFIELD_WIDTH == 0)
  {
    //check if right block valid neightbor- clear or food
    if(grid[currentIndex + 1] == CLEAR_VALUE || grid[currentIndex + 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + 1);
    //check if up block valid neightbor- clear or food
    if(grid[currentIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex + PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + PLAYFIELD_WIDTH);
    //check if down block valid neightbor - clear or food
    if(grid[currentIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex - PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - PLAYFIELD_WIDTH);
  }

  //check if on right wall
  else if((currentIndex + 1) % PLAYFIELD_WIDTH == 0)
  {
    //check if left block valid neightbor- clear or food
    if(grid[currentIndex - 1] == CLEAR_VALUE || grid[currentIndex - 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - 1);
    //check if up block valid neightbor- clear or food
    if(grid[currentIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex + PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + PLAYFIELD_WIDTH);
    //check if down block valid neightbor - clear or food
    if(grid[currentIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex - PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - PLAYFIELD_WIDTH);
  }

  //in middle
  else
  {
    //add all of the edges as long as they are valid- clear or food
    //UP
    if(grid[currentIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex + PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + PLAYFIELD_WIDTH);
    //LEFT
    if(grid[currentIndex - 1] == CLEAR_VALUE || grid[currentIndex - 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - 1);
    //DOWN
    if(grid[currentIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE || grid[currentIndex - PLAYFIELD_WIDTH] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex - PLAYFIELD_WIDTH);
    //RIGHT
    if(grid[currentIndex + 1] == CLEAR_VALUE || grid[currentIndex + 1] == FOOD_VALUE)
      neighborIndices.insert(neighborIndices.begin(), currentIndex + 1);
  }

  return neighborIndices;
}


//current spot is the index you're checking
//startSpot is the orgin index
int findGCost(const int *grid /*Graph *graph*/, int currentSpot, int startSpot)
{
  //construct a graph
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  //construct BFS and pass in the headspot
  BFSPaths BFSpath(&graph, startSpot);

  //get the list of spots which is the path to the food
  std::list<int>pathToFood = BFSpath.PathTo(currentSpot);

  //get the distance between the two spots, passing in end index
  int distance = BFSpath.Distance(currentSpot);

  return distance;
}

int findHCost(const int *grid, int currentSpot, int endSpot)
{
  //construct a graph
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  //construct BFS and pass in the headspot
  BFSPaths BFSpath(&graph, endSpot);

  //get the list of spots which is the path to the food
  std::list<int>pathToFood = BFSpath.PathTo(currentSpot);

  //get the distance between the two spots, passing in end index
  int distance = BFSpath.Distance(currentSpot);

  return distance;
}
