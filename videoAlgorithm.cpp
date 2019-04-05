#include "videoAlgorithm.h"


void FindPath(int startIndex, int targetIndex)
{
  std::list<int> open;
  std::list<int> closed;

  //add the start node to the open list
  open.add(startIndex);

  //loop through to find the path- this will be slow until I implement the heap
  //while the open list is not empty, try to find paths
  while(!open.empty())
  {
    //set the current index equal to the first node in open
    int currentIndex = open[0];

    //loop through all of the open nodes in the list
    for(int i = 0; i < open.size(); i++)
    {
      //if the f costs of the open are less than or equal to the current index,
      //AND the open h cost is less than the current h cost, then pick the open
      //index we're checking as the one that's current- ie it is the lowest
      //f cost node
      if(open[i].fcost < currentIndex.fcost || open[i].fcost == currentIndex.fcost
        && open[i].hcost < currentIndex.hcost)
        currentIndex = open[i];
    }

    //take the lowest picked node off of the open list and add it to the closed
    //list
    open.remove(currentIndex);
    closed.add(currentIndex);

    //check if the current index is the target index- if so, we've found the path
    if(currentIndex == targetIndex)
    {
      retracePath(startIndex, targetIndex);
      return;
    }

    //go through each neightboring node and check if it's able to be accessed
    //or not. If it is in the closed list or not walkable, continue
    std::list<int> neightbors = getNeighbors(currentIndex);
    for(i = 0; i < neighbors.size(); i++)
    {
      //see if neighbor is in closed list, if it is, move on to next neighbor
      if(closed.contains(neighbor[i]))
        continue;

      //get how much it would cost to move to that neightbor
      int newMovementCostToNeighbor = currentIndex.gcost + getDistance(currentIndex, neighbors[i])
      if(newMovementCostToNeighbor < neighbors[i].gcost || !open.contains(neighbor[i]))
      {
        //set the f cost of the neighbor
        neighbor.gcost = newMovementCostToNeighbor;
        neighbor.hcost = getDistance(neighbor[i], targetIndex);

        //set the neighbor parent as the currentIndex
        neighbor.parent = currentIndex;

        //if open does not contain neighbor, add it
        if(!open.constains(neighbor[i]))
          open.add(neighbor);
      }
    }

  }
}

void retracePath(int startIndex, int endIndex)
{
    std::list<int> path;
    int currentIndex = endIndex;
    while(currentIndex != startIndex)
    {
      path.add(currentIndex);
      currentIndex = currentIndex.parent;
    }

    path.reverse();
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
  int dY = startColumn + endColumn
  //add the dx and dY together to get the distance
  return (dX + dY);
}


//returns the index of each possible neightbor
std::list<int> getNeightbors(int currentIndex)
{
  std::list<int> neighborIndices;

  //check if in top left corner
  if(currentIndex == (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - PLAYFIELD_WIDTH)
  {
    //check if the down block is a valid neighbor- clear or food
    if(currentIndex - PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex - PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex - PLAYFIELD_WIDTH);
    //check if the right block is a valid neightbor- clear or food
    if(currentIndex + 1 == CLEAR_VALUE || currentIndex + 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex + 1);
  }

  //check if in top right corner
  else if(currentIndex == (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1)
  {
    //check if the down block is valid neighbor- clear or food
    if(currentIndex - PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex - PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex - PLAYFIELD_WIDTH);
    //check if left block is a valid neightbor- clear or food
    if(currentIndex - 1 == CLEAR_VALUE || currentIndex - 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex - 1);
  }

  //check if in bottom left corner
  else if(currentIndex == 0)
  {
    //check if the up block is valid neighbor- clear or food
    if(currentIndex + PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex + PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex + PLAYFIELD_WIDTH);
    //check if right block is a valid neightbor- clear or food
    if(currentIndex + 1 == CLEAR_VALUE || currentIndex + 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex + 1);
  }

  //check if in bottom right corner
  else if(currentIndex == PLAYFIELD_WIDTH - 1)
  {
    //check if the up block is valid neightbor- clear or food
    if(currentIndex + PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex + PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex + PLAYFIELD_WIDTH);
    //check if left block is a valid neighbor- clear or food
    if(currentIndex - 1 == CLEAR_VALUE || currentIndex - 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex - 1);
  }

  //check if on top edge
  else if(currentIndex >= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH - PLAYFIELD_WIDTH) &&
  currentIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1)
  {
    //check if right block valid neighbor- clear or food
    if(currentIndex + 1 == CLEAR_VALUE || currentIndex + 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex + 1);
    //check if left block valid neightbor- clear or food
    if(currentIndex - 1 == CLEAR_VALUE || currentIndex - 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex - 1);
    //check if down block valid- clear or food
    if(currentIndex - PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex - PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex - PLAYFIELD_WIDTH);
  }

  //check if on bottom edge
  else if(currentIndex >= 0 && currentIndex <= PLAYFIELD_WIDTH - 1)
  {
    //check if right block valid neighbor- clear or food
    if(currentIndex + 1 == CLEAR_VALUE || currentIndex + 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex + 1);
    //check if left block valid neightbor- clear or food
    if(currentIndex - 1 == CLEAR_VALUE || currentIndex - 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex - 1);
    //check if up block valid- clear or food
    if(currentIndex + PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex + PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex + PLAYFIELD_WIDTH);
  }

  //check if on left wall
  else if(currentIndex % PLAYFIELD_WIDTH == 0)
  {
    //check if right block valid neightbor- clear or food
    if(currentIndex + 1 == CLEAR_VALUE || currentIndex + 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex + 1);
    //check if up block valid neightbor- clear or food
    if(currentIndex + PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex + PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex + PLAYFIELD_WIDTH);
    //check if down block valid neightbor - clear or food
    if(currentIndex - PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex - PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex - PLAYFIELD_WIDTH);
  }

  //check if on right wall
  else if((currentIndex + 1) % PLAYFIELD_WIDTH == 0)
  {
    //check if left block valid neightbor- clear or food
    if(currentIndex - 1 == CLEAR_VALUE || currentIndex - 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex - 1);
    //check if up block valid neightbor- clear or food
    if(currentIndex + PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex + PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex + PLAYFIELD_WIDTH);
    //check if down block valid neightbor - clear or food
    if(currentIndex - PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex - PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex - PLAYFIELD_WIDTH);
  }

  //in middle
  else
  {
    //add all of the edges as long as they are valid- clear or food
    //UP
    if(currentIndex + PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex + PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex + PLAYFIELD_WIDTH);
    //LEFT
    if(currentIndex - 1 == CLEAR_VALUE || currentIndex - 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex - 1);
    //DOWN
    if(currentIndex - PLAYFIELD_WIDTH == CLEAR_VALUE || currentIndex - PLAYFIELD_WIDTH == FOOD_VALUE)
      neighborIndices.add(currentIndex - PLAYFIELD_WIDTH);
    //RIGHT
    if(currentIndex + 1 == CLEAR_VALUE || currentIndex + 1 == FOOD_VALUE)
      neighborIndices.add(currentIndex + 1);
  }

  return neighborIndices;
}
