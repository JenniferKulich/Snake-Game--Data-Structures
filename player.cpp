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
   
}
