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

  //if the food eaten (aka tail length) is less than the smaller of the width
  //and height of the board, just do BFS. Once gets length, won't do BFS
  int smallest;
  if(PLAYFIELD_WIDTH < PLAYFIELD_HEIGHT)
    smallest = PLAYFIELD_WIDTH;
  else
    smallest = PLAYFIELD_HEIGHT;

  if(foodEaten < smallest)
  {
    startingBFS = true;
    searchingFood = false;
  }
  else
    startingBFS = false;
  if(foodEaten == smallest)
  {
    searchingFood = true;
  }
  //if not searching for food, do BFS to the the bottom right, then top right
  //corner, then top left corner, then bottom left corner. Will set searching
  //for food to false and where've you're heading to true
  //if cannot do the BFS to the walls/corners, then do manhattan path to it
  bool contin = false;
  ValidMove nonSearchingMove;
  while(!searchingFood && !startingBFS)
  {
    BFSPaths BFSpath(&graph, headSpot);

    //if the botom right is set to true, go to the bottom right corner
    contin = false;
    if(toBottomRight)
    {
      nonSearchingMove= moveRightSide(grid,headSpot, contin);
      if(contin == true)
        continue;
      else
        return nonSearchingMove;
    }

    contin = false;
    if(toTopRight)
    {
      nonSearchingMove= moveTopRight(grid,headSpot, contin);
      if(contin == true)
        continue;
      else
        return nonSearchingMove;
    }

    contin = false;

    if(toTopLeft)
    {
      nonSearchingMove= moveTopLeft(grid,headSpot, contin);
      if(contin == true)
        continue;
      else
        return nonSearchingMove;
    }

    contin = false;

    if(toBottomLeft)
    {
      nonSearchingMove= moveBottomLeft(grid,headSpot, contin);
      if(contin == true)
        continue;
      else
        return nonSearchingMove;
    }
  }

  //check if there is food in the second to top row
  int foodrow = food.second;
  while(searchingFood && !startingBFS && (foodrow == PLAYFIELD_HEIGHT - 2 && (grid[foodSpot + 2 + PLAYFIELD_WIDTH] != CLEAR_VALUE
  || grid[foodSpot + 1 + PLAYFIELD_WIDTH] != CLEAR_VALUE || grid[foodSpot + PLAYFIELD_WIDTH != CLEAR_VALUE]
|| grid[foodSpot - 1 + PLAYFIELD_WIDTH] != CLEAR_VALUE)))
  {
    //if it's not 2 away, just go right to the right
    if(grid[foodSpot + 2 + PLAYFIELD_WIDTH] == CLEAR_VALUE)
    {
      //go to the right
      searchingFood = false;
      toBottomRight = true;
    }
    else
    {
      searchingFood = false;
      toBottomRight = true;
      foodSecondTop = true;
    }
  }
  //construct BFS
  BFSPaths BFSpath(&graph, headSpot);


  //get the list of spots which is the path to the food
  std::list<int>pathToFood = BFSpath.PathTo(foodSpot);


  //check if there is a path
  //if there's not a path, do the Manhattan move
  if(!BFSpath.hasPath(foodSpot))
    return ManhattanChecker(grid, headSpot);

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
      foodEaten +=1;
      searchingFood = false;

      //check if food it's in top row, if it is, set toTopLeft to true
      //check if food is on left wall, if it is, set toBottomLeft to true
      if(nextIndex <= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) &&
      nextIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        toTopLeft = true;
      else if(foodSpot % PLAYFIELD_WIDTH == 0 &&
        (nextIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1 &&
        nextIndex >= 0))
        toBottomLeft = true;
      else
        toBottomRight = true;
    }
    return LEFT;
  }
  //this will be index for moving right
  else if(nextIndex == headSpot + 1)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      foodEaten +=1;

      searchingFood = false;
      if(nextIndex <= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) &&
      nextIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        toTopLeft = true;
      else if(foodSpot % PLAYFIELD_WIDTH == 0
        && (nextIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1 &&
        nextIndex >= 0))
          toBottomLeft = true;
      else
        toBottomRight = true;
    }
    return RIGHT;
  }
  //this will be the index for moving down
  else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      foodEaten +=1;

      searchingFood = false;
      if(nextIndex <= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) &&
      nextIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        toTopLeft = true;
      else if(foodSpot % PLAYFIELD_WIDTH == 0 &&
        (nextIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1 &&
        nextIndex >= 0))
          toBottomLeft = true;
      else
        toBottomRight = true;
    }
        return DOWN;
  }

  //this will be the index for moving up
  else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
  {
    if(grid[nextIndex] == FOOD_VALUE)
    {
      foodEaten +=1;

      searchingFood = false;
      if(nextIndex <= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) &&
      nextIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
        toTopLeft = true;
      else if(foodSpot % PLAYFIELD_WIDTH == 0 &&
        (nextIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1 &&
        nextIndex >= 0))
          toBottomLeft = true;
      else
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
ValidMove Player::ManhattanMove(const int *grid)
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
void Player::newMove(const int *grid, ValidMove &move, ValidMove origionalMove, int count, int headIndex)
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

/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will help the snake find its way to the right side of the board based
 * on where it currently is. It will try to stay in the same row if it can.
 * Does check to make sure that the snake will no be trapping itself.
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] playerIndex - the index of where the snake is
 *
 * @return - the index of where the snake should go to or a very large number
 *           meaning that it cannot go to the right side of the board
 *
 *****************************************************************************/
int Player::toRightSide(const int *grid, int playerIndex)
{
  //get the right most index based in row you're on
  int mostRightIndex;
  int trialIndex = playerIndex;

  //loop through everything in the row until get to
  while(trialIndex % PLAYFIELD_WIDTH != 0)
    trialIndex += 1;

  mostRightIndex = trialIndex - 1;

  //check if that spot is open and the spot above it is open
  if((grid[mostRightIndex] == CLEAR_VALUE || grid[mostRightIndex] == FOOD_VALUE)
  && (grid[mostRightIndex + PLAYFIELD_WIDTH] == CLEAR_VALUE ||
     grid[mostRightIndex + PLAYFIELD_WIDTH] == FOOD_VALUE))
    return mostRightIndex;

  //if that spot is not open, go up in the board until hit the corner - 1
  for(int i = mostRightIndex + PLAYFIELD_WIDTH;
     i < (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH;
      i += PLAYFIELD_WIDTH)
  {
    if((grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE) &&
    (grid[i + PLAYFIELD_WIDTH] == CLEAR_VALUE
      || grid[i + PLAYFIELD_WIDTH] == FOOD_VALUE))
      return i;
  }

  //if cannot do that, start down at the bottom
  for(int i = PLAYFIELD_WIDTH - 1; i < mostRightIndex; i += PLAYFIELD_WIDTH)
  {
    if(grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE)
      return i;
  }

  //return massive number if got nothing
  return 20000;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will go through top row and find a new corner point for the snake if the
 * actual corner is filled up
 *
 * @param[in] grid - The playfield and everything in it
 *
 * @return - the index of the first spot on the top row that's open, a 1 if
 *           nothing is open.
 *
 *****************************************************************************/
int Player::newTopRightCorner(const int *grid)
{
  //start at width * height - 2 and go until width * height - width + 1
  for(int i = (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 2;
  i > (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH + 1; i --)
  {
    if(grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE)
      return i;
  }

  //if can't do that, return 1 which means that will just go to next corner
  return 1;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will go through the left column and find a new corner point for the
 * snake if the actual corner is filled up
 *
 * @param[in] grid - The playfield and everything in it
 *
 * @return - the index of the first spot on the left column that's open, a 1 if
 *           nothing is open
 *
 *****************************************************************************/
int Player::newTopLeftCorner(const int *grid)
{
  //start at width * height) - width )- width  greater than width i--
  for(int i = ((PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - (2 * PLAYFIELD_WIDTH));
   i > PLAYFIELD_WIDTH; i--)
  {
    if(grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE)
      return i;
  }

  //if can't do that, return 1 which means that will just go to next corner
  return 1;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will go through the bottom row and find a new corner point for the
 * snake if the actual corner is filled up
 *
 * @param[in] grid - The playfield and everything in it
 *
 * @return - the index of the first spot on the bottom row that's open, a 1 if
 *           nothing is open
 *
 *****************************************************************************/
int Player::newBottomLeftCorner(const int *grid)
{
  //start at 1 and go until (width - 1) - 1 i++
  for(int i = 1; i < PLAYFIELD_WIDTH - 2; i++)
  {
    if(grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE)
      return i;
  }

  //if can't do that, return really big number  which means that will just go to the food
  return 20000;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will have the snake attempt to move to the right side of the board. If
 * it cannot find a path, it will do the Manhattan move
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] headSpot - the head index of the snake
 * @param[in,out] contin - if there is no new corner, continue
 *
 * @return - the move the snake needs to make to get to the right side of the
 *           board
 *
 *****************************************************************************/
ValidMove Player::moveRightSide(const int *grid, int headSpot, bool &contin)
{
  int rightMoveTo;
  int nextIndex;
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  BFSPaths BFSpath(&graph, headSpot);

  //first need to try going to the very right side

  rightMoveTo = toRightSide(grid, headSpot);
  //check if less than index, if it is, just go to the next corner
  if(rightMoveTo < headSpot || rightMoveTo == 20000)
  {
    toBottomRight = false;
    toTopRight = true;
    contin = true;
    return NONE;
  }

  std::list<int>pathToFood = BFSpath.PathTo(rightMoveTo);

  //check if there is a path
  //if there's not a path, do the Manhattan move
  if(!BFSpath.hasPath(rightMoveTo))
    return ManhattanChecker(grid, headSpot);


  // go to it
  nextIndex = pathToFood.front();
  if(nextIndex == rightMoveTo)
  {
    toBottomRight = false;
    toTopRight = true;
  }
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

    //if can't do anything, do nothing
    return NONE;
}

/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will have the snake attempt to move to the top right corner of the
 * board. If it cannot find a path, it will do the Manhattan move
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] headSpot - the head index of the snake
 * @param[in,out] contin - if there is no new corner, continue
 *
 * @return - the move the snake needs to make to get to the top right corner of
 *           the board
 *
 *****************************************************************************/
ValidMove Player::moveTopRight(const int *grid, int headSpot, bool &contin)
{
  int corner;
  int nextIndex;
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  BFSPaths BFSpath(&graph, headSpot);

  corner = (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - 1;
  if(grid[corner] != CLEAR_VALUE && grid[corner] != FOOD_VALUE)
    corner = newTopRightCorner(grid);

  if(corner == 1)
  {
    toTopRight = false;
    toTopLeft = true;
    contin = true;
    return NONE;
  }

  //try to stick to the very right wall

//if on right wall and can keep being in top row, do it
  if(headSpot < (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - PLAYFIELD_WIDTH
  && headSpot > PLAYFIELD_WIDTH && (headSpot + 1) % PLAYFIELD_WIDTH == 0)
  {
      //check if can keep moving right
      if(grid[headSpot + PLAYFIELD_WIDTH] == CLEAR_VALUE ||
        grid[headSpot + PLAYFIELD_WIDTH] == FOOD_VALUE)
          return UP;
  }

  //check if can move over to right wall at all, if can, do that and
  //override BFS- don't want to trap self if there's a block in the corner
  //check if the moving right will not allow it to move up after
  if((headSpot + 1) % PLAYFIELD_WIDTH != 0)
  {
    //see if can move right and will be able to move up after it
    if((grid[headSpot + 1] == CLEAR_VALUE || grid[headSpot + 1] == FOOD_VALUE)
    && (grid[headSpot + 1 + PLAYFIELD_WIDTH] == CLEAR_VALUE ||
      grid[headSpot + 1 + PLAYFIELD_WIDTH] == FOOD_VALUE))
      return RIGHT;
  }

  std::list<int>pathToFood = BFSpath.PathTo(corner);

  //check if there is a path
  //if there's not a path, do the Manhattan move
  if(!BFSpath.hasPath(corner))
    return ManhattanChecker(grid, headSpot);

  // go to it
  nextIndex = pathToFood.front();
  if(nextIndex == corner)
  {
    toTopRight = false;
    toTopLeft = true;
  }
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

    //if can't do anything, do nothing
    return NONE;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will have the snake attempt to move to the top left corner of the board.
 * If it cannot find a path, it will do the Manhattan move
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] headSpot - the head index of the snake
 * @param[in,out] contin - if there is no new corner, continue
 *
 * @return - the move the snake needs to make to get to the top left corner of
 *           the board
 *
 *****************************************************************************/
ValidMove Player::moveTopLeft(const int *grid, int headSpot, bool &contin)
{
  int corner;
  int nextIndex;
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  BFSPaths BFSpath(&graph, headSpot);

  corner = (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH;
  if(grid[corner] != CLEAR_VALUE && grid[corner] != FOOD_VALUE)
    corner = newTopLeftCorner(grid);

  if(corner == 1)
  {
    toTopRight = false;
    toTopLeft = true;
    contin = true;
    return NONE;
  }

  //to handle from any row, check if less than the playfield but within the
  //correct columns- make sure you can move up, if on row below the top row, make
  //sure you can also move left after moving into the top row

  //if in top row and can keep being in top row, do it
  if(headSpot < (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1 &&
  headSpot > (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - PLAYFIELD_WIDTH)
  {
    /*int thing = grid[headSpot -PLAYFIELD_WIDTH];
    int thing2 = FOOD_VALUE;
    std::cout << "In top row so should keep going" << std::endl;
    std::cout << "foodSecondTop: " << foodSecondTop << std::endl;
    std::cout << "grid[headSpot - PLAYFIELD_WIDTH]: " << thing << std::endl;
    std::cout << "FOOD_VALUE: " << thing2 << std::endl;
    std::cout << std::endl << std::endl;
    */
    if(foodSecondTop == true && grid[headSpot -PLAYFIELD_WIDTH] == FOOD_VALUE)
    {
      foodSecondTop = false;
      return DOWN;
    }
    //check if can keep moving right
    if(grid[headSpot - 1] == CLEAR_VALUE || grid[headSpot - 1] == FOOD_VALUE)
      return LEFT;
  }
  //check if can move over to top wall at all, if can, do that and
  //override BFS- don't want to trap self if there's a block in the corner
  //check if the moving up will not allow it to move left after
  if(headSpot < (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
  {
    if(foodSecondTop == true && grid[headSpot - 1] == FOOD_VALUE)
    {
      foodSecondTop = false;
      return LEFT;
    }
    //check if can move up, if so, check if can also move left after the
    //move up
    if((grid[headSpot + PLAYFIELD_WIDTH] == CLEAR_VALUE ||
      grid[headSpot + PLAYFIELD_WIDTH] == FOOD_VALUE)
      && (grid[headSpot + PLAYFIELD_WIDTH - 1] == CLEAR_VALUE ||
        grid[headSpot + PLAYFIELD_WIDTH - 1] == FOOD_VALUE))
        return UP;
  }


  std::list<int>pathToFood = BFSpath.PathTo(corner);

  //check if there is a path
  //if there's not a path, do the Manhattan move
  if(!BFSpath.hasPath(corner))
    return ManhattanChecker(grid, headSpot);

  // go to it
  nextIndex = pathToFood.front();
  if(nextIndex == corner)
  {
    toTopLeft = false;
    toBottomLeft = true;
  }
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

    //if can't do anything, do nothing
    return NONE;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will have the snake attempt to move to the bottom left corner of the
 * board. If it cannot find a path, it will do the Manhattan move
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] headSpot - the head index of the snake
 * @param[in,out] contin - if there is no new corner, continue
 *
 * @return - the move the snake needs to make to get to the bottom left corner
 *           of the board
 *
 *****************************************************************************/
ValidMove Player::moveBottomLeft(const int *grid, int headSpot, bool &contin)
{
  int corner;
  int nextIndex;
  Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

  BFSPaths BFSpath(&graph, headSpot);

  corner = 0;
  if(grid[corner] != CLEAR_VALUE && grid[corner] != FOOD_VALUE)
    corner = newBottomLeftCorner(grid);

  if(corner == 20000)
  {
    toTopLeft = false;
    searchingFood = true;
    contin = true;
    return NONE;
  }

  //if on column, keep there if can
  if(headSpot % PLAYFIELD_WIDTH == 0 && (headSpot > 0 &&
    headSpot < (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH))
  {
    //check if can keep moving down
    if(grid[headSpot - PLAYFIELD_WIDTH] == CLEAR_VALUE ||
      grid[headSpot - PLAYFIELD_WIDTH] == FOOD_VALUE)
      return DOWN;
  }
  //check if can move over to top wall at all, if can, do that and
  //override BFS- don't want to trap self if there's a block in the corner
  //check if the moving up will not allow it to move left after

//if not on the column, the go to it
  if(headSpot % PLAYFIELD_WIDTH != 0)
  {
    //check if can move left to get to spot
    if(grid[headSpot - 1] == CLEAR_VALUE || grid[headSpot - 1] == FOOD_VALUE)
      return LEFT;
  }


  std::list<int>pathToFood = BFSpath.PathTo(corner);

  //check if there is a path
  //if there's not a path, do the Manhattan move
  if(!BFSpath.hasPath(corner))
    return ManhattanChecker(grid, headSpot);


  // go to it
  nextIndex = pathToFood.front();
  if(nextIndex == corner)
  {
    searchingFood = true;
    toBottomLeft = false;
  }

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

    //if can't do anything, do nothing
    return NONE;
}


/**************************************************************************//**
 * @author Dr. Hinker, Jennifer Kulich
 *
 * @par Description:
 * This will make sure that the Manhattan move that's being made is a valid
 * move.
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] headSpot - the head index of the snake
 *
 * @return - the move the snake needs to make to get to the top left corner of
 *           the board
 *
 *****************************************************************************/
ValidMove Player::ManhattanChecker(const int *grid, int headSpot)
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

  else if(newPossibleMove == UP &&
    (grid[headSpot + PLAYFIELD_WIDTH] != CLEAR_VALUE &&
      grid[headSpot + PLAYFIELD_WIDTH] != FOOD_VALUE))
    return NONE;

  else if(newPossibleMove == DOWN &&
    (grid[headSpot - PLAYFIELD_WIDTH] != CLEAR_VALUE &&
      grid[headSpot - PLAYFIELD_WIDTH] != FOOD_VALUE))
    return NONE;

  else if(newPossibleMove == NONE)
    return NONE;

  return newPossibleMove;
}
