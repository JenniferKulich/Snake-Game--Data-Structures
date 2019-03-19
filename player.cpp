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

   //just trying to move the snake
   //return DOWN;

   int dX = head.first - food.first;
   int dY = head.second - food.second;

   //try the strategy where the snake goes around edge of board then then
   //zig-zag ignore obstacles!!
   //will go down left side of board and then zig zag up- only works on even
   //number of rows and no obstacles

   //if there an odd number of rows, do the zig zag thing but skip the row
   //before the food

    //check to see if rows (height) is odd
    if(PLAYFIELD_HEIGHT % 2 != 0)
    {
        return rowsOdd(head.first, head.second, food.first, food.second);

    }
    //check to see if rows (height) is even
   if(PLAYFIELD_HEIGHT % 2 == 0)
   {
     /*
     std::cout << "(" << head.first << " , " << head.second << ")" << std::endl;
     //check to see if snake on top row and not in top left corner
     if(head.second == PLAYFIELD_HEIGHT - 1 && head.first != 0)
       return LEFT;
    //check to see if snake in the bottom row
    if(head.second == 0  && head.first != PLAYFIELD_WIDTH - 1)
      return RIGHT;
     //check to see if in middle of play field
     if(head.first > 0 && head.second > 0)
     {
       //if on spot (--,1), move up
       //if on spot (--,PLAYFIELD_WIDTH), move up
       if(head.first == 1 || head.first == PLAYFIELD_WIDTH - 1)
       {
         //check to see if it should be moving Left
         if(head.first == PLAYFIELD_WIDTH - 1 && head.second % 2 != 0)
          return LEFT;
        //check if should be moving right
        if(head.first == 1 && head.second % 2 == 0)
          return RIGHT;
         return UP;
       }

       //if on an odd row, be moving left
       if(head.second % 2 != 0)
         return LEFT;
       //if on even row, be moving right
       if(head.second % 2 == 0)
         return RIGHT;
     }

       //start  by moving to top left corner
       //check if starting on left side
       if(head.first == 0 && head.second > 0)
            return DOWN;

      //check to see if the snake is in the bottom left corner
       if(head.first == 0 && head.second == 0)
            return RIGHT;

      //check to see if the snake is in the bottom right corner
      if(head.first == PLAYFIELD_WIDTH - 1 && head.second == 0)
      {
        std::cout << "Here" << std::endl;
        return UP;
      }
            //return UP;
*/
//std::cout<<"Before Call." << std::endl;
//std::cout <<"(" << head.first << " , " << head.second << ")" << std::endl;

    return rowsEven(head.first, head.second);

   }



  //#define PLAYFIELD_WIDTH 17
  //#define PLAYFIELD_HEIGHT 15




/*
//TODO: check if there are obstacles and move
   if (dX == 0) // In the right column
   {
      if (dY > 0) // Above the food
      {
         //check if moving DOWN would move it onto itself
         if(grid[head.second + 1] != 0)
            return LEFT;
         else
            return DOWN;
      }
         //return DOWN;
      else
      {
         if(grid[head.second - 1] != 0)
            return LEFT;
         else
            return UP;
      }
        // return UP;
   }
   else // Not in the right column
   {
      if (dX > 0) // I'm to the right of the food
      {
         //check if moving LEFT would moe it onto itself
         if(grid[head.first - 1] != 0)
            return UP;
         else
            return LEFT;
      }
         //return LEFT;
      else  // I'm to the left of the food
      {
         if(grid[head.first + 1] != 0)
            return UP;
         else
            return RIGHT;
      }
         //return RIGHT;
   }
   return NONE;
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
}



ValidMove Player::rowsOdd(int headX, int headY, int foodX, int foodY)
{


    //check to see if the food isn't on an outside edge. If it is, we'll
    //get it on the pass around, but the second to top row
  //  if(foodX )


}
