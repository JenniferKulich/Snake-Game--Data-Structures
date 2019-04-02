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


int moveToMake = 0;
  //check if there is a path
  //if there's not a path, get the DFS
  if(!BFSpath.hasPath(foodSpot))
  {
    /*
    std::cout << "In DFS" << std::endl;
    DFSPaths DFSpath(&graph, headSpot);
    pathToFood = DFSpath.PathTo(foodSpot);
    if(!DFSpath.hasPathTo(foodSpot))
      std::cout << "No path anywhere." << std::endl;
      */
    std::cout << "Going to pick a random move" << std::endl;
    //if no move can be made, pick a random move
    moveToMake = randomMove(grid, headSpot);
    if(moveToMake == 0)
      return UP;
    if(moveToMake == 1)
        return DOWN;
    if(moveToMake == 2)
        return LEFT;
    if(moveToMake == 3)
        return RIGHT;




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
  //this will be the index for moving down
  else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
    return DOWN;
  //this will be the index for moving up
  else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
    return UP;

    //if can't do anything, just go down
    return NONE;
}

/*
//this function will find where the tail is located so a potential path
//could be found to the tail to loiter
std::pair<int, int> tailLocation(const int *grid)
{
  int numAttatchments = 0;
  //#define TAIL_VALUE 1
  //loop throught the grid to find tail points. It will be the end of the
  //tail if it only has one attatchment.
  //to check for attatchments, check the up, down, right, and left squares
  //make sure that the place you're checking is not an edge or corner.
  for (int row = 0 ; row < PLAYFIELD_HEIGHT ; row++)
  {
    for (int column = 0 ; column < PLAYFIELD_WIDTH ; column++)
    {
      if (grid[row * PLAYFIELD_WIDTH + column] == TAIL_VALUE)
      {
        //if in bottom left corner, check if another tail up or right
        if(row == 0 && column == 0)
        {
          //check up
          if(grid[((row + 1) * PLAYFIELD_WIDTH + column)] == TAIL_VALUE)
            numAttatchments = numAttatchments + 1;
          //check right

        }
      }
         return std::pair<int, int>(column,row);
    }

  }

  return std::pair<int, int>(0,0);

  //corner cases
  //if on top edge, only check right,left, down
  //if on bottom edge, only check right,left, up
  //if on left edge, only right, up ,down
  //if on right edge, only ceck left, up,down
  //check the middle of the board
}
*/

//function that will have the snake move around randomly until it can find a
//path to the food
int randomMove(const int *grid, int headSpot)
{
  //pick a random move if it is valid
  bool canMove = false;
  srand(time(NULL));
  //0 = up
  //1 = down
  //2 = left
  //3 = right

  //check if that is a valid move
  //while a valid move can be made, return it
  while(canMove == false)
  {
    int randNum = (rand() % 4);

    if(randNum == 0)
    {
      if(headSpot + PLAYFIELD_WIDTH == CLEAR_VALUE)
      {
        canMove = true;
        return 0;
      }
    }

    else if(randNum == 1)
    {
      if(headSpot - PLAYFIELD_WIDTH == CLEAR_VALUE)
      {
        canMove = true;
        return 1;

      }
    }
    else if(randNum == 2)
    {
      if(headSpot - 1 == CLEAR_VALUE)
      {
        canMove = true;
        return 2;
      }
    }
    else if(randNum == 3)
    {
      if(headSpot + 1 == CLEAR_VALUE)
      {
        canMove = true;
        return 3;
      }
    }

  }

}
