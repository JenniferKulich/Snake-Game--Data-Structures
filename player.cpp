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

//try bfs





  //calculate in the one-d array where the head is at
  //index = row * width + column
  //int index = (head.first * PLAYFIELD_WIDTH) + head.second;
  int nextIndex = 0;
  int headSpot =(head.second * PLAYFIELD_WIDTH) + head.first;
  int foodSpot = (food.second * PLAYFIELD_WIDTH) + food.first;

  //construct a graph for BFS
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  //construct BFS
  BFSPaths path(&graph, headSpot);


  //get the list of spots which is the path to the food
  std::list<int>pathToFood = path.PathTo(foodSpot);
  std::cout << "recalculated path" << std::endl;

  //go through the first item in the list and determine where to move to
  nextIndex = pathToFood.front();

  //calculate how this index relates to the head index
  //this will the be index for moving left
  std::cout << "Headspot: " << headSpot << std::endl;
  std::cout << "FoodSpot: " << foodSpot << std::endl;
  std::cout << "nexIndex: " << nextIndex << std::endl;
  //check if the headspot is right next to the foodspot
  if(foodSpot == headSpot - 1)
  {
    std::cout << "NOM" << std::endl;
    return LEFT;
  }
  else if(foodSpot == headSpot + 1)
  {
    std::cout << "NOM" << std::endl;
    return RIGHT;
  }
   else if(foodSpot == headSpot + PLAYFIELD_WIDTH)
   {
     std::cout << "NOM" << std::endl;
     return UP;
   }
  else if(foodSpot == headSpot - PLAYFIELD_WIDTH)
  {
    std::cout << "NOM" << std::endl;
    return DOWN;
  }
  else if(nextIndex == headSpot - 1)
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

  std::cout << "Done" << std::endl << std::endl;


    //if can't do anything, just go down
    return DOWN;


   //just trying to move the snake
   //return DOWN;


/*

   //int dX = head.first - food.first;
   //int dY = head.second - food.second;

   //try the strategy where the snake goes around edge of board then then
   //zig-zag ignore obstacles!!
   //will go down left side of board and then zig zag up- only works on even
   //number of rows and no obstacles

   //if there an odd number of rows, do the zig zag thing but skip the row
   //before the food

    //check to see if rows (height) is odd
    if(PLAYFIELD_HEIGHT % 2 != 0)
        return rowsOdd(head.first, head.second, food.first, food.second);

    //check to see if rows (height) is even
   if(PLAYFIELD_HEIGHT % 2 == 0)
    return rowsEven(head.first, head.second);


  //#define PLAYFIELD_WIDTH 17
  //#define PLAYFIELD_HEIGHT 15
*/

}


ValidMove Player::rowsEven(int headX, int headY)
{
  //zig-zag ignore obstacles!!
  //will go down left side of board and then zig zag up- only works on even
  //number of rows and no obstacles

  //check to see if snake on top row and not in top left corner
  if(headY == PLAYFIELD_HEIGHT - 1 && headX != 0)
    return LEFT;
 //check to see if snake in the bottom row
 if(headY == 0  && headX != PLAYFIELD_WIDTH - 1)
   return RIGHT;
  //check to see if in middle of play field
  if(headX > 0 && headY > 0)
  {
    //if on spot (--,1), move up
    //if on spot (--,PLAYFIELD_WIDTH), move up
    if(headX == 1 || headX == PLAYFIELD_WIDTH - 1)
    {
      //check to see if it should be moving Left
      if(headX == PLAYFIELD_WIDTH - 1 && headY % 2 != 0)
       return LEFT;
     //check if should be moving right
     if(headX == 1 && headY % 2 == 0)
       return RIGHT;
      return UP;
    }

    //if on an odd row, be moving left
    if(headY % 2 != 0)
      return LEFT;
    //if on even row, be moving right
    if(headY % 2 == 0)
      return RIGHT;
  }

    //start  by moving to top left corner
    //check if starting on left side
    if(headX == 0 && headY > 0)
         return DOWN;

   //check to see if the snake is in the bottom left corner
    if(headX == 0 && headY == 0)
         return RIGHT;

   //check to see if the snake is in the bottom right corner
   if(headX == PLAYFIELD_WIDTH - 1 && headY == 0)
         return UP;

    return RIGHT;
}



ValidMove Player::rowsOdd(int headX, int headY, int foodX, int foodY)
{
  //check to see if snake on top row and not in top left corner
  if(headY == PLAYFIELD_HEIGHT - 1 && headX != 0)
    return LEFT;
 //check to see if snake in the bottom row
 if(headY == 0  && headX != PLAYFIELD_WIDTH - 1)
   return RIGHT;
  //check to see if in middle of play field
  if(headX > 0 && headY > 0)
  {
    //if on spot (--,1), move up
    //if on spot (--,PLAYFIELD_WIDTH), move up
    if(headX == 1 || headX == PLAYFIELD_WIDTH - 1)
    {

      //if there's food in second from top row, skip the fourth from top
      if(foodY == PLAYFIELD_HEIGHT - 2 && headY == PLAYFIELD_HEIGHT - 4)
        return UP;

      //if food isn't in second from top and head there, skip it
      if(foodY != PLAYFIELD_HEIGHT - 2 && headY == PLAYFIELD_HEIGHT - 2)
        return UP;
      //check to see if it should be moving Left
      if(headX == PLAYFIELD_WIDTH - 1 && headY % 2 != 0)
       return LEFT;
     //check if should be moving right
     if(headX == 1 && headY % 2 == 0)
       return RIGHT;
      return UP;
    }

    //if on an odd row, be moving left
    if(headY % 2 != 0)
      return LEFT;
    //if on even row, be moving right
    if(headY % 2 == 0)
      return RIGHT;
  }

    //start  by moving to top left corner
    //check if starting on left side
    if(headX == 0 && headY > 0)
         return DOWN;

   //check to see if the snake is in the bottom left corner
    if(headX == 0 && headY == 0)
         return RIGHT;

   //check to see if the snake is in the bottom right corner
   if(headX == PLAYFIELD_WIDTH - 1 && headY == 0)
         return UP;

    return RIGHT;
}
