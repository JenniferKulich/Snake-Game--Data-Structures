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
    int headSpot =(head.second * PLAYFIELD_WIDTH) + head.first;
    int foodSpot = (food.second * PLAYFIELD_WIDTH) + food.first;

    int nextIndex = 0;
    bool contin = false;
    ValidMove nonSearchingMove;

    //construct a graph
    Graph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);

    //at the very beginning of the game, check if there are no obstacles on
    //the board
    if(startOfGame)
        checkBoardClear(grid);


    //if the board is clear, do the Hamilton circuit
    if(clearBoard == true)
    {
        //check to see if rows (height) is odd
        if(PLAYFIELD_HEIGHT % 2 != 0)
            return rowsOdd(head.first, head.second, food.first, food.second);

        //check to see if rows (height) is even
        if(PLAYFIELD_HEIGHT % 2 == 0)
            return rowsEven(head.first, head.second);
    }

    //check to see if food in top rows
    //if the food is in the top row, go to the bottom right corner instead of
    //going to the food
    int foodRow = food.second;
    if(searchingFood && !startingBFS && (foodRow == PLAYFIELD_HEIGHT - 2))
    {
        searchingFood = false;
        toBottomRight = true;
        foodSecondTop = true;
    }


    //if the food eaten (aka tail length) is less than the smaller of the width
    //and height of the board, just do BFS. Once gets length, won't do BFS
    checkIfTailLength();


    //if not searching for food, do BFS to the the bottom right, then top right
    //corner, then top left corner, then bottom left corner. Will set searching
    //for food to false and where've you're heading to true
    //NOTE: I know that this should be a seperate function. I did try this, and
    //because of the nature of what it does, it will go into an infinite loop
    //and cause the program to hang. I was not able to come up with a solution
    //for this, so I decided to not make it a seperate function
    while(!searchingFood && !startingBFS)
    {
        BFSPaths BFSpath(&graph, headSpot);

        //if the botom right is set to true, go to the bottom right side
        contin = false;
        if(toBottomRight)
        {
            nonSearchingMove= moveRightSide(grid,headSpot, contin);
            //if contin is true, then cannot move to that corner
            if(contin == true)
                continue;
            else
                return nonSearchingMove;
        }

        contin = false;

        //if the top right is set to true, go to the top right corner
        if(toTopRight)
        {
            nonSearchingMove= moveTopRight(grid,headSpot, contin);
            //if contin is true, then cannot move to that corner
            if(contin == true)
                continue;
            else
                return nonSearchingMove;
        }

        contin = false;

        //if the top left is set to true, go to the top left corner
        if(toTopLeft)
        {
            nonSearchingMove= moveTopLeft(grid,headSpot, contin);
            //if contin is true, then cannot move to that corner
            if(contin == true)
                continue;
            else
                return nonSearchingMove;
        }

        contin = false;

        //if the bottom left is set to true, go to the bottom left corner
        if(toBottomLeft)
        {
            nonSearchingMove= moveBottomLeft(grid,headSpot, contin);
            //if contin is true, then cannot move to that corner
            if(contin == true)
                continue;
            else
                return nonSearchingMove;
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

    //go to the first item in the list and determine where to move to
    nextIndex = pathToFood.front();

    //will calculate if the BFS move is possible and which way to move
    return BFSnextMove(grid, nextIndex, headSpot, foodSpot);


    //if abolsutly nothing can be done, return NONE to end game
    return NONE;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Will go throught the board at the very beginning of the game and check if it
 * is clear. Will set the appropriate flags if the board is found to be clear
 * or not
 *
 * @param[in] grid - The playfield and everything in it
 *
 *****************************************************************************/
void Player::checkBoardClear(const int *grid)
{
    //loop through the grid and look for an obstacle
    for(int i = 0; i < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; i++)
    {
        if(grid[i] != CLEAR_VALUE && grid[i] != HEAD_VALUE &&
          grid[i] != FOOD_VALUE)
        {
            clearBoard = false;
            startOfGame = false;
        }
        if(clearBoard == false)
            i = PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT;
    }

    //only needed this flag to know it was the very beginning of the game
    //don't need to be set true anymore since the board has been checked
    startOfGame = false;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Will check if the snake is the length of the smallest parameter of the board
 * (width or height). If the snake is smaller, than it will continue doing the
 * BFS move. If it isn't, it will go to the searching for food mode.
 *
 *****************************************************************************/
void Player::checkIfTailLength()
{
    int smallest;

    //check what's smaller, the width or height
    if(PLAYFIELD_WIDTH < PLAYFIELD_HEIGHT)
        smallest = PLAYFIELD_WIDTH;
    else
        smallest = PLAYFIELD_HEIGHT;

    //check if the snake is smaller than smallest parameter
    if(foodEaten < smallest)
    {
        startingBFS = true;
        searchingFood = false;
    }
    else
        startingBFS = false;

    //if the snake is equal to the smallest parameter, will begin the searching
    //for food part of the board traversal
    if(foodEaten == smallest)
        searchingFood = true;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Will take in the next index spot to move based on the BFS path. Will check if
 * the move that's going to be made is possible. Will also check if food is in
 * the top row- if it is, it will be handled differently.
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] nextIndex - where the snake will be moving next
 * @param[in] headSpot - index of where the snake head is
 * @param[in] foodSpot - index of where the food head is
 *
 * @return - which way the snake should move next
 *
 *****************************************************************************/
ValidMove Player::BFSnextMove(const int *grid, int nextIndex, int headSpot,
  int foodSpot)
{
    //check if nextIndex is to the left
    if(nextIndex == headSpot - 1)
    {
        //check if the food is along the edge traversal path
        foodAlongTraversal(grid, nextIndex, foodSpot);
        return LEFT;
    }

    //check if nextIndex is to the right
    else if(nextIndex == headSpot + 1)
    {
        foodAlongTraversal(grid, nextIndex, foodSpot);
        return RIGHT;
    }

    //check if nextIndex is down
    else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
    {
        foodAlongTraversal(grid, nextIndex, foodSpot);
        return DOWN;
    }

    //check if nextIndex is up
    else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
    {
        foodAlongTraversal(grid, nextIndex, foodSpot);
        return UP;
    }

    //if can't do anything, do nothing
    return NONE;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Will check if the next places along the traversal is food. If it is, will
 * not want to have searchingFood set to true and will want to proceed to the
 * correct corner
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] nextIndex - where the snake will be moving next
 * @param[in] foodSpot - index of where the food head is
 *
 *****************************************************************************/
void Player::foodAlongTraversal(const int *grid, int nextIndex, int foodSpot)
{
    //check the index the snake is moving to next is food.
    if(grid[nextIndex] == FOOD_VALUE)
    {
        foodEaten +=1;
        searchingFood = false;

        //check if food it's in top row, if it is, set toTopLeft to true
        if(nextIndex <= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) &&
            nextIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
            toTopLeft = true;

        //check if food is on left wall, if it is, set toBottomLeft to true
        else if(foodSpot % PLAYFIELD_WIDTH == 0 &&
                (nextIndex <= (PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH) - 1 &&
                 nextIndex >= 0))
            toBottomLeft = true;

        //if not along those walls, just move to the bottom right
        else
            toBottomRight = true;
    }
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
 * Will take in a move. If the move canot be made, a new move will be picked,
 * and the function will be recurrively called to see if the move can be made.
 * If the move can be made, it will be returned. Will keep track of how many
 * times the function recurrively called. If it's gone through all moves,
 * it will return NONE since no moves can made.
 *
 * @param[in] grid - The playfield with everything in it
 * @param[in,out] move - the move that will be made
 * @param[in]  origionalMove  - the origional move
 * @param[in] count - how many times we've tried to make a different move
 * @param[in] headIndex - where the head of the snake is located
 *
 *****************************************************************************/
void Player::newMove(const int *grid, ValidMove &move, ValidMove origionalMove,
                     int count, int headIndex)
{
    //base case
    //if first time calling function, set origionalMove to the move passed in
    if(count == 0)
        origionalMove = move;

    //exit condition
    //if the cycled through all possible moves and cannot make any, return NONE
    if(count > 0 && move == origionalMove)
    {
        move = NONE;
        return;
    }

    count = count + 1;
    //if the right move cannot be made, try moving down
    //check to make sure right is not clear or on far right wall
    if((move == RIGHT) && (grid[headIndex + 1] != CLEAR_VALUE ||
        (headIndex + 1) == PLAYFIELD_WIDTH - 1))
    {
        move = DOWN;
        newMove(grid, move, origionalMove, count, headIndex);
    }

    //if the down move cannot be made, try moving left
    else if((move == DOWN) && (grid[headIndex - PLAYFIELD_WIDTH] != CLEAR_VALUE ||
            (headIndex <= PLAYFIELD_WIDTH - 1)))
    {
        move = LEFT;
        newMove(grid, move, origionalMove,count,headIndex);
    }

    //if the LEFT move cannot be made, try moving up
    else if((move == LEFT) && (grid[headIndex - 1] != CLEAR_VALUE ||
            (headIndex % PLAYFIELD_WIDTH) == 0))
    {
        move = UP;
        newMove(grid,move,origionalMove,count,headIndex);
    }


    //fi the UP move cannot be made, try moving right
    else if((move == UP) && (grid[headIndex + PLAYFIELD_WIDTH] != CLEAR_VALUE ||
            ((headIndex / PLAYFIELD_WIDTH) == (PLAYFIELD_HEIGHT - 1))))
    {
        move = RIGHT;
        newMove(grid,move,origionalMove,count,headIndex);
    }

}

/**************************************************************************//**
 * @author Jennifer Kulich
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

    //loop through everything in the row until get to end- easiest to find left
    //end
    while(trialIndex % PLAYFIELD_WIDTH != 0)
        trialIndex += 1;

    mostRightIndex = trialIndex - 1;

    //check to see if the index is in the two rows below the top row. If it is,
    //then will want side index to go to to be below it somewhere
    if((playerIndex <= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - (2 * PLAYFIELD_WIDTH)
        && playerIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - (3 * PLAYFIELD_WIDTH))
        || (playerIndex >= (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH))
        return newTopSideIndex(grid, trialIndex);


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
        //check to make sure that that spot and the spot above it is empty
        if((grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE) &&
                (grid[i + PLAYFIELD_WIDTH] == CLEAR_VALUE
                 || grid[i + PLAYFIELD_WIDTH] == FOOD_VALUE))
            return i;
    }

    //if cannot do that, start down at the bottom
    for(int i = PLAYFIELD_WIDTH - 1; i < mostRightIndex; i += PLAYFIELD_WIDTH)
    {
        //check if that spot is empty
        if(grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE)
            return i;
    }

    //return massive number if got nothing
    return 20000;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Will loop through to find a spot on the right side of the board that is free
 * for the snake to go to
 *
 * @param[in] grid - The playfield and everything in it
 * @param[in] trialIndex - index cpassed in to get a new side index
 *
 * @return - the new index on the side of the board to go to
 *
 *****************************************************************************/
int Player::newTopSideIndex(const int *grid, int trialIndex)
{
    int newIndex = trialIndex - PLAYFIELD_WIDTH;

    //make the index be further down
    while(newIndex >= PLAYFIELD_WIDTH - 1)
    {
        //make sure the new index that's being check is clear or food and less than
        //two rows below top row
        if((grid[newIndex - PLAYFIELD_WIDTH] == CLEAR_VALUE ||
            grid[newIndex - PLAYFIELD_WIDTH] == FOOD_VALUE) &&
            (newIndex < (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - (3 * PLAYFIELD_WIDTH)))
            return newIndex;

        //decrease the row since the spot was not clear, food, or below two rows
        //from the top row
        newIndex -= PLAYFIELD_WIDTH;
    }

    return newIndex;
}


/**************************************************************************//**
 * @author Jennifer Kulich
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
            i > (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH + 1; i--)
    {
        if(grid[i] == CLEAR_VALUE || grid[i] == FOOD_VALUE)
            return i;
    }

    //if can't do that, return 1 which means that will just go to next corner
    return 1;
}


/**************************************************************************//**
 * @author Jennifer Kulich
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
 * @author Jennifer Kulich
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

    //if can't do that, return really big number which means that will just go
    //to the food
    return 20000;
}


/**************************************************************************//**
 * @author Jennifer Kulich
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


    // go the first index on the list
    nextIndex = pathToFood.front();

    //if the next index gets to the edge it was trying to get to, switch mode
    if(nextIndex == rightMoveTo)
    {
        toBottomRight = false;
        toTopRight = true;
    }

    //check to see which direction going
    return traversalMovementChecker(headSpot, nextIndex);

}

/**************************************************************************//**
 * @author Jennifer Kulich
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

    // go to next index
    nextIndex = pathToFood.front();

    //if the next index gets to the edge it was trying to get to, switch mode
    if(nextIndex == corner)
    {
        toTopRight = false;
        toTopLeft = true;
    }

    //check to see which direction going
    return traversalMovementChecker(headSpot, nextIndex);
}


/**************************************************************************//**
 * @author Jennifer Kulich
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
        if(foodSecondTop == true)
        {
            //check if the food is below it, and not onde down and right of the corner
            if(grid[headSpot - PLAYFIELD_WIDTH] == FOOD_VALUE &&
              headSpot - 1 != (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
            {
                foodSecondTop = false;
                return DOWN;
            }

            //if the food is down and right one of the corner, just go to the bottom
            //left corner from there
            else if (grid[headSpot - PLAYFIELD_WIDTH] == FOOD_VALUE &&
                     headSpot - 1 == (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) - PLAYFIELD_WIDTH)
            {
                toTopLeft = false;
                toBottomLeft = true;
                foodSecondTop = false;
                return DOWN;
            }
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

    //if the next index gets to the edge it was trying to get to, switch mode
    if(nextIndex == corner)
    {
        toTopLeft = false;
        toBottomLeft = true;
    }

    //check to see which direction going
    return traversalMovementChecker(headSpot, nextIndex);
}


/**************************************************************************//**
 * @author Jennifer Kulich
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

    //if the next index gets to the edge it was trying to get to, switch mode
    if(nextIndex == corner)
    {
        searchingFood = true;
        toBottomLeft = false;
    }

    //check to see which direction going
    return traversalMovementChecker(headSpot, nextIndex);
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Will take in an index to move to and compare it to the headIndex. Based on
 * where it is located from the head, it will return which way the snake
 * should move
 *
 * @param[in] headSpot - the head index of the snake
 * @param[in] nextIndex - the index of where the snake is to move to next
 * @param[in,out] contin - if there is no new corner, continue
 *
 * @return - the move the snake needs to make to get to the next spot on the
 *           board
 *
 *****************************************************************************/
ValidMove Player::traversalMovementChecker(int headSpot, int nextIndex)
{
    //check if next index is left
    if(nextIndex == headSpot - 1)
        return LEFT;
    //check if next index if right
    else if(nextIndex == headSpot + 1)
        return RIGHT;
    //checking if next index is down
    else if(nextIndex == headSpot - PLAYFIELD_WIDTH)
        return DOWN;
    //check if next index is up
    else if(nextIndex == headSpot + PLAYFIELD_WIDTH)
        return UP;

    //if can't do anything, do nothing
    return NONE;
}


/**************************************************************************//**
 * @author Jennifer Kulich
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
    ValidMove newPossibleMove = NONE;

    //get the Manhattan move
    newPossibleMove = ManhattanMove(grid);

    //if the move is RIGHT, make sure it can move right
    if(newPossibleMove == RIGHT && (grid[headSpot + 1] != CLEAR_VALUE &&
        grid[headSpot + 1] != FOOD_VALUE))
        return NONE;

    //if the move is Left, make sure can move left
    else if(newPossibleMove == LEFT && (grid[headSpot - 1] != CLEAR_VALUE &&
            grid[headSpot - 1] != FOOD_VALUE))
        return NONE;

    //if the move is up, make sure it can move up
    else if(newPossibleMove == UP &&
            (grid[headSpot + PLAYFIELD_WIDTH] != CLEAR_VALUE &&
             grid[headSpot + PLAYFIELD_WIDTH] != FOOD_VALUE))
        return NONE;

    //if the move is down, make sure can move down
    else if(newPossibleMove == DOWN &&
            (grid[headSpot - PLAYFIELD_WIDTH] != CLEAR_VALUE &&
             grid[headSpot - PLAYFIELD_WIDTH] != FOOD_VALUE))
        return NONE;

    //if the move returned is none, return none
    else if(newPossibleMove == NONE)
        return NONE;

    //return the new move- none, as a fallback if nothing came back from
    //Manhattan move
    return newPossibleMove;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * If there are an even number of rows, the snake will zig zag back and forth,
 * ignoring obstacles and will leave the left column open so it can traverse it
 * back down
 *
 * @param[in] headX - x position of the snake head
 * @param[in] headY - y position of the snake head
 *
 * @return - the move the snake needs to make to continue the pattern
 *
 *****************************************************************************/
ValidMove Player::rowsEven(int headX, int headY)
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


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * If there are an odd number of rows, the snake will zig zag back and forth,
 * ignoring obstacles leaving the left column open so it can traverse it
 * back down, and skipping a row on the board to traverse
 *
 * @param[in] headX - x position of the snake head
 * @param[in] headY - y position of the snake head
 * @param[in] foodX - x position of the food
 * @param[in] foodY - y position of the food
 *
 * @return - the move the snake needs to make to continue the pattern
 *
 *****************************************************************************/
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
