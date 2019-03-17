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
ValidMove Player::makeMove(const Playfield *pf)
{
   const int *grid = pf->getGrid();
   std::pair<int, int> head = getLocation(grid, HEAD_VALUE);
   std::pair<int, int> food = getLocation(grid, FOOD_VALUE);

   int dX = head.first - food.first;
   int dY = head.second - food.second;

   if (dX == 0) // In the right column
   {
      if (dY > 0) // Above the food
         return DOWN;
      else
         return UP;
   }
   else // Not in the right column
   {
      if (dX > 0) // I'm to the right of the food
         return LEFT;
      else  // I'm to the left of the food
         return RIGHT;
   }      
   return NONE;
}
