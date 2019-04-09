#include "player.h"
#include <iostream>

Player::Player() {}

/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Searches the playfield grid for a specific value
 *
 * @param[in] grid - The playfield
 * @param[in] value - the value that's being looked for on the grid
 *
 * @return - set of coordinates for the value that's being looked for
 *
 *****************************************************************************/
std::pair<int, int> getLocation(const int *grid, int value)
{
   for (int row = 0 ; row < PLAYFIELD_HEIGHT ; row++)
      for (int column = 0 ; column < PLAYFIELD_WIDTH ; column++)
         if (grid[row * PLAYFIELD_WIDTH + column] == value)
            return std::pair<int, int>(column,row);

   return std::pair<int, int>(0,0);
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Calls functions or makes decision on how the snake should move on the board
 * Called in driver.cpp
 *
 * @param[in] pf - the playfield
 *
 * @return - the move that is to be made
 *
 *****************************************************************************/
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

  //if searching for food, do a BFS to the food

  //if not searching for food, do BFS to the the bottom right, then top right
  //corner, then top left corner, then bottom left corner. Will set searching
  //for food to false and where've you're heading to true
  //if cannot do the BFS to the walls/corners, then do manhattan path to it
  
  while(!searchingFood)
  {
    BFSPaths BFSpath(&graph, headSpot);

    //if the botom right is set to true, go to the bottom right corner
    if(toBottomRight)
    {
      std::list<int>pathToFood = BFSpath.PathTo(PLAYFIELD_WIDTH - 1);

      // go to it
      nextIndex = pathToFood.front();
      if(nextIndex == headSpot - 1)
      {
        if(nextIndex == PLAYFIELD_WIDTH - 1)
        {
          toBottomRight = false;
          toTopRight = true;
        }
        return LEFT;
      }
      //this will be index for moving right
      else if(nextIndex == headSpot + 1)
      {
        if(nextIndex == PLAYFIELD_WIDTH - 1)
        {
          toBottomRight = false;
          toTopRight = true;
        }
        return RIGHT;
      }
      //this will be the index for moving down
      else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
      {
        if(nextIndex == PLAYFIELD_WIDTH - 1)
        {
          toBottomRight = false;
          toTopRight = true;
        }
            return DOWN;
      }

      //this will be the index for moving up
      else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
      {
        if(nextIndex == PLAYFIELD_WIDTH - 1)
        {
          toBottomRight = false;
          toTopRight = true;
        }
        return UP;
      }

        //if can't do anything, do nothing
        return NONE;
    }

    if(toTopRight)
    {
      std::list<int>pathToFood = BFSpath.PathTo((PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 1);


      // go to it
      nextIndex = pathToFood.front();
      if(nextIndex == headSpot - 1)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 1)
        {
          toTopRight = false;
          toTopLeft = true;
        }
        return LEFT;
      }
      //this will be index for moving right
      else if(nextIndex == headSpot + 1)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 1)
        {
          toTopRight = false;
          toTopLeft = true;
        }
        return RIGHT;
      }
      //this will be the index for moving down
      else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 1)
        {
          toTopRight = false;
          toTopLeft = true;
        }
            return DOWN;
      }

      //this will be the index for moving up
      else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 1)
        {
          toTopRight = false;
          toTopLeft = true;
        }
        return UP;
      }

        //if can't do anything, do nothing
        return NONE;
    }

    if(toTopLeft)
    {
      std::list<int>pathToFood = BFSpath.PathTo((PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH);

      // go to it
      nextIndex = pathToFood.front();
      if(nextIndex == headSpot - 1)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        {
          toTopLeft = false;
          toBottomLeft = true;
        }
        return LEFT;
      }
      //this will be index for moving right
      else if(nextIndex == headSpot + 1)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        {
          toTopLeft = false;
          toBottomLeft = true;
        }
        return RIGHT;
      }
      //this will be the index for moving down
      else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        {
          toTopLeft = false;
          toBottomLeft = true;
        }
            return DOWN;
      }

      //this will be the index for moving up
      else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
      {
        if(nextIndex == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        {
          toTopLeft = false;
          toBottomLeft = true;
        }
        return UP;
      }

        //if can't do anything, do nothing
        return NONE;
    }

    if(toBottomLeft)
    {
      std::list<int>pathToFood = BFSpath.PathTo(0);

      // go to it
      nextIndex = pathToFood.front();
      if(nextIndex == headSpot - 1)
      {
        if(nextIndex == 0)
        {
          searchingFood = true;
          toBottomLeft = false;
        }
        return LEFT;
      }
      //this will be index for moving right
      else if(nextIndex == headSpot + 1)
      {
        if(nextIndex == 0)
        {
          searchingFood = true;
          toBottomLeft = false;
        }
        return RIGHT;
      }
      //this will be the index for moving down
      else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
      {
        if(nextIndex == 0)
        {
          searchingFood = true;
          toBottomLeft = false;
        }
            return DOWN;
      }

      //this will be the index for moving up
      else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
      {
        if(nextIndex == 0)
        {
          searchingFood = true;
          toBottomLeft = false;
        }
        return UP;
      }

        //if can't do anything, do nothing
        return NONE;
    }

  }


  //construct BFS
  BFSPaths BFSpath(&graph, headSpot);


  //get the list of spots which is the path to the food
  std::list<int>pathToFood = BFSpath.PathTo(foodSpot);


  //check if there is a path
  //if there's not a path, get the DFS
  if(!BFSpath.hasPath(foodSpot))
  {
    //CODE FOR MANHATTAN MOVE
    ValidMove newPossibleMove = NONE;
    newPossibleMove = ManhattanMove(grid);

    if(newPossibleMove == RIGHT && (grid[headSpot + 1] != CLEAR_VALUE &&
    grid[headSpot + 1] != FOOD_VALUE))
      return NONE;

    else if(newPossibleMove == LEFT && (grid[headSpot - 1] != CLEAR_VALUE &&
    grid[headSpot - 1] != FOOD_VALUE))
      return NONE;

    else if(newPossibleMove == UP && (grid[headSpot + PLAYFIELD_WIDTH] != CLEAR_VALUE &&
  grid[headSpot + PLAYFIELD_WIDTH] != FOOD_VALUE))
      return NONE;

    else if(newPossibleMove == DOWN && (grid[headSpot - PLAYFIELD_WIDTH] != CLEAR_VALUE &&
    grid[headSpot - PLAYFIELD_WIDTH] != FOOD_VALUE))
      return NONE;

    else if(newPossibleMove == NONE)
      return NONE;

    return newPossibleMove;

  }

  //go through the first item in the list and determine where to move to
  nextIndex = pathToFood.front();

  //calculate how this index relates to the head index
  //this will the be index for moving left
  //check if the headspot is right next to the foodspot
  //check to see if the next thing doing is eating food. If so, change the bools
  if(nextIndex == headSpot - 1)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      searchingFood = false;
      toBottomRight = true;
    }
    return LEFT;
  }
  //this will be index for moving right
  else if(nextIndex == headSpot + 1)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      searchingFood = false;
      toBottomRight = true;
    }
    return RIGHT;
  }
  //this will be the index for moving down
  else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      searchingFood = false;
      toBottomRight = true;
    }
        return DOWN;
  }

  //this will be the index for moving up
  else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      searchingFood = false;
      toBottomRight = true;
    }
    return UP;
  }

    //if can't do anything, do nothing
    return NONE;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * Used Dr. Hinker's code for Manhattan distace move. Will also call a function
 * that will make sure that that move can be made.
 *
 * @param[in] grid - The playfield and everything in it
 *
 * @return - the move for the Manhattan move
 *
 *****************************************************************************/
ValidMove ManhattanMove(const int *grid)
{
  ValidMove move = NONE;
  std::pair<int, int> head = getLocation(grid, HEAD_VALUE);
  std::pair<int, int> food = getLocation(grid, FOOD_VALUE);
  int dX = head.first - food.first;
  int dY = head.second - food.second;
  int headIndex =(head.second * PLAYFIELD_WIDTH) + head.first;


  //check if the player is in right column
  if(dX == 0)
  {
    //check if player is above the food
    if(dY > 0)
      move = DOWN;
    else
      move = UP;
  }
  //for when the player is not in the right column
  else
  {
    //check if player is right to the food
    if(dX > 0)
      move = LEFT;
    else
      move = RIGHT;
  }
  //call function to make sure the snake would not be killing self
  newMove(grid, move, move, 0, headIndex);

  return move;
}


/**************************************************************************//**
 * @author Jennifer Kulich with help of Dalton Baker
 *
 * @par Description:
 * Will check if the move is valid. If it is not a valid move, will try to find
 * a new valid move to make through a permutation
 *
 * @param[in] grid - The playfield with everything in it
 * @param[in,out] move - the move that will be made
 * @param[in]  origionalMove  - the origional move
 * @param[in] count - how many times we've tried to make a different move
 * @param[in] headIndex - where the head of the snake is located
 *
 *****************************************************************************/
void newMove(const int *grid, ValidMove &move, ValidMove origionalMove, int count, int headIndex)
{
  //function will take in a move, if the move cannot be made, a new move and
  //call this function again to see if that move can be made
  //if this is the first check, set the trialMove to the origional  move
  if(count == 0)
    origionalMove = move;

  //if the origional move passd in was valid, don't do anything and return
  if(count > 0 && move == origionalMove)
  {
    move = NONE;
    return;
  }

  count = count + 1;
  //if the right move cannot be made, try moving down and make sure it would
  //work
  //check to make sure right is not clear or on far right wall
  if((move == RIGHT) && (grid[headIndex + 1] != CLEAR_VALUE ||
    (headIndex + 1) == PLAYFIELD_WIDTH - 1))
  {
    move = DOWN;
    newMove(grid, move, origionalMove, count, headIndex);
  }

  //if the down move cannot be made, try moving left and make sure it would
  //work
  else if((move == DOWN) && (grid[headIndex - PLAYFIELD_WIDTH] != CLEAR_VALUE ||
    (headIndex <= PLAYFIELD_WIDTH - 1)))
  {
    move = LEFT;
    newMove(grid, move, origionalMove,count,headIndex);
  }

  //if the LEFT move cannot be made, try moving up and make sure it would work
  else if((move == LEFT) && (grid[headIndex - 1] != CLEAR_VALUE ||
  (headIndex % PLAYFIELD_WIDTH) == 0))
  {
    move = UP;
    newMove(grid,move,origionalMove,count,headIndex);
  }


  //jf the UP move cannot be made, try moving right and make sure it would work
  else if((move == UP) && (grid[headIndex + PLAYFIELD_WIDTH] != CLEAR_VALUE ||
((headIndex / PLAYFIELD_WIDTH) == (PLAYFIELD_HEIGHT - 1))))
  {
    move = RIGHT;
    newMove(grid,move,origionalMove,count,headIndex);
  }

}
