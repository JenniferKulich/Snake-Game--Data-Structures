#include "player.h"
#include <iostream>

Player::Player() {}

// Searches the playfield grid for a specific value
std::pair<int, int> getLocation(const int *grid, int value)
{
   for (int i = 0 ; i < PLAYFIELD_HEIGHT ; i++)
      for (int j = 0 ; j < PLAYFIELD_WIDTH ; j++)
         if (grid[i * PLAYFIELD_WIDTH + j] == value)
            return std::pair<int, int>(j,i);

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
  /*
  BFSPath(Graph *G, int s);
  void bfs(Graph *G, int s);
  bool hasPath(int v);
  std::list<int> PathTo(int v);
  int Distance(int v);

  */

  //calculate in the one-d array where the head is at
  //index = row * width + column
  //int index = (head.first * PLAYFIELD_WIDTH) + head.second;
  int nextIndex = 0;
  int headSpot =(head.first * PLAYFIELD_WIDTH) + head.second;
  int foodSpot = (food.first * PLAYFIELD_WIDTH) + food.second;
  //construct BFS
  BFSPaths path(grid, headSpot);
  //get the list of spots which is the path to the food
  std::list<int>pathToFood = path.PathTo(foodSpot);
  //go through the first item in the list and determine where to move to
  nextIndex = pathToFood.front();
  //calculate how this index relates to the head index
  //this will the be index for moving left
  if(nextIndex == headSpot - 1)
      return LEFT;
  //this will be index for moving right
  if(nextIndex == headSpot + 1)
    return RIGHT;
  //this will be the index for moving up
  if(nextIndex == ((head.first * PLAYFIELD_WIDTH) + (head.second + 1)))
    return UP;
  //this will be the index for moving down
  if(nextIndex == ((head.first * PLAYFIELD_WIDTH) + (head.second - 1)))
    return DOWN;



    //if can't do anything, just go down
    return DOWN;


   //just trying to move the snake
   //return DOWN;

   //int dX = head.first - food.first;
   //int dY = head.second - food.second;

   //try the strategy where the snake goes around edge of board then then
   //zig-zag ignore obstacles!!
   //will go down left side of board and then zig zag up- only works on even
   //number of rows and no obstacles

   //if there an odd number of rows, do the zig zag thing but skip the row
   //before the food
/*
    //check to see if rows (height) is odd
    if(PLAYFIELD_HEIGHT % 2 != 0)
        return rowsOdd(head.first, head.second, food.first, food.second);

    //check to see if rows (height) is even
   if(PLAYFIELD_HEIGHT % 2 == 0)
    return rowsEven(head.first, head.second);

*/
  //#define PLAYFIELD_WIDTH 17
  //#define PLAYFIELD_HEIGHT 15

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
