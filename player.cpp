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

    //CODE FOR RANDOM MOVE
    //if no move can be made, pick a random move
    /*
    moveToMake = randomMove(grid, headSpot);
    if(moveToMake == 0)
      return UP;
    if(moveToMake == 1)
      return DOWN;
    if(moveToMake == 2)
      return LEFT;
    if(moveToMake == 3)
      return RIGHT;
    if(moveToMake == 5)
      return NONE;
    */

    //CODE FOR MANHATTAN MOVE
    std::cout << "Making Manhattan Move" <<std::endl;
    return ManhattanMove(grid);
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
    std::cout << "Core Dump?"<< std::endl;
    return NONE;
}


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
    //std::cout<<"Made it to random move 2" <<std::endl;
    int randNum = (rand() % 4);
    //std::cout<<"Made it to random move 3" <<std::endl;
    if(randNum == 0)
    {
      if(grid[headSpot + PLAYFIELD_WIDTH] == CLEAR_VALUE)
      {
        canMove = true;
        return 0;
      }
    }

    else if(randNum == 1)
    {
      if(grid[headSpot - PLAYFIELD_WIDTH] == CLEAR_VALUE)
      {
        canMove = true;
        return 1;

      }
    }
    else if(randNum == 2)
    {
      if(grid[headSpot - 1] == CLEAR_VALUE)
      {
        canMove = true;
        return 2;
      }
    }
    else if(randNum == 3)
    {
      if(grid[headSpot + 1] == CLEAR_VALUE)
      {
        canMove = true;
        return 3;
      }
    }
  }

  return 5;
}

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

  std::cout << "Move: " << move << std::endl;
  return move;
}

void newMove(const int *grid, ValidMove &move, ValidMove trialMove, int count, int headIndex)
{
  //function will take in a move, if the move cannot be made, a new move and
  //call this function again to see if that move can be made

  //if this is the first check, set the trialMove to the origional  move
  if(count == 0)
    trialMove = move;

  //if the origional move passd in was valid, don't do anything and return
  if(count > 0 && move == trialMove)
    return;

  count = count + 1;

  //if the right move cannot be made, try moving down and make sure it would
  //work
  //check to make sure right is not clear or on far right wall
  if((move == RIGHT) && (grid[headIndex + 1] % 4 != 0/*grid[headIndex + 1] != CLEAR_VALUE */ ||
    (headIndex + 1) % PLAYFIELD_WIDTH == 0))
  {
    move = DOWN;
    newMove(grid, move, trialMove, count, headIndex);
  }

  //if the down move cannot be made, try moving left and make sure it would
  //work
  else if((move == DOWN) && (grid[headIndex - PLAYFIELD_WIDTH] % 4 != 0/*grid[headIndex - PLAYFIELD_WIDTH] != CLEAR_VALUE*/ ||
    (headIndex / PLAYFIELD_WIDTH) == 0))
  {
    move = LEFT;
    newMove(grid, move, trialMove,count,headIndex);
  }

  //if the LEFT move cannot be made, try moving up and make sure it would work
  else if((move == LEFT) && (grid[headIndex - 1] % 4 != 0/*grid[headIndex - 1] != CLEAR_VALUE*/ ||
  (headIndex % PLAYFIELD_WIDTH) == 0))
  {
    move = UP;
    newMove(grid,move,trialMove,count,headIndex);
  }

  //jf the UP move cannot be made, try moving right and make sure it would work
  else if((move == UP) && (grid[headIndex + PLAYFIELD_WIDTH] % 4 != 0/*grid[headIndex + PLAYFIELD_WIDTH] != CLEAR_VALUE*/ ||
(headIndex / PLAYFIELD_WIDTH) == (PLAYFIELD_HEIGHT - 1)))
  {
    move = RIGHT;
    newMove(grid,move,trialMove,count,headIndex);
  }
}
