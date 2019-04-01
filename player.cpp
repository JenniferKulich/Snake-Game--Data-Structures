#include "player.h"
#include <iostream>

Player::Player() {}

// Searches the playfield grid for a specific value
std::pair<int, int> getLocation(const int *grid, int value)
{
   for (int row = 0 ; row < PLAYFIELD_HEIGHT ; row++)
      for (int column = 0 ; column < PLAYFIELD_WIDTH ; column++)
         if (grid[row * PLAYFIELD_WIDTH + column] == value)
            return std::pair<int, int>(column,row);

   return std::pair<int, int>(0,0);
}

// Make a move that approaches the food.  Does not try to avoid
// obstacles (like its own tail)
//called in driver.cpp
ValidMove Player::makeMove(const Playfield *pf)
{
   const int *grid = pf->getGrid();
   std::pair<int, int> head = getLocation(grid, HEAD_VALUE);
   std::pair<int, int> food = getLocation(grid, FOOD_VALUE);

  //calculate in the one-d array where the head is at
  //index = row * width + column
  //int index = (head.first * PLAYFIELD_WIDTH) + head.second;
  int nextIndex = 0;
  int headSpot =(head.second * PLAYFIELD_WIDTH) + head.first;
  int foodSpot = (food.second * PLAYFIELD_WIDTH) + food.first;


  //construct a graph
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  //construct BFS
  BFSPaths BFSpath(&graph, headSpot);

  //get the list of spots which is the path to the food
  std::list<int>pathToFood = BFSpath.PathTo(foodSpot);


  //check if there is a path
  //if there's not a path, get the DFS
  if(!BFSpath.hasPath(foodSpot))
  {
    DFSPaths DFSpath(&graph, headSpot);
    pathToFood = DFSpath.PathTo(foodSpot);
  }

  //go through the first item in the list and determine where to move to
  nextIndex = pathToFood.front();

  //calculate how this index relates to the head index
  //this will the be index for moving left
  /*
  std::cout << "Headspot: " << headSpot << std::endl;
  std::cout << "FoodSpot: " << foodSpot << std::endl;
  std::cout << "nexIndex: " << nextIndex << std::endl;
  */
  //check if the headspot is right next to the foodspot
  if(nextIndex == headSpot - 1)
    return LEFT;
  //this will be index for moving right
  else if(nextIndex == headSpot + 1)
    return RIGHT;
  //this will be the index for moving up
  else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
    return DOWN;
  //this will be the index for moving down
  else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
    return UP;



    //if can't do anything, just go down
    return NONE;
}
