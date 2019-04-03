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

  //construct BFS
  BFSPaths BFSpath(&graph, headSpot);

  //get the list of spots which is the path to the food
  std::list<int>pathToFood = BFSpath.PathTo(foodSpot);


  //check if there is a path
  //if there's not a path, get the DFS
  if(!BFSpath.hasPath(foodSpot))
  {
        std::cout << "No Path" << std::endl;

    //CODE FOR MANHATTAN MOVE
    ValidMove newPossibleMove = NONE;
    newPossibleMove = ManhattanMove(grid);
    std::cout << "Possible Move 1: " << newPossibleMove << std::endl;
/*
    if(newPossibleMove == RIGHT && (grid[headSpot + 1] != CLEAR_VALUE &&
    grid[headSpot + 1] != FOOD_VALUE))
    {
      std::cout << "Should move RIGHT but doesn't" << std::endl;
      std::cout << "grid[headSpot + 1]: " << grid[headSpot + 1] << std::endl;
      return NONE;
    }
    if(newPossibleMove == LEFT && (grid[headSpot - 1] != CLEAR_VALUE &&
    grid[headSpot - 1] != FOOD_VALUE))
    {
      std::cout << "Should move LEFT but doesn't" << std::endl;
      std::cout << "grid[headSpot - 1]: " << grid[headSpot - 1] << std::endl;
      return NONE;
    }
    if(newPossibleMove == UP && (grid[headSpot + PLAYFIELD_WIDTH] != CLEAR_VALUE &&
  grid[headSpot + PLAYFIELD_WIDTH] != FOOD_VALUE))
  {
      std::cout << "Should move UP but doesn't" << std::endl;
      std::cout << "grid[headSpot + PLAYFIELD_WIDTH]: " << grid[headSpot + PLAYFIELD_WIDTH] << std::endl;
      return NONE;

  }
    if(newPossibleMove == DOWN && (grid[headSpot - PLAYFIELD_WIDTH] != CLEAR_VALUE &&
    grid[headSpot - PLAYFIELD_WIDTH] != FOOD_VALUE))
    {
      std::cout << "Should move DOWN but doesn't" << std::endl;
     std::cout << "grid[headSpot -PLAYFIELD_WIDTH]: " << grid[headSpot - PLAYFIELD_WIDTH] << std::endl;
      return NONE;

    }
*/
    std::cout << "Possible Move 2: " << newPossibleMove << std::endl;
    return newPossibleMove;
  }

  //go through the first item in the list and determine where to move to
  nextIndex = pathToFood.front();

  //calculate how this index relates to the head index
  //this will the be index for moving left
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
  std::cout << "Manhattan wants to move: " << move << std::endl;
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
  if((move == RIGHT) && ((grid[headIndex + 1] != CLEAR_VALUE && grid[headIndex + 1] != FOOD_VALUE) ||
    (headIndex + 1) == PLAYFIELD_WIDTH - 1))
  {
    move = DOWN;
    newMove(grid, move, origionalMove, count, headIndex);
  }

  //if the down move cannot be made, try moving left and make sure it would
  //work
  else if((move == DOWN) && ((grid[headIndex - PLAYFIELD_WIDTH] != CLEAR_VALUE && grid[headIndex - PLAYFIELD_WIDTH] != FOOD_VALUE)||
    (headIndex <= PLAYFIELD_WIDTH - 1)))
  {
    move = LEFT;
    newMove(grid, move, origionalMove,count,headIndex);
  }

  //if the LEFT move cannot be made, try moving up and make sure it would work
  else if((move == LEFT) && ((grid[headIndex - 1] != CLEAR_VALUE && grid[headIndex - 1] != FOOD_VALUE ) ||
  (headIndex % PLAYFIELD_WIDTH) == 0))
  {
    move = UP;
    newMove(grid,move,origionalMove,count,headIndex);
  }

  //jf the UP move cannot be made, try moving right and make sure it would work
  else if((move == UP) && ((grid[headIndex + PLAYFIELD_WIDTH] != CLEAR_VALUE && grid[headIndex + PLAYFIELD_WIDTH] != FOOD_VALUE)||
(headIndex > ((PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - PLAYFIELD_WIDTH))))
  {
    move = RIGHT;
    newMove(grid,move,origionalMove,count,headIndex);
  }

}
